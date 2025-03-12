#include <iostream>
#include "external/curl/curl.h"

int main() {
    CURL* curl;
    CURLcode res;

    // Inicjalizowanie biblioteki libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);

    // Tworzenie uchwytu dla po��czenia
    curl = curl_easy_init();

    if (curl) {
        // Ustawianie URL do pobrania
        curl_easy_setopt(curl, CURLOPT_URL, "http://www.example.com");

        // Wykonanie zapytania
        res = curl_easy_perform(curl);

        // Sprawdzanie, czy wyst�pi�y b��dy
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        else {
            std::cout << "Strona zosta�a pobrana pomy�lnie!" << std::endl;
        }

        // Zwalnianie uchwytu
        curl_easy_cleanup(curl);
    }

    // Zwalnianie zasob�w globalnych
    curl_global_cleanup();

    return 0;
}