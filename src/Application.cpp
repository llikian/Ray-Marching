/***************************************************************************************************
 * @file  Application.cpp
 * @brief Implementation of the Application class
 **************************************************************************************************/

#include "Application.hpp"

#include <cmath>

#include "callbacks.hpp"
#include "maths/geometry.hpp"
#include "maths/trigonometry.hpp"
#include "mesh/Mesh.hpp"
#include "mesh/meshes.hpp"

Application::Application()
    : window(nullptr), width(900), height(900),
      time(0.0f), delta(0.0f),
      cursorVisible(false),
      shader(nullptr), raymarching(nullptr),
      projection(perspective(M_PI_4f, static_cast<float>(width) / height, 0.1f, 100.0f)),
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
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glActiveTexture(GL_TEXTURE0);

    // Sets the default diffuse and specular maps to a plain white color
    const unsigned char white[3]{255, 255, 255};
    glBindTexture(GL_TEXTURE_2D, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, white);

    /**** Lights ****/
    // Directional Light
    directionalLight.direction = normalize(vec3(-2.0f, -3.0f, -2.0f));
    directionalLight.ambient = vec3(0.1f);
    directionalLight.diffuse = vec3(1.0f);
    directionalLight.specular = vec3(1.0f);

    // Flashlight
    flashlight.cutOff = std::cos(radians(12.5f));
    flashlight.outerCutOff = std::cos(radians(15.0f));

    flashlight.constant = 1.0f;
    flashlight.linear = 0.09f;
    flashlight.quadratic = 0.032f;

    flashlight.ambient = vec3(0.0f);
    flashlight.diffuse = vec3(1.0f);
    flashlight.specular = vec3(1.0f);

    // Point Lights
    vec3 pointLightPositions[]{
        vec3(5.0f, 1.0f, 5.0f),
        vec3(5.0f, 1.0f, -5.0f),
        vec3(-5.0f, 1.0f, 5.0f),
        vec3(-5.0f, 1.0f, -5.0f)
    };

    vec3 pointLightColors[]{
        vec3(1.0f, 0.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 0.0f, 1.0f),
        vec3(1.0f, 0.0f, 1.0f)
    };

    pointLights.resize(4);
    for(unsigned int i = 0 ; i < pointLights.size() ; ++i) {
        pointLights[i].position = pointLightPositions[i];

        pointLights[i].constant = 1.0f;
        pointLights[i].linear = 0.09f;
        pointLights[i].quadratic = 0.032f;

        pointLights[i].ambient = vec3(0.25f);
        pointLights[i].diffuse = pointLightColors[i];
        pointLights[i].specular = vec3(1.0f);
    }

    /**** Shaders & Uniforms ****/
    shader = new Shader("shaders/default/default.vert", "shaders/default/default.frag");
    raymarching = new Shader("shaders/raymarching/default.vert", "shaders/raymarching/default.frag");
    initUniforms();
}

Application::~Application() {
    delete shader;
    delete raymarching;

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::run() {
    Mesh screen = Meshes::screen();
    Mesh sphere = Meshes::sphere(16, 32);

    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    GLuint textureColorBuffer;
    glGenTextures(1, &textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

    GLuint rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Framebuffer is not complete.");
    }

    /**** Main Loop ****/
    while(!glfwWindowShouldClose(window)) {
        handleEvents();

        delta = glfwGetTime() - time;
        time = glfwGetTime();

        /**** Traditional 3D ****/
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->use();
        shader->setUniform("cameraPos", camera.getPosition());
        shader->setUniform("spotlight.position", camera.getPosition());
        shader->setUniform("spotlight.direction", camera.getDirection());

        calculateMVP(translate(0.0f, 1.75f, 0.0f) * scale(0.75f));
        sphere.draw();

        /**** Raymarching ****/
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        raymarching->use();
        raymarching->setUniform("time", time);
        raymarching->setUniform("cameraPos", camera.getPosition());
        raymarching->setUniform("cameraFront", camera.getDirection());
        raymarching->setUniform("cameraRight", camera.getRight());
        raymarching->setUniform("cameraUp", camera.getUp());

        glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
        screen.draw();

        glfwSwapBuffers(window);
    }
}

void Application::setWindowSize(int width, int height) {
    this->width = width;
    this->height = height;

    shader->use();
    shader->setUniform("resolution", width, height);

    raymarching->use();
    raymarching->setUniform("resolution", width, height);
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
                    Shader* temp = raymarching;

                    try {
                        initUniforms();
                        delete temp;
                    } catch(const std::exception& exception) {
                        std::cerr << "ERROR : " << exception.what() << '\n';
                        raymarching = temp;
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
                    raymarching->setUniform("active_scene", ++scene);

                    keys[key.first] = false;
                    break;
                case GLFW_KEY_DOWN:
                    if(scene > 0) {
                        raymarching->setUniform("active_scene", --scene);
                    }

                    keys[key.first] = false;
                    break;
                case GLFW_KEY_L:
                    hasLighting = !hasLighting;
                    raymarching->setUniform("hasLighting", hasLighting);

                    keys[key.first] = false;
                    break;
                default:
                    break;
            }
        }
    }
}

void Application::calculateMVP(const Matrix4& model) {
    shader->setUniform("mvp", std::move(camera.getVPmatrix(projection) * model));
    shader->setUniform("model", model);
}

void Application::initUniforms() {
    /**** Default Shader ****/
    shader->use();

    shader->setUniform("material.diffuse", 0);
    shader->setUniform("material.specular", 1);
    shader->setUniform("material.shininess", 32.0f);

    shader->setUniform("directionalLight.direction", directionalLight.direction);
    shader->setUniform("directionalLight.ambient", directionalLight.ambient);
    shader->setUniform("directionalLight.diffuse", directionalLight.diffuse);
    shader->setUniform("directionalLight.specular", directionalLight.specular);

    shader->setUniform("spotlight.cutOff", flashlight.cutOff);
    shader->setUniform("spotlight.outerCutOff", flashlight.outerCutOff);
    shader->setUniform("spotlight.constant", flashlight.constant);
    shader->setUniform("spotlight.linear", flashlight.linear);
    shader->setUniform("spotlight.quadratic", flashlight.quadratic);
    shader->setUniform("spotlight.ambient", flashlight.ambient);
    shader->setUniform("spotlight.diffuse", flashlight.diffuse);
    shader->setUniform("spotlight.specular", flashlight.specular);

    std::string nameStart("pointLights["), name;
    for(unsigned int i = 0 ; i < pointLights.size() ; ++i) {
        name = nameStart + std::to_string(i) + "].";

        shader->setUniform(name + "position", pointLights[i].position);
        shader->setUniform(name + "constant", pointLights[i].constant);
        shader->setUniform(name + "linear", pointLights[i].linear);
        shader->setUniform(name + "quadratic", pointLights[i].quadratic);
        shader->setUniform(name + "ambient", pointLights[i].ambient);
        shader->setUniform(name + "diffuse", pointLights[i].diffuse);
        shader->setUniform(name + "specular", pointLights[i].specular);
    }

    calculateMVP(Matrix4(1.0f));

    /**** Raymarching Shader ****/
    raymarching->use();

    raymarching->setUniform("resolution", width, height);
    raymarching->setUniform("mouse", 0.5f, 0.5f);
    raymarching->setUniform("time", 0.0f);
    raymarching->setUniform("active_scene", scene);
    raymarching->setUniform("hasLighting", hasLighting);
}
