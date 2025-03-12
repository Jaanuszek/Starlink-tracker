#include <iostream>
#include "external/curl/curl.h"

int main() {
    CURL* curl;
    CURLcode res;

    // Inicjalizowanie biblioteki libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);

    // Tworzenie uchwytu dla po³¹czenia
    curl = curl_easy_init();

    if (curl) {
        // Ustawianie URL do pobrania
        curl_easy_setopt(curl, CURLOPT_URL, "http://www.example.com");

        // Wykonanie zapytania
        res = curl_easy_perform(curl);

        // Sprawdzanie, czy wyst¹pi³y b³êdy
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        else {
            std::cout << "Strona zosta³a pobrana pomyœlnie!" << std::endl;
        }

        // Zwalnianie uchwytu
        curl_easy_cleanup(curl);
    }

    // Zwalnianie zasobów globalnych
    curl_global_cleanup();

    return 0;
}