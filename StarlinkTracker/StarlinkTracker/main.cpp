#include <iostream>  
#include <fstream>  
#include <string>  
#include <curl/curl.h>  
#include <glad/glad.h>  
#include <GLFW/glfw3.h>  
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "include/Shader.h"
#include "include/fetchApi.h"
#include "include/Mesh.h"
#include "include/Models/Sphere.h"
#include "include/Texture.h"
#include "include/JSONParser.h"
#include "Tle.h"
#include "DateTime.h"
#include <assimp/Importer.hpp>


int width = 800;
int height = 600;

std::vector<Satellite> satellites;
bool showSatelliteWindow = false;

float rotationAngle = 0.0f;
float rotationSpeed = 0.05f;

std::vector<Vertex> ver = {
    {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.5f, 1.0f}},
	{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
    {{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}
};
std::vector<unsigned int> ind = { 0, 1, 2 ,3 , 1, 2 };

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


bool isPointInRectangle(float px, float py, glm::mat4 transform)
{
	glm::vec4 v1 = transform * glm::vec4(ver[0].position, 1.0f);
	glm::vec4 v2 = transform * glm::vec4(ver[1].position, 1.0f);
	glm::vec4 v3 = transform * glm::vec4(ver[2].position, 1.0f);
	glm::vec4 v4 = transform * glm::vec4(ver[3].position, 1.0f);

	float x1 = v1.x, y1 = v1.y;
	float x2 = v2.x, y2 = v2.y;
	float x3 = v3.x, y3 = v3.y;
	float x4 = v4.x, y4 = v4.y;

	float area1 = 0.5f * (-y2 * x3 + y1 * (-x2 + x3) + x1 * (y2 - y3) + x2 * y3);
	float sign1 = area1 < 0 ? -1.0f : 1.0f;

	float s1 = sign1 * (y1 * x3 - x1 * y3 + (y3 - y1) * px + (x1 - x3) * py);
	float t1 = sign1 * (x1 * y2 - y1 * x2 + (y1 - y2) * px + (x2 - x1) * py);

	float area2 = 0.5f * (-y3 * x4 + y1 * (-x3 + x4) + x1 * (y3 - y4) + x3 * y4);
	float sign2 = area2 < 0 ? -1.0f : 1.0f;

	float s2 = sign2 * (y1 * x4 - x1 * y4 + (y4 - y1) * px + (x1 - x4) * py);
	float t2 = sign2 * (x1 * y3 - y1 * x3 + (y1 - y3) * px + (x3 - x1) * py);

	return (s1 > 0 && t1 > 0 && (s1 + t1) < 2 * sign1 * area1) || (s2 > 0 && t2 > 0 && (s2 + t2) < 2 * sign2 * area2);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        int width, height;
        glfwGetWindowSize(window, &width, &height);

        float x = (xpos / width) * 2.0f - 1.0f;
        float y = 1.0f - (ypos / height) * 2.0f;

        glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(-rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));

        if (isPointInRectangle(x, y, transform)) {
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

    GLFWwindow* window = glfwCreateWindow(width, height, "StarlinkTracker", NULL, NULL);
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
    glfwSwapInterval(1);
	glEnable(GL_DEPTH_TEST);
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

    std::string SAT_ID = "63329";  // For example, ISS (International Space Station)
    std::string url = "https://api.n2yo.com/rest/v1/satellite/tle/" + SAT_ID + "&apiKey=" + API_KEY;
    std::string satData;
    {
		// zrobilem scope zeby sie destruktor wywolal i zamknal curla
        fetchApi satelliteDataAPI;
        satelliteDataAPI.fetchDataFromAPI(url, satData);
    }
	JSONParser jsonParser;
	JSONParser::ParseJSONSattelite(satData, satellites);
	jsonParser.ParseGeoJSON("assets/geoJSON/countriesGeoJSON.json");
    {
        Mesh mesh(ver, ind, ".\\assets\\earthMap.png");
        Sphere sphere(100, 100, 0.5f);
		std::vector<Vertex> SphereVertices = sphere.getVertices();
		std::vector<unsigned int> SphereIndices = sphere.getIndices();
        Mesh SphereMesh(SphereVertices, SphereIndices, ".\\assets\\earthMap.png");


        // Drawing countries on map
		std::map<Country, PrimitiveData> countriesMap = jsonParser.getCountries();

		std::vector<VertexPosOnly> countriesBorderVertices;

        for (auto& [country, primitiveData] : countriesMap) {
            for (const auto& polygon : primitiveData.polygons) {
                countriesBorderVertices.insert(countriesBorderVertices.end(), polygon.second.begin(), polygon.second.end());
            }
        }

		Mesh CountriesBorderMesh(countriesBorderVertices);

        // Set shader from a file
		Shader shader("shaders/basicShader.shader");
		Shader shaderBorders("shaders/countriesBorderShader.shader");

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // Init ImGUI
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        while (!glfwWindowShouldClose(window)) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
            rotationAngle += rotationSpeed;
            glm::mat4 bordersProjection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
            glm::mat4 bordersModel = glm::mat4(1.0f);
            glm::mat4 bordersView = glm::mat4(1.0f);
            bordersView = glm::translate(bordersView, glm::vec3(0.0f, 0.0f, -3.0f));
            bordersView = glm::rotate(bordersView, glm::radians(-rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

			//Use shader program (use this specific shader)
			shader.useShaderProgram();
			// Set uniform matrix in Shader
            shader.setUniformMat4fv("projection", bordersProjection);
            shader.setUniformMat4fv("model", bordersModel);
            shader.setUniformMat4fv("view", bordersView);
			shader.setUniform1i("ourTexture", 0);
            SphereMesh.Draw(GL_TRIANGLES);

            glClear(GL_DEPTH_BUFFER_BIT);
			shaderBorders.useShaderProgram();
			shaderBorders.setUniformMat4fv("projection", bordersProjection);
			shaderBorders.setUniformMat4fv("model", bordersModel);
			shaderBorders.setUniformMat4fv("view", bordersView);
			shaderBorders.setUniform1i("ourTexture", 0);

			CountriesBorderMesh.DrawWithoutEBO(GL_POINTS, countriesBorderVertices.size());

            //Jak chcesz wrocic do tego trójk¹ta/ prostok¹ta, to zakomentuj wy¿sz¹ linijke i odkomunetuj to na dole
			//mesh.Draw();

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
