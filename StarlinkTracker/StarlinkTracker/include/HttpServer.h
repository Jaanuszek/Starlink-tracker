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
    HttpServer(bool* isCountriesBorderVisiblePtr, std::vector<std::unique_ptr<Starlink>>* starlinksPtr, std::string apiKey, std::tm localTimeRef);
    ~HttpServer();

    void start();
    void stop();

private:
    void setupEndpoints();

    std::thread serverThread;
    httplib::Server svr;

    bool* isCountriesBorderVisible;
    std::vector<std::unique_ptr<Starlink>>* starlinks;
    std::string apiKey;
    std::tm localTime;
};

#endif // HTTPSERVER_HPP