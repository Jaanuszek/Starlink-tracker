#include <iostream>  
#include <fstream>  
#include <string>  
#include <curl/curl.h>  
#include <glad/glad.h>  
#include <GLFW/glfw3.h>  
#include <json.hpp>  

using std::string;

// Wierzcho³ki trójk¹ta (wspó³rzêdne OpenGL)
const float triangleVertices[3][2] = {
    { 0.0f,  0.5f},  // Górny wierzcho³ek
    {-0.5f, -0.5f},  // Lewy dolny
    { 0.5f, -0.5f}   // Prawy dolny
};

// Funkcja sprawdzaj¹ca, czy punkt (px, py) znajduje siê w trójk¹cie
bool isPointInTriangle(float px, float py) {
    auto [x1, y1] = triangleVertices[0];
    auto [x2, y2] = triangleVertices[1];
    auto [x3, y3] = triangleVertices[2];

    // Obliczanie pól powierzchni trójk¹tów metod¹ iloczynu wektorowego
    float A = 0.5 * (-y2 * x3 + y1 * (-x2 + x3) + x1 * (y2 - y3) + x2 * y3);
    float sign = A < 0 ? -1.0f : 1.0f;

    float s = sign * (y1 * x3 - x1 * y3 + (y3 - y1) * px + (x1 - x3) * py);
    float t = sign * (x1 * y2 - y1 * x2 + (y1 - y2) * px + (x2 - x1) * py);

    return s > 0 && t > 0 && (s + t) < 2 * sign * A;
}

// Funkcja obs³ugi klikniêcia myszk¹
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        // Pobranie pozycji kursora w pikselach
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Pobranie rozmiaru okna
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        // Konwersja wspó³rzêdnych pikselowych do OpenGL (-1 do 1)
        float x = (xpos / width) * 2.0f - 1.0f;
        float y = 1.0f - (ypos / height) * 2.0f;

        // Sprawdzenie, czy punkt jest w trójk¹cie
        if (isPointInTriangle(x, y)) {
            std::cout << "Klikniêto w trójk¹t! Wspó³rzêdne OpenGL: (" << x << ", " << y << ")\n";
        }
    }
}

int main() {
    // Inicjalizacja GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Tworzenie okna
    GLFWwindow* window = glfwCreateWindow(800, 600, "Kliknij w trojkat", NULL, NULL);
    if (!window) {
        std::cerr << "B³¹d tworzenia okna GLFW!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouse_button_callback);  // Ustawienie obs³ugi myszy

    // Inicjalizacja GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "B³¹d inicjalizacji GLAD!" << std::endl;
        return -1;
    }

    // Ustawienie viewportu
    glViewport(0, 0, 800, 600);

    // Definicja wierzcho³ków trójk¹ta
    float vertices[] = {
         0.0f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };

    // Tworzenie VAO i VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Shader
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        void main() {
            gl_Position = vec4(aPos, 1.0);
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0, 0.5, 0.2, 1.0);
        }
    )";

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Pêtla renderuj¹ca
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Sprz¹tanie
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

//int main() {  
//   // opengl test  
//   if (!glfwInit())  
//   {  
//       std::cout << "Failed to initialize GLFW" << std::endl;  
//   }  
//   else  
//   {  
//       std::cout << "GLFW initialized" << std::endl;  
//   }  
//
//   // libcurl test  
//   CURL* curl;  
//   CURLcode res;  
//
//   curl_global_init(CURL_GLOBAL_DEFAULT);  
//
//   curl_version_info_data* version_info = curl_version_info(CURLVERSION_NOW);
//   if (version_info == nullptr) {
//	   std::cerr << "Failed to get curl version info" << std::endl;
//	   return 1;
//   }
//   if (version_info->version == nullptr) {
//	   std::cerr << "Failed to get curl version" << std::endl;
//	   return 1;
//   }
//   if (version_info->ssl_version == nullptr) {
//	   std::cerr << "Failed to get curl ssl version" << std::endl;
//	   //return 1;
//   }
//   else
//   {
//       std::cout << "SSL version: " << version_info->ssl_version << std::endl;
//   }
//   std::cout << "libcurl version: " << version_info->version << std::endl;
//
//
//   // Creating a handle for the connection  
//   curl = curl_easy_init();  
//   if (curl) {  
//       // Setting the URL to download  
//       std::string API_KEY;  
//       std::fstream file("apiKey.txt");  
//       if (file.is_open()) {  
//           std::getline(file, API_KEY);  
//           file.close();  
//       }  
//       else {  
//           std::cerr << "Failed to open apiKey.txt" << std::endl;  
//       }  
//       std::cout << "API Key apiKey: " << API_KEY << std::endl;  
//
//       string SAT_ID = "56144";  
//       string LAT = "52.2298";  
//       string LON = "21.0122";  
//       string ALT = "100";  
//       string url = "https://api.n2yo.com/rest/v1/satellite/positions/" + SAT_ID + "/" + LAT + "/" + LON + "/" + ALT + "/1/?apiKey=" + API_KEY;  
//       std::cout << url.c_str() << std::endl;  
//       //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //better curl logs
//       curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
//       curl_easy_setopt(curl, CURLOPT_URL, url.c_str());  
//
//
//       // Set the User-Agent to avoid being blocked by some servers  
//       curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");  
//
//       // Performing the request  
//       res = curl_easy_perform(curl);  
//
//       // Checking for errors  
//       if (res != CURLE_OK) {  
//           std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;  
//       }  
//       else {  
//           std::cout << "\nStarlink data fetched!" << std::endl;  
//       }  
//
//       // Releasing the handle  
//       curl_easy_cleanup(curl);  
//   }  
//
//   // Releasing global resources  
//   curl_global_cleanup();  
//
//   // nlohmann::json test  
//   nlohmann::json j = {  
//       {"pi", 3.141},  
//       {"happy", true},  
//       {"name", "niels"},  
//       {"nothing", nullptr},  
//       {"answer", {  
//           {"everything", 42}  
//       }},  
//       {"list", {1, 0, 2}},  
//       {"object", {  
//           {"currency", "usd"},  
//           {"value", 42.99}  
//       }}  
//   };  
//   std::cout << j["pi"] << std::endl;  
//
//   return 0;  
//}