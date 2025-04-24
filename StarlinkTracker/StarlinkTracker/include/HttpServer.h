#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include <httplib.h>
#include <json.hpp>
#include <thread>
#include <unordered_map>
#include "./fetchApi.h"
#include "./Models/Starlink.h"
#include "./Camera.h"

class HttpServer {
public:
    HttpServer(bool* isCountriesBorderVisiblePtr, std::string apiKey, std::tm localTimeRef, Camera& cameraRef, std::vector<std::unique_ptr<Starlink>>& starlinksRef);
    ~HttpServer();

    void start();
    void stop();
    std::vector<Satellite>& getSatellitesInfo() { 
        return satellites;
    }
    void clearSateliteVector() {
        satellites.clear();
    }
    const std::unordered_map<int, std::pair<bool, bool>>& getStarlinkVisibilityMap() const {
        return starlinkVisibilityMap;
    }

private:
    void setupEndpoints();
    std::string getCityNameFromCoordinates(float latitude, float longitude);

    std::thread serverThread;
    httplib::Server svr;

    bool* isCountriesBorderVisible;
    std::vector<Satellite> satellites;
    std::string apiKey;
    std::tm localTime;

    std::unordered_map<int, std::pair<bool, bool>> starlinkVisibilityMap;
    std::vector<std::unique_ptr<Starlink>>& starlinks;

    Camera& camera;
};

#endif // HTTPSERVER_HPP