#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include <httplib.h>
#include <json.hpp>
#include <thread>
#include "StarlinkService.h"

class HttpServer {
public:
    HttpServer(bool* isCountriesBorderVisiblePtr);
    ~HttpServer();

    void start();
    void stop();

private:
    void setupEndpoints();

    std::thread serverThread;
    httplib::Server svr;

    bool* isCountriesBorderVisible;
};

#endif // HTTPSERVER_HPP
