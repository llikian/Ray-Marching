/***************************************************************************************************
 * @file  Shader.cpp
 * @brief Implementation of the Shader class
 **************************************************************************************************/

#include "Shader.hpp"

#include <glad/glad.h>
#include <fstream>
#include <sstream>

std::string preprocessShader(const std::filesystem::path& path) {
    if(!std::filesystem::exists(path)) {
        throw std::runtime_error("File \"" + path.string() + "\" was not found.");
    }

    const std::string folder = path.parent_path();

    std::ifstream file(path);
    if(!file.is_open()) {
        throw std::runtime_error("Couldn't open \"" + path.string() + "\".");
    }

    std::stringstream output;
    std::string line;

    while(std::getline(file, line)) {
        if(line.contains("#include")) {
            unsigned int first = line.find_first_of('"') + 1;
            unsigned int last = line.find_last_of('"');
            const std::string includePath = folder + '/' + line.substr(first, last - first);

            output << preprocessShader(includePath) << '\n';
        } else {
            output << line << '\n';
        }
    }

    if(path.extension() == ".frag") {
        std::ofstream oFile("temp/fragment_shader.frag");
        oFile << output.str();
    }

    return output.str();
}

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
    int messageLength;

    /**** Vertex Shader ****/
    std::string vertexShaderCode = preprocessShader(vertexShaderPath);
    const char* vertexShader = vertexShaderCode.c_str();
    unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderID, 1, &vertexShader, nullptr);
    glCompileShader(vertexShaderID);

    glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &messageLength);
    if(messageLength > 0) {
        char* message = new char[messageLength];
        glGetShaderInfoLog(vertexShaderID, messageLength, nullptr, message);

        std::string errorMessage = "Failed to compile vertex shader :\n";
        errorMessage += message;

        delete[] message;

        throw std::runtime_error(errorMessage);
    }

    /**** Fragment Shader ****/
    std::string fragmentShaderCode = preprocessShader(fragmentShaderPath);
    const char* fragmentShader = fragmentShaderCode.c_str();
    unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderID, 1, &fragmentShader, nullptr);
    glCompileShader(fragmentShaderID);

    glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &messageLength);
    if(messageLength > 0) {
        char* message = new char[messageLength];
        glGetShaderInfoLog(fragmentShaderID, messageLength, nullptr, message);

        std::string errorMessage = "Failed to compile fragment shader :\n";
        errorMessage += message;

        delete[] message;

        throw std::runtime_error(errorMessage);
    }

    /**** Shader Program ****/
    id = glCreateProgram();
    glAttachShader(id, vertexShaderID);
    glAttachShader(id, fragmentShaderID);
    glLinkProgram(id);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &messageLength);
    if(messageLength > 0) {
        char* message = new char[messageLength];
        glGetProgramInfoLog(id, messageLength, nullptr, message);

        std::string errorMessage = "Failed to link shader program :\n";
        errorMessage += message;

        delete[] message;

        throw std::runtime_error(errorMessage);
    }
}

Shader::~Shader() {
    glDeleteProgram(id);
}

void Shader::use() {
    glUseProgram(id);
}

void Shader::setUniform(const std::string& uniform, int value) const {
    glUniform1i(glGetUniformLocation(id, uniform.c_str()), value);
}

void Shader::setUniform(const std::string& uniform, unsigned int value) const {
    glUniform1ui(glGetUniformLocation(id, uniform.c_str()), value);
}

void Shader::setUniform(const std::string& uniform, bool value) const {
    glUniform1i(glGetUniformLocation(id, uniform.c_str()), static_cast<int>(value));
}

void Shader::setUniform(const std::string& uniform, float value) const {
    glUniform1f(glGetUniformLocation(id, uniform.c_str()), value);
}

void Shader::setUniform(const std::string& uniform, float x, float y) const {
    glUniform2f(glGetUniformLocation(id, uniform.c_str()), x, y);
}

void Shader::setUniform(const std::string& uniform, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(id, uniform.c_str()), x, y, z);
}

void Shader::setUniform(const std::string& uniform, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(id, uniform.c_str()), x, y, z, w);
}

void Shader::setUniform(const std::string& uniform, const vec2& vec) const {
    glUniform2fv(glGetUniformLocation(id, uniform.c_str()), 1, &vec.x);
}

void Shader::setUniform(const std::string& uniform, const vec3& vec) const {
    glUniform3fv(glGetUniformLocation(id, uniform.c_str()), 1, &vec.x);
}

void Shader::setUniform(const std::string& uniform, const vec4& vec) const {
    glUniform4fv(glGetUniformLocation(id, uniform.c_str()), 1, &vec.x);
}

void Shader::setUniform(const std::string& uniform, const Matrix4& matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(id, uniform.c_str()), 1, true, &(matrix[0][0]));
}

void Shader::getUniform(const std::string& uniform, int& value) const {
    glGetUniformiv(id, glGetUniformLocation(id, uniform.c_str()), &value);
}

void Shader::getUniform(const std::string& uniform, unsigned int& value) const {
    glGetUniformuiv(id, glGetUniformLocation(id, uniform.c_str()), &value);
}

void Shader::getUniform(const std::string& uniform, bool& value) const {
    int v;
    glGetUniformiv(id, glGetUniformLocation(id, uniform.c_str()), &v);
    value = v;
}

void Shader::getUniform(const std::string& uniform, float& value) const {
    glGetUniformfv(id, glGetUniformLocation(id, uniform.c_str()), &value);
}

void Shader::getUniform(const std::string& uniform, vec2& vec) const {
    glGetUniformfv(id, glGetUniformLocation(id, uniform.c_str()), &vec.x);
}

void Shader::getUniform(const std::string& uniform, vec3& vec) const {
    glGetUniformfv(id, glGetUniformLocation(id, uniform.c_str()), &vec.x);
}

void Shader::getUniform(const std::string& uniform, vec4& vec) const {
    glGetUniformfv(id, glGetUniformLocation(id, uniform.c_str()), &vec.x);
}

void Shader::getUniform(const std::string& uniform, Matrix4& matrix) const {
    glGetUniformfv(id, glGetUniformLocation(id, uniform.c_str()), &(matrix[0][0]));
}