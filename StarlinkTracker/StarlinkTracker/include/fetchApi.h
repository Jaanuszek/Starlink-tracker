#ifndef FETCHAPI_H
#define FETCHAPI_H

#include <iostream>
#include <string>
#include <functional>
#include <curl/curl.h>

class fetchApi
{
private:
	CURL* Curl;
	CURLcode Res;
	static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
public:
	fetchApi();
	~fetchApi();
	void fetchDataFromAPI(const std::string& url, std::string& data);
};

#endif