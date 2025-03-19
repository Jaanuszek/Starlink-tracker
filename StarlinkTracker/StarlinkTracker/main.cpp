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
#include "include/VBO.h"
#include "include/VAO.h"
#include "include/Shader.h"
#include "include/fetchApi.h"
#include "Tle.h"
#include "DateTime.h"
#include "Vector.h"
#include "SGP4.h"
#include <assimp/Importer.hpp>

struct Satellite {
    int satid;
    std::string satname;
    int transactionscount;
    std::string tleLine1;
    std::string tleLine2;
};

std::vector<Satellite> satellites;
bool showSatelliteWindow = false;

float rotationAngle = 0.0f;
float rotationSpeed = 0.01f;

void parseJSONSattelite(const std::string& satData)
{
    try {
        nlohmann::json parsedData = nlohmann::json::parse(satData);

        if (parsedData.contains("info") && parsedData["info"].contains("satid") && parsedData["info"].contains("satname") && parsedData["info"].contains("transactionscount") && parsedData.contains("tle")) {
            const auto& info = parsedData["info"];
            Satellite satellite;
            satellite.satid = info["satid"].get<int>();
            satellite.satname = info["satname"].get<std::string>();
            satellite.transactionscount = info["transactionscount"].get<int>();

            if (parsedData["tle"].is_string()) {
                std::string tle = parsedData["tle"].get<std::string>();

                size_t splitPos = tle.find("\r\n");
                if (splitPos != std::string::npos) {
                    satellite.tleLine1 = tle.substr(0, splitPos);
                    satellite.tleLine2 = tle.substr(splitPos + 2);
                }
				std::cout << satellite.satname << std::endl;
				std::cout << satellite.tleLine1 << std::endl;
				std::cout << satellite.tleLine2 << std::endl;

                libsgp4::SGP4 sgp4(libsgp4::Tle(satellite.satname, satellite.tleLine1, satellite.tleLine2));
                libsgp4::DateTime dt(2025, 3, 17, 20, 0, 0);
                libsgp4::Eci eci = sgp4.FindPosition(dt);
                libsgp4::Vector position = eci.Position();
                libsgp4::Vector velocity = eci.Velocity();

                std::cout << "Position (km): x = " << position.x << ", y = " << position.y << ", z = " << position.z << std::endl;
                std::cout << "Velocity (km/s): x = " << velocity.x << ", y = " << velocity.y << ", z = " << velocity.z << std::endl;

                satellites.push_back(satellite);
            }
            else {
                std::cerr << "TLE field is missing or not a string!" << std::endl;
            }
        }
        else {
            std::cerr << "Required fields are missing in the response!" << std::endl;
        }
    }
    catch (const nlohmann::json::exception& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
    }
}

std::vector<Vertex> ver = {
    {{0.0f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.5f, 1.0f}},
    {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
    {{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}
};

bool isPointInTriangle(float px, float py, glm::mat4 transform) {
    glm::vec4 v1 = transform * glm::vec4(ver[0].position, 1.0f);
    glm::vec4 v2 = transform * glm::vec4(ver[1].position, 1.0f);
    glm::vec4 v3 = transform * glm::vec4(ver[2].position, 1.0f);

    float x1 = v1.x, y1 = v1.y;
    float x2 = v2.x, y2 = v2.y;
    float x3 = v3.x, y3 = v3.y;

    float area = 0.5f * (-y2 * x3 + y1 * (-x2 + x3) + x1 * (y2 - y3) + x2 * y3);
    float sign = area < 0 ? -1.0f : 1.0f;

    float s = sign * (y1 * x3 - x1 * y3 + (y3 - y1) * px + (x1 - x3) * py);
    float t = sign * (x1 * y2 - y1 * x2 + (y1 - y2) * px + (x2 - x1) * py);

    return s > 0 && t > 0 && (s + t) < 2 * sign * area;
}

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
            showSatelliteWindow = true;
        }
    }
}

void renderSatelliteDataImGui() {
    ImGui::Begin("Satellite Data");
    
    if (!satellites.empty()) {
        ImGui::Text("Satellite Information");
        ImGui::Separator();

        if (ImGui::BeginTable("SatelliteDataTable", 5, ImGuiTableFlags_Borders)) {
            ImGui::TableSetupColumn("SatID");
            ImGui::TableSetupColumn("Name");
            ImGui::TableSetupColumn("Transaction Count");
            ImGui::TableSetupColumn("TLE Line 1");
            ImGui::TableSetupColumn("TLE Line 2");

            ImGui::TableHeadersRow();

            for (const auto& sat : satellites) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("%d", sat.satid);
                ImGui::TableNextColumn();
                ImGui::Text("%s", sat.satname.c_str());
                ImGui::TableNextColumn();
                ImGui::Text("%d", sat.transactionscount);
                ImGui::TableNextColumn();
                ImGui::Text("%s", sat.tleLine1.c_str());
                ImGui::TableNextColumn();
                ImGui::Text("%s", sat.tleLine2.c_str());
            }
            ImGui::EndTable();
        }
    }
    else {
        ImGui::Text("No satellite data available.");
    }

    if (ImGui::Button("Close")) {
        showSatelliteWindow = false;
    }

    ImGui::End();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "StarlinkTracker", NULL, NULL);
    if (!window) {
        std::cerr << "[ERROR] creating GLFW!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "[ERROR] initialising GLAD!" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    std::string API_KEY;
    std::fstream file("apiKey.txt");
    if (file.is_open()) {
        std::getline(file, API_KEY);
        file.close();
    }
    else {
        std::cerr << "Failed to open apiKey.txt" << std::endl;
        return -1;
    }

    std::string SAT_ID = "25544";  // For example, ISS (International Space Station)
    std::string url = "https://api.n2yo.com/rest/v1/satellite/tle/" + SAT_ID + "&apiKey=" + API_KEY;
    std::string satData;
    {
		// zrobilem scope zeby sie destruktor wywolal i zamknal curla
        fetchApi satelliteDataAPI;
        satelliteDataAPI.fetchDataFromAPI(url, satData);
    }
	parseJSONSattelite(satData);

    {
		// Bind VAO -> Bind VBO -> Set attrib pointers -> Unbind VBO ->Unbind VAO
        VAO vao;
        vao.bind();
		VBO vbo(ver);
        vbo.unbind();
        vao.unbind();
        // Set shader from a file
		Shader shader("shaders/basicShader.shader");

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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

			// Use shader program (use this specific shader)
			shader.useShaderProgram();
			// Set uniform matrix in Shader
			shader.setUniformMat4fv("transform", transform);

            vao.bind();
            glDrawArrays(GL_TRIANGLES, 0, 3);
            vao.unbind();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (showSatelliteWindow) {
                renderSatelliteDataImGui();
            }
              
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
