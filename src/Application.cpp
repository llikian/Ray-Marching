/***************************************************************************************************
 * @file  Application.cpp
 * @brief Implementation of the Application class
 **************************************************************************************************/

#include "Application.hpp"

#include <cmath>

#include "callbacks.hpp"
#include "maths/geometry.hpp"

Application::Application()
    : window(nullptr), width(900), height(900),
      time(0.0f), delta(0.0f),
      cursorVisible(false),
      shader(nullptr),
      camera(Point(0.0f, 2.0f, 5.0f)),
      scene(0), hasLighting(true) {

    /**** GLFW ****/
    if(!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Raymarching", nullptr, nullptr);
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

    /**** Shader ****/
    initShader();
}

Application::~Application() {
    delete shader;

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::run() {
    float vertices[] {
      -1.0f, 1.0f,
      -1.0f, -1.0f,
      1.0f, -1.0f,
      1.0f, 1.0f
    };

    unsigned int indices[] {
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

        shader->use();
        shader->setUniform("time", time);
        shader->setUniform("cameraPos", camera.getPosition());
        shader->setUniform("cameraFront", camera.getDirection());
        shader->setUniform("cameraRight", camera.getRight());
        shader->setUniform("cameraUp", camera.getUp());

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

    shader->setUniform("resolution", width, height);
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
                case GLFW_KEY_R: {
                    Shader* temp = shader;

                    try {
                        initShader();
                        delete temp;
                    } catch(const std::exception& exception) {
                        std::cerr << "ERROR : " << exception.what() << '\n';
                        shader = temp;
                    }

                    keys[key.first] = false;
                    break;
                }
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
                    shader->setUniform("active_scene", ++scene);

                    keys[key.first] = false;
                    break;
                case GLFW_KEY_DOWN:
                    if(scene > 0) {
                        shader->setUniform("active_scene", --scene);
                    }

                    keys[key.first] = false;
                    break;
                case GLFW_KEY_L:
                    hasLighting = !hasLighting;
                    shader->setUniform("hasLighting", hasLighting);

                    keys[key.first] = false;
                    break;
                default:
                    break;
            }
        }
    }
}

void Application::initShader() {
    shader = new Shader("data/shaders/default.vert", "data/shaders/default.frag");
    shader->use();
    shader->setUniform("resolution", width, height);
    shader->setUniform("mouse", 0.5f, 0.5f);
    shader->setUniform("time", 0.0f);
    shader->setUniform("active_scene", scene);
    shader->setUniform("hasLighting", hasLighting);
}
