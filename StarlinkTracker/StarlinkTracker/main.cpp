#include <iostream>  
#include <fstream>  
#include <string>  
#include <curl/curl.h>  
#include <glad/glad.h>  
#include <GLFW/glfw3.h>  
#include <json.hpp>  

using std::string;  

int main() {  
   // opengl test  
   if (!glfwInit())  
   {  
       std::cout << "Failed to initialize GLFW" << std::endl;  
   }  
   else  
   {  
       std::cout << "GLFW initialized" << std::endl;  
   }  

   // libcurl test  
   CURL* curl;  
   CURLcode res;  

   curl_global_init(CURL_GLOBAL_DEFAULT);  

   curl_version_info_data* version_info = curl_version_info(CURLVERSION_NOW);
   if (version_info == nullptr) {
	   std::cerr << "Failed to get curl version info" << std::endl;
	   return 1;
   }
   if (version_info->version == nullptr) {
	   std::cerr << "Failed to get curl version" << std::endl;
	   return 1;
   }
   if (version_info->ssl_version == nullptr) {
	   std::cerr << "Failed to get curl ssl version" << std::endl;
	   //return 1;
   }
   else
   {
       std::cout << "SSL version: " << version_info->ssl_version << std::endl;
   }
   std::cout << "libcurl version: " << version_info->version << std::endl;


   // Creating a handle for the connection  
   curl = curl_easy_init();  
   if (curl) {  
       // Setting the URL to download  
       std::string API_KEY;  
       std::fstream file("apiKey.txt");  
       if (file.is_open()) {  
           std::getline(file, API_KEY);  
           file.close();  
       }  
       else {  
           std::cerr << "Failed to open apiKey.txt" << std::endl;  
       }  
       std::cout << "API Key apiKey: " << API_KEY << std::endl;  

       string SAT_ID = "56144";  
       string LAT = "52.2298";  
       string LON = "21.0122";  
       string ALT = "100";  
       string url = "https://api.n2yo.com/rest/v1/satellite/positions/" + SAT_ID + "/" + LAT + "/" + LON + "/" + ALT + "/1/?apiKey=" + API_KEY;  
       std::cout << url.c_str() << std::endl;  
       //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //better curl logs
       curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
       curl_easy_setopt(curl, CURLOPT_URL, url.c_str());  


       // Set the User-Agent to avoid being blocked by some servers  
       curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");  

       // Performing the request  
       res = curl_easy_perform(curl);  

       // Checking for errors  
       if (res != CURLE_OK) {  
           std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;  
       }  
       else {  
           std::cout << "\nStarlink data fetched!" << std::endl;  
       }  

       // Releasing the handle  
       curl_easy_cleanup(curl);  
   }  

   // Releasing global resources  
   curl_global_cleanup();  

   // nlohmann::json test  
   nlohmann::json j = {  
       {"pi", 3.141},  
       {"happy", true},  
       {"name", "niels"},  
       {"nothing", nullptr},  
       {"answer", {  
           {"everything", 42}  
       }},  
       {"list", {1, 0, 2}},  
       {"object", {  
           {"currency", "usd"},  
           {"value", 42.99}  
       }}  
   };  
   std::cout << j["pi"] << std::endl;  

   return 0;  
}