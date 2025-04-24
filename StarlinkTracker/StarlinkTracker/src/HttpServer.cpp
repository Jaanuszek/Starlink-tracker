#include "../include/HttpServer.h"
#include <iostream>

using json = nlohmann::json;

HttpServer::HttpServer(bool* isCountriesBorderVisiblePtr,
    std::string apiKey,
    std::tm localTimeRef,
    Camera& cameraRef,
    std::vector<std::unique_ptr<Starlink>>& starlinksRef)
    : isCountriesBorderVisible(isCountriesBorderVisiblePtr),
    apiKey(apiKey),
    localTime(localTimeRef),
    camera(cameraRef),
    starlinks(starlinksRef)
{
}

HttpServer::~HttpServer() {
    stop();
}

void HttpServer::setupEndpoints() {
    svr.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "POST, GET, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type"}
        });

    svr.Post("/LoadStarlinks", [this](const httplib::Request& req, httplib::Response& res) {
        json data = json::parse(req.body);

        if (data.contains("starlinkIds") && data["starlinkIds"].is_array()) {
            std::vector<int> starlinkIds = data["starlinkIds"];

            fetchApi satelliteDataAPI;
            JSONParser jsonParser;

            for (int id : starlinkIds) {
                if (starlinkVisibilityMap.find(id) == starlinkVisibilityMap.end()) {
                    std::string satID = std::to_string(id);
                    std::string url = "https://api.n2yo.com/rest/v1/satellite/tle/" + satID + "&apiKey=" + apiKey;
                    std::string satData;
                    satelliteDataAPI.fetchDataFromAPI(url, satData);

                    if (!satData.empty()) {
                        JSONParser::ParseJSONSattelite(satData, satellites);
                        starlinkVisibilityMap[id] = { true, false };
                    }
                }
            }

            res.set_content(json{ {"message", "Starlinks loaded successfully"}, {"count", satellites.size()} }.dump(), "application/json");
        }
        else {
            res.status = 400;
            res.set_content(json{ {"error", "Invalid or missing 'starlinkIds'"} }.dump(), "application/json");
        }
        });

    svr.Get("/GetLoadedStarlinksInfo", [this](const httplib::Request& req, httplib::Response& res) {
        json result = json::array();

        for (const auto& [id, visData] : starlinkVisibilityMap) {
            result.push_back({
                {"id", id},
                {"visible", visData.first},
                {"trajectoryVisible", visData.second}
                });
        }

        res.set_content(result.dump(), "application/json");
        });;

    svr.Post("/ToggleStarlinkVisibility", [this](const httplib::Request& req, httplib::Response& res) {
        json data = json::parse(req.body);
        int starlinkId = data.value("starlinkId", 0);

        auto it = starlinkVisibilityMap.find(starlinkId);
        if (it != starlinkVisibilityMap.end()) {
            it->second.first = !it->second.first; // toggle visible

            json response = {
                {"message", "Starlink visibility toggled"},
                {"starlinkId", starlinkId},
                {"visible", it->second.first}
            };
            res.set_content(response.dump(), "application/json");
        }
        else {
            res.status = 404;
            res.set_content(json{ {"error", "Starlink ID not found"} }.dump(), "application/json");
        }
        });

    svr.Post("/Highlight", [](const httplib::Request& req, httplib::Response& res) {
        json data = json::parse(req.body);

        int starlinkId = data.value("starlinkId", 0);

        res.set_content(json{ {"message", "Starlink highlighted"} }.dump(), "application/json");
        });

    svr.Post("/ToggleCountriesBorder", [this](const httplib::Request& req, httplib::Response& res) {
        *isCountriesBorderVisible = !(*isCountriesBorderVisible);

        json response = {
            {"message", "Countries border visibility toggled"},
            {"isCountriesBorderVisible", *isCountriesBorderVisible}
        };
        res.set_content(response.dump(), "application/json");
        });

    svr.Get("/GetCountriesBorderVisibility", [this](const httplib::Request& req, httplib::Response& res) {
        json response = {
            {"isCountriesBorderVisible", *isCountriesBorderVisible}
        };
        res.set_content(response.dump(), "application/json");
        });

    svr.Post("/ToggleTrajectory", [this](const httplib::Request& req, httplib::Response& res) {
        json data = json::parse(req.body);
        int starlinkId = data.value("starlinkId", 0);

        auto it = starlinkVisibilityMap.find(starlinkId);
        if (it != starlinkVisibilityMap.end()) {
            it->second.second = !it->second.second;

            json response = {
                {"message", "Trajectory visibility toggled"},
                {"starlinkId", starlinkId},
                {"trajectoryVisible", it->second.second}
            };
            res.set_content(response.dump(), "application/json");
        }
        else {
            res.status = 404;
            res.set_content(json{ {"error", "Starlink ID not found"} }.dump(), "application/json");
        }
        });

    svr.Post("/RotateCamera", [this](const httplib::Request& req, httplib::Response& res) {
        json data = json::parse(req.body);
        std::cout << "ROTATING" << std::endl;

        float angleX = data.value("angleX", 0.0f);
        float angleY = data.value("angleY", 0.0f);

        camera.MoveCamera(angleX, angleY);

        res.set_content(json{ {"message", "Camera moved"} }.dump(), "application/json");
        });

    svr.Get(R"(^/GetStarlinkInfo/(\d+)$)", [this](const httplib::Request& req, httplib::Response& res) {
        int id = std::stoi(req.matches[1]);

        auto it = std::find_if(starlinks.begin(), starlinks.end(),
            [id](const std::unique_ptr<Starlink>& s) {
                return s->getSatelliteInfo().satid == id;
            });

        if (it == starlinks.end()) {
            json error = {
                {"error", "Starlink not found"}
            };
            res.status = 404;
            res.set_content(error.dump(4), "application/json");
            return;
        }

        const Starlink* starlink = it->get();
        const Satellite& info = starlink->getSatelliteInfo();
        libsgp4::Vector pos = starlink->getCurrentPosition();
        libsgp4::Vector vel = starlink->getCurrentVelocity();

        std::string country = getCityNameFromCoordinates(info.latitude, info.longitude);

        json response = {
            {"id", info.satid},
            {"satname", info.satname},
            {"tleLine1", info.tleLine1},
            {"tleLine2", info.tleLine2},
            {"latitude", info.latitude},
            {"longitude", info.longitude},
            {"altitude", info.altitude},
            {"positionX", pos.x},
            {"positionY", pos.y},
            {"positionZ", pos.z},
            {"speedX", vel.x},
            {"speedY", vel.y},
            {"speedZ", vel.z},
            {"country", country}
        };

        res.set_content(response.dump(4), "application/json");
        });
}

std::string HttpServer::getCityNameFromCoordinates(float latitude, float longitude) {
    fetchApi api;
    std::string url = "https://nominatim.openstreetmap.org/reverse?format=json&lat=" + std::to_string(latitude) + "&lon=" + std::to_string(longitude);
    std::string responseData;

    api.fetchDataFromAPI(url, responseData);

    try {
        nlohmann::json root = nlohmann::json::parse(responseData);

        if (root.contains("address") && root["address"].contains("country")) {
            return root["address"]["country"].get<std::string>();
        }
    }
    catch (const nlohmann::json::exception& e) {
        std::cerr << "[ERROR] Failed to parse JSON response from OpenStreetMap: " << e.what() << std::endl;
    }

    return "Ocean or Unknown location";
}

void HttpServer::start() {
    setupEndpoints();
    serverThread = std::thread([this]() {
        svr.listen("localhost", 8080);
        });
}

void HttpServer::stop() {
    svr.stop();
    if (serverThread.joinable()) {
        serverThread.join();
    }
}