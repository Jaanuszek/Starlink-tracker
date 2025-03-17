#include "../include/fetchApi.h"

fetchApi::fetchApi()
{
	Curl = curl_easy_init();
	if (!Curl) {
		std::cerr << "[ERROR] Failed to initialize CURL" << std::endl;
	}
}

fetchApi::~fetchApi()
{
	curl_easy_cleanup(Curl);
}

size_t fetchApi::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	size_t totalSize = size * nmemb;
	if (!userp) {
		std::cerr << "[ERROR] No user pointer provided!" << std::endl;
		return 0;
	}
	std::string* satData = static_cast<std::string*>(userp);
	*satData += std::string(static_cast<char*>(contents), totalSize);
	return totalSize;
}

void fetchApi::fetchDataFromAPI(const std::string& url, std::string& data)
{
	if (Curl)
	{
		curl_easy_setopt(Curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(Curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(Curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
		curl_easy_setopt(Curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(Curl, CURLOPT_WRITEDATA, &data);

		Res = curl_easy_perform(Curl);
		if (Res != CURLE_OK) {
			std::cerr << "[ERROR] curl_easy_perform() failed: " << curl_easy_strerror(Res) << std::endl;
		}
	}
}