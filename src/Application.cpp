/***************************************************************************************************
 * @file  Application.cpp
 * @brief Implementation of the Application class
 **************************************************************************************************/

#include "Application.hpp"

#include <cmath>
#include <filesystem>

#include "callbacks.hpp"
#include "maths/geometry.hpp"

namespace filesystem = std::filesystem;

Application::Application()
    : window(nullptr), width(900), height(900),
      time(0.0f), delta(0.0f),
      cursorVisible(false),
      camera(Point(0.0f, 2.0f, 5.0f)),
      scene(0), hasLighting(true) {

    /**** GLFW ****/
    if(!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Ray-Marching", nullptr, nullptr);
    if(!window) {
        throw std::runtime_error("Failed to create window.");
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    mousePos.x = width / 2.0f;
    mousePos.y = height / 2.0f;

    /**** GLFW Callbacks ****/
    glfwSetWindowSizeCallback(window, windowSizeCallback);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);

    /**** GLAD ****/
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD.");
    }

    /**** OpenGL ****/
    glViewport(0, 0, width, height);

    /**** Shaders ****/
    initShaders();
}

Application::~Application() {
    for(unsigned int i = 0 ; i < SHADER_MAP_FUNCTIONS ; ++i) {
        delete shaders[i];
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::run() {
    float vertices[]{
        -1.0f, 1.0f,
        -1.0f, -1.0f,
        1.0f, -1.0f,
        1.0f, 1.0f
    };

    unsigned int indices[]{
        0, 1, 2,
        0, 2, 3
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /**** Main Loop ****/
    while(!glfwWindowShouldClose(window)) {
        handleEvents();

        delta = glfwGetTime() - time;
        time = glfwGetTime();

        shaders[scene]->use();
        shaders[scene]->setUniform("time", time);
        shaders[scene]->setUniform("cameraPos", camera.getPosition());
        shaders[scene]->setUniform("cameraFront", camera.getDirection());
        shaders[scene]->setUniform("cameraRight", camera.getRight());
        shaders[scene]->setUniform("cameraUp", camera.getUp());

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Application::setWindowSize(int width, int height) {
    this->width = width;
    this->height = height;

    shaders[scene]->setUniform("resolution", width, height);
}

void Application::handleKeyCallback(int key, int action, int /* mods */) {
    if(action == GLFW_PRESS) {
        keys[key] = true;
    } else if(action == GLFW_RELEASE) {
        keys[key] = false;
    }
}

void Application::handleCursorPositionEvent(float xPos, float yPos) {
    if(!cursorVisible) {
        camera.look(vec2(xPos - mousePos.x, yPos - mousePos.y));
    }

    mousePos.x = xPos;
    mousePos.y = yPos;
}

void Application::handleEvents() {
    glfwPollEvents();
    handleKeyboardEvents();
}

void Application::handleKeyboardEvents() {
    for(const std::pair<int, bool> key: keys) {
        if(key.second) {
            switch(key.first) {
                case GLFW_KEY_ESCAPE:
                    glfwSetWindowShouldClose(window, true);
                    break;
                case GLFW_KEY_F5:
                    glfwSetInputMode(window, GLFW_CURSOR,
                                     cursorVisible ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
                    cursorVisible = !cursorVisible;

                    keys[key.first] = false;
                    break;
                case GLFW_KEY_W:
                    camera.move(CameraControls::forward, delta);
                    break;
                case GLFW_KEY_S:
                    camera.move(CameraControls::backward, delta);
                    break;
                case GLFW_KEY_A:
                    camera.move(CameraControls::left, delta);
                    break;
                case GLFW_KEY_D:
                    camera.move(CameraControls::right, delta);
                    break;
                case GLFW_KEY_SPACE:
                    camera.move(CameraControls::upward, delta);
                    break;
                case GLFW_KEY_LEFT_SHIFT:
                    camera.move(CameraControls::downward, delta);
                    break;
                case GLFW_KEY_UP:
                    scene = (scene + 1) % SHADER_MAP_FUNCTIONS;

                    keys[key.first] = false;
                    break;
                case GLFW_KEY_DOWN:
                    scene = (scene + SHADER_MAP_FUNCTIONS - 1) % SHADER_MAP_FUNCTIONS;

                    keys[key.first] = false;
                    break;
                case GLFW_KEY_L:
                    hasLighting = !hasLighting;
                    shaders[scene]->setUniform("hasLighting", hasLighting);

                    keys[key.first] = false;
                    break;
                default:
                    break;
            }
        }
    }
}

void Application::initShaders() {
    unsigned int shaderIDs[3]{
        Shader::compileShader("shaders/default.vert"),
        Shader::compileShader("shaders/default.frag"),
        0
    };

    std::string path("shaders/maps/map");

    for(unsigned int i = 0 ; i < SHADER_MAP_FUNCTIONS ; ++i) {
        shaderIDs[2] = Shader::compileShader(path + std::to_string(i) + ".frag");
        
        shaders[i] = new Shader(shaderIDs, 3, "Ray-Marching (map" + std::to_string(i) + ')');
        shaders[i]->use();
        shaders[i]->setUniform("resolution", static_cast<float>(width), static_cast<float>(height));
        shaders[i]->setUniform("hasLighting", hasLighting);

        switch(i) {
            case 0:
            case 1:
            case 3:
            case 5:
                shaders[i]->setUniform("hasShadows", false);
                break;
            default:
                shaders[i]->setUniform("hasShadows", true);
                break;
        }

        glDeleteShader(shaderIDs[2]);
    }

    glDeleteShader(shaderIDs[0]);
    glDeleteShader(shaderIDs[1]);
}
