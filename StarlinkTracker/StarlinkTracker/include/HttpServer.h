#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include <httplib.h>
#include <json.hpp>
#include <thread>
#include "StarlinkService.h"
#include "./fetchApi.h"
#include "./Models/Starlink.h"

class HttpServer {
public:
    HttpServer(bool* isCountriesBorderVisiblePtr, std::string apiKey, std::tm localTimeRef);
    ~HttpServer();

    void start();
    void stop();
    std::vector<Satellite>& getSatellitesInfo() { 
        return satellites;
    }
    void clearSateliteVector() {
        satellites.clear();
    }

private:
    void setupEndpoints();

    std::thread serverThread;
    httplib::Server svr;

    bool* isCountriesBorderVisible;
    std::vector<Satellite> satellites;
    std::string apiKey;
    std::tm localTime;
};

#endif // HTTPSERVER_HPP