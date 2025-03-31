#include <iostream>
#include <fstream>
#include <string>
#include <curl/curl.h>
#include <json.hpp>
#include "include/Window.h"
#include "include/Camera.h"
#include "include/Shader.h"
#include "include/fetchApi.h"
#include "include/Mesh.h"
#include "include/Models/Sphere.h"
#include "include/Models/Starlink.h"
#include "include/JSONParser.h"
#include "include/Texture.h"
#include "include/HttpServer.h"
#include "include/Model.h"

int width = 800;
int height = 600;

std::vector<Satellite> satellites;
bool showSatelliteWindow = false;

float rotationAngle = 0.0f;
float rotationSpeed = 0.05f;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

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

int main() {
    Window mainWindow = Window(800, 600);
    mainWindow.Initialize();

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

    std::vector<std::string> satIDs = { "63329", "63307", "62966", "61262" };
    std::map<std::string, std::string> satelitesData;
    {
        fetchApi satelliteDataAPI;
        for (const auto& SAT_ID : satIDs) {
            std::string url = "https://api.n2yo.com/rest/v1/satellite/tle/" + SAT_ID + "&apiKey=" + API_KEY;
            std::string satData;
            satelliteDataAPI.fetchDataFromAPI(url, satData);
            satelitesData[SAT_ID] = satData;
        }
    }

    Model starlinkModel("assets/Models/starlink/starlink.obj");

    JSONParser jsonParser;
    for (auto& [satID, satData] : satelitesData) {
        JSONParser::ParseJSONSattelite(satData, satellites);
    }
    jsonParser.ParseGeoJSON("assets/geoJSON/countriesGeoJSON.json", 0.51f);
    {
        Camera camera = Camera(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

        Sphere sphere(100, 100, 0.5f);
        std::vector<Vertex> SphereVertices = sphere.getVertices();
        std::vector<unsigned int> SphereIndices = sphere.getIndices();
        Mesh SphereMesh(SphereVertices, SphereIndices, ".\\assets\\earthMap.png");

        std::vector<Starlink> starlinks;
        for (auto& sat : satellites) {
            starlinks.push_back(Starlink(sat));
        }

        // Drawing countries on map
        std::map<Country, PrimitiveData> countriesMap = jsonParser.getCountries();

        std::vector<int> countriesOffsets; // offset for each country
        std::vector<int> countriesCounts; // count of vertices for each country
        std::vector<VertexPosOnly> countriesBorderVertices;

        for (auto& [country, primitiveData] : countriesMap) {
            for (const auto& [polygonIndex, polygonVertices] : primitiveData.polygons) {
                countriesBorderVertices.insert(countriesBorderVertices.end(), polygonVertices.begin(), polygonVertices.end());
                countriesOffsets.push_back(countriesBorderVertices.size() - polygonVertices.size());
                countriesCounts.push_back(polygonVertices.size());
            }
        }
        Mesh CountriesBorderMesh(countriesBorderVertices);

        // Set shader from a file
        Shader shader("shaders/basicShader.shader");
        Shader shaderBorders("shaders/countriesBorderShader.shader");
        Shader starlinkShader("shaders/countriesBorderShader.shader"); //temporary shader

        glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.GetFrameBufferWidth() / (GLfloat)mainWindow.GetFrameBufferHeight(), 0.1f, 100.f);
        glm::mat4 earthModel = glm::mat4(1.0f);
        earthModel = glm::rotate(earthModel, glm::radians(-180.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // rotate 180 degrees to change coordinates upside down 
        glm::mat4 bordersModel = glm::mat4(1.0f);
        bordersModel = glm::scale(bordersModel, glm::vec3(-1.0f, 1.0f, 1.0f));

        HttpServer server;
        server.start();

        GLfloat simulationTime = 0.0f;

        while (!mainWindow.ShouldClose()) {

            GLfloat now = glfwGetTime();
            deltaTime = now - lastTime;
            lastTime = now;

            simulationTime += deltaTime * 180.0f; // 30.0f is the speed of the simulation

            camera.ProcessKeyboardInput(deltaTime);
            camera.ProcessMouseInput(mainWindow.GetMouseXDelta(), mainWindow.GetMouseYDelta());

            mainWindow.ToggleCursorVisibility();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            rotationAngle += rotationSpeed;
            glm::mat4 View = glm::mat4(1.0f);
            View = glm::translate(View, glm::vec3(0.0f, 0.0f, -3.0f));
            View = glm::rotate(View, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
            
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

            //Use shader program (use this specific shader)
            shader.useShaderProgram();
            // Set uniform matrix in Shader
            shader.setUniformMat4fv("projection", projection);
            shader.setUniformMat4fv("model", earthModel);
            shader.setUniformMat4fv("view", camera.GetViewMatrix());
            shader.setUniform1i("ourTexture", 0);
            SphereMesh.Draw(GL_TRIANGLES);

            shaderBorders.useShaderProgram();
            shaderBorders.setUniformMat4fv("projection", projection);
            shaderBorders.setUniformMat4fv("model", bordersModel);
            shaderBorders.setUniformMat4fv("view", camera.GetViewMatrix());
            shaderBorders.setUniform1i("ourTexture", 0);

            CountriesBorderMesh.DrawMultipleMeshes(GL_LINE_STRIP, countriesOffsets, countriesCounts, countriesOffsets.size());

            starlinkShader.useShaderProgram();
            starlinkShader.setUniformMat4fv("projection", projection);
            starlinkShader.setUniformMat4fv("view", camera.GetViewMatrix());
            for (auto& starlink : starlinks) {
                starlink.UpdatePosition(simulationTime);
                starlinkShader.setUniformMat4fv("model", starlink.getModelMatrix());
                starlinkModel.DrawModel();
            }

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (showSatelliteWindow) {
                renderSatelliteDataImGui();
            }
              
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            mainWindow.SwapFrameBuffers();
            glfwPollEvents();
        }

        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        server.stop();
    }

    return 0;
}
