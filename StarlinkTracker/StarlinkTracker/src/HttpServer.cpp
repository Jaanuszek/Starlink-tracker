#include "../include/HttpServer.h"
#include <iostream>

using json = nlohmann::json;

HttpServer::HttpServer(bool* isCountriesBorderVisiblePtr,
    std::vector<std::unique_ptr<Starlink>>* starlinksPtr,
    std::string apiKey,
    std::tm localTimeRef)
    : isCountriesBorderVisible(isCountriesBorderVisiblePtr),
    apiKey(apiKey),
    localTime(localTimeRef)
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
                std::string satID = std::to_string(id);
                std::string url = "https://api.n2yo.com/rest/v1/satellite/tle/" + satID + "&apiKey=" + apiKey;
                std::string satData;
                satelliteDataAPI.fetchDataFromAPI(url, satData);

                if (!satData.empty()) {
                    JSONParser::ParseJSONSattelite(satData, satellites);
                }
            }

            res.set_content(json{ {"message", "Starlinks loaded successfully"}, {"count", satellites.size()} }.dump(), "application/json");
        }
        else {
            res.status = 400;
            res.set_content(json{ {"error", "Invalid or missing 'starlinkIds'"} }.dump(), "application/json");
        }
        });

    svr.Post("/DisableStarlink", [](const httplib::Request& req, httplib::Response& res) {
        json data = json::parse(req.body);

        int starlinkId = data.value("starlinkId", 0);
        disableStarlink(starlinkId);

        res.set_content(json{ {"message", "Starlink disabled"} }.dump(), "application/json");
        });

    svr.Post("/EnableStarlink", [](const httplib::Request& req, httplib::Response& res) {
        json data = json::parse(req.body);

        int starlinkId = data.value("starlinkId", 0);
        enableStarlink(starlinkId);

        res.set_content(json{ {"message", "Starlink enabled"} }.dump(), "application/json");
        });

    svr.Post("/Highlight", [](const httplib::Request& req, httplib::Response& res) {
        json data = json::parse(req.body);

        int starlinkId = data.value("starlinkId", 0);
        highlightStarlink(starlinkId);

        res.set_content(json{ {"message", "Starlink highlighted"} }.dump(), "application/json");
        });

    svr.Post("/ToggleCountriesBorder", [this](const httplib::Request& req, httplib::Response& res) {
        toggleCountriesBorder(isCountriesBorderVisible);

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

    svr.Post("/ShowTrajectory", [](const httplib::Request& req, httplib::Response& res) {
        json data = json::parse(req.body);

        int starlinkId = data.value("starlinkId", 0);
        showTrajectory(starlinkId);

        res.set_content(json{ {"message", "Trajectory displayed"} }.dump(), "application/json");
        });

    svr.Post("/RotateCamera", [](const httplib::Request& req, httplib::Response& res) {
        json data = json::parse(req.body);

        float angleX = data.value("angleX", 0.0f);
        float angleY = data.value("angleY", 0.0f);
        float angleZ = data.value("angleZ", 0.0f);
        rotateCamera(angleX, angleY, angleZ);

        res.set_content(json{ {"message", "Camera rotated"} }.dump(), "application/json");
        });

    svr.Get(R"(/GetStarlinkInfo/(\d+))", [](const httplib::Request& req, httplib::Response& res) {
        std::string id = req.matches[1];

        float positionX = 123.45;
        float positionY = 67.89;
        float positionZ = 101.11;
        float speedX = 1.1;
        float speedY = 2.2;
        float speedZ = 3.3;
        std::string country = "USA";

        std::cout << "Getting starlink info" << std::endl;

        json response = {
            {"id", id},
            {"tle", "Sample TLE Data"},
            {"positionX", positionX},
            {"positionY", positionY},
            {"positionZ", positionZ},
            {"speedX", speedX},
            {"speedY", speedY},
            {"speedZ", speedZ},
            {"country", country}
        };

        res.set_content(response.dump(), "application/json");
        });
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