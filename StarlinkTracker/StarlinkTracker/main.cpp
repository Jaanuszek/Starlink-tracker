#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
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

bool showSatelliteWindow = false;

float rotationAngle = 0.0f;
float rotationSpeed = 0.05f;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

float starlinkSpeed = 30.0f;

bool isCountriesBorderVisible = true;

int main() {
    Window mainWindow = Window(800, 600);
    mainWindow.Initialize();

    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm local_time;
    localtime_s(&local_time, &now_c);

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

    Model starlinkModel("assets/Models/starlink/starlink.obj", "shaders/starlinkModelShader.shader");

    JSONParser jsonParser;
    jsonParser.ParseGeoJSON("assets/geoJSON/countriesGeoJSON.json", 0.50f);
    {
        Camera camera = Camera(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

        Sphere sphere(100, 100, 0.5f);
        std::vector<Vertex> SphereVertices = sphere.getVertices();
        std::vector<unsigned int> SphereIndices = sphere.getIndices();
        Mesh SphereMesh(SphereVertices, SphereIndices, ".\\assets\\earthMap.png");

        // Coordinates for drawing borders on the sphere
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
        // Creation of countries border mesh
        Mesh CountriesBorderMesh(countriesBorderVertices);

        glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.GetFrameBufferWidth() / (GLfloat)mainWindow.GetFrameBufferHeight(), 0.1f, 100.f);
        glm::mat4 earthModel = glm::mat4(1.0f);
        earthModel = glm::rotate(earthModel, glm::radians(-180.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // rotate 180 degrees to change coordinates upside down 
        glm::mat4 bordersModel = glm::mat4(1.0f);
        bordersModel = glm::scale(bordersModel, glm::vec3(-1.0f, 1.0f, 1.0f));

        // Creation of Shaders
        Shader shader("shaders/basicShader.shader");
        Shader shaderBorders("shaders/countriesBorderShader.shader");
        Shader starlinkTrajectoryShader("shaders/starlinkTrajectoryShader.shader");

        std::unordered_map<std::string, shaderUniformData> starlinkUniformDataMap =  
        {  
            {"projection", shaderUniformData{"projection", projection}},
            {"model", shaderUniformData{"model", earthModel}},
            {"view", shaderUniformData{"view", camera.GetViewMatrix()}},
        };

        // Vector containing all starlinks
        std::vector<std::unique_ptr<Starlink>> starlinks;
        // Map containing all starlinks (it helps us checking wheter the satellite is already in the map or not)
        std::unordered_map<int, Satellite> SatellitesInfoMap;

        std::vector<Satellite> SatellitesInfoVec;

        HttpServer server(&isCountriesBorderVisible, API_KEY, local_time);
        server.start();

        GLfloat simulationTime = 0.0f;

        // for FPS counter
        double previousTime = 0.0;
        double currentTime = 0.0;
        double timeDiff;
        unsigned int frameCounter = 0;
        float FPS = 0.0f;

        // Counting time for trajectory line
        auto lastTimeChrono = std::chrono::high_resolution_clock::now();
        while (!mainWindow.ShouldClose()) {

            SatellitesInfoVec = server.getSatellitesInfo();
            if (!SatellitesInfoVec.empty()) {
                for (auto& satelliteInfo : SatellitesInfoVec) {
                    // Check if the satellite is already in the map
                    if (SatellitesInfoMap.find(satelliteInfo.satid) == SatellitesInfoMap.end()) {
                        SatellitesInfoMap[satelliteInfo.satid] = satelliteInfo;
                        starlinks.push_back(std::make_unique<Starlink>(satelliteInfo, local_time));
                    }
                }
                server.clearSateliteVector();
                SatellitesInfoVec.clear();
            }

            auto currTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> elapsed = currTime - lastTimeChrono;

            GLfloat now = glfwGetTime();
            deltaTime = now - lastTime;
            lastTime = now;

            // FPS counter, it work simillar to deltaTime, but a little bit different. That's why we need different variables for that
            currentTime = glfwGetTime();
            timeDiff = currentTime - previousTime;
            frameCounter++;
            FPS = mainWindow.CountFPSandMS(previousTime, currentTime, timeDiff, frameCounter);

            // You can speed up the starlinks here
            simulationTime += deltaTime * starlinkSpeed;

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

            if (isCountriesBorderVisible) {
                shaderBorders.useShaderProgram();
                shaderBorders.setUniformMat4fv("projection", projection);
                shaderBorders.setUniformMat4fv("model", bordersModel);
                shaderBorders.setUniformMat4fv("view", camera.GetViewMatrix());

                CountriesBorderMesh.DrawMultipleMeshes(GL_LINE_STRIP, countriesOffsets, countriesCounts, countriesOffsets.size());
            }

            // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
            // Jakbyœ chcia³ usuwaæ starlinki o konrketnym id, to usuñ je z vectora "starlinks"
            // oraz usuñ pozycje z mapy "SatellitesInfoMap"
            // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
            for (auto& starlink : starlinks) {

                // its the same as above (f.e shader.updateUniformMat4fv itp..)
                // but for model it is implemented to work like this
                // at least for now
                starlinkUniformDataMap["projection"] = shaderUniformData{ "projection", projection };
                starlinkUniformDataMap["view"] = shaderUniformData{ "view", camera.GetViewMatrix() };
                starlinkUniformDataMap["model"] = shaderUniformData{ "model", starlink->getModelMatrix() };
                starlinkModel.UpdateShaderUniforms(starlinkUniformDataMap); // it has to be called after uniformDataMap is updated
                // Updating Starlink position
                starlink->UpdatePosition(simulationTime);
                // Drawing Starlink model
                starlinkModel.DrawModel();
                // Drawing trajectory
                starlinkTrajectoryShader.useShaderProgram();
                starlinkTrajectoryShader.setUniformMat4fv("projection", projection);
                starlinkTrajectoryShader.setUniformMat4fv("view", camera.GetViewMatrix());
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
                starlinkTrajectoryShader.setUniformMat4fv("model", model);
                starlinkTrajectoryShader.setUniformVec4f("color", starlink->getTrajectoryLineColor());
                // saving starlink position in vector every 0.1 seconds
                if (elapsed.count() > 0.1f) {
                    starlink->saveStarlinkPositionInVector();
                    lastTimeChrono = currTime;
                }
                starlink->DrawTrajectory();
            }

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
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