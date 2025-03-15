#include <iostream>  
#include <fstream>  
#include <string>  
#include <curl/curl.h>  
#include <glad/glad.h>  
#include <GLFW/glfw3.h>  
#include <json.hpp>  
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

using std::string;

// Wierzcho³ki trójk¹ta (wspó³rzêdne OpenGL, przed transformacj¹)
const glm::vec3 triangleVertices[3] = {
    { 0.0f,  0.5f, 0.0f},  // Górny wierzcho³ek
    {-0.5f, -0.5f, 0.0f},  // Lewy dolny
    { 0.5f, -0.5f, 0.0f}   // Prawy dolny
};

// Globalna zmienna do œledzenia k¹ta obrotu
float rotationAngle = 0.0f;
float rotationSpeed = 0.01f;

// Funkcja sprawdzaj¹ca, czy punkt (px, py) znajduje siê w trójk¹cie (po transformacji)
bool isPointInTriangle(float px, float py, glm::mat4 transform) {
    glm::vec4 v1 = transform * glm::vec4(triangleVertices[0], 1.0f);
    glm::vec4 v2 = transform * glm::vec4(triangleVertices[1], 1.0f);
    glm::vec4 v3 = transform * glm::vec4(triangleVertices[2], 1.0f);

    float x1 = v1.x, y1 = v1.y;
    float x2 = v2.x, y2 = v2.y;
    float x3 = v3.x, y3 = v3.y;

    float A = 0.5f * (-y2 * x3 + y1 * (-x2 + x3) + x1 * (y2 - y3) + x2 * y3);
    float sign = A < 0 ? -1.0f : 1.0f;

    float s = sign * (y1 * x3 - x1 * y3 + (y3 - y1) * px + (x1 - x3) * py);
    float t = sign * (x1 * y2 - y1 * x2 + (y1 - y2) * px + (x2 - x1) * py);

    return s > 0 && t > 0 && (s + t) < 2 * sign * A;
}

// Funkcja obs³ugi klikniêcia myszk¹
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        int width, height;
        glfwGetWindowSize(window, &width, &height);

        float x = (xpos / width) * 2.0f - 1.0f;
        float y = 1.0f - (ypos / height) * 2.0f;

        glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));

        if (isPointInTriangle(x, y, transform)) {
            std::cout << "Klikniêto w trójk¹t! Wspó³rzêdne OpenGL: (" << x << ", " << y << ")\n";
        }
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Kliknij w trojkat", NULL, NULL);
    if (!window) {
        std::cerr << "B³¹d tworzenia okna GLFW!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "B³¹d inicjalizacji GLAD!" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    float vertices[] = {
         0.0f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };

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

    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        uniform mat4 transform;
        void main() {
            gl_Position = transform * vec4(aPos, 1.0);
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

    int transformLoc = glGetUniformLocation(shaderProgram, "transform");

    // Init ImGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    while (!glfwWindowShouldClose(window)) {
        rotationAngle += rotationSpeed;
        glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("My Window");
        ImGui::Text("Hello");
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
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