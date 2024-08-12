/***************************************************************************************************
 * @file  Shader.hpp
 * @brief Declaration of the Shader class
 **************************************************************************************************/

#pragma once

#include <filesystem>
#include <string>

#include "maths/vec2.hpp"
#include "maths/vec3.hpp"
#include "maths/vec4.hpp"
#include "maths/Matrix4.hpp"

/**
 * @brief Preprocesses a shader. This actually reads '#include' directives in the glsl code and
 * appends the corresponding files in place of the directives.
 * @param path The shader's path.
 * @return The preprocessed source code.
 */
std::string preprocessShader(const std::filesystem::path& path);

/**
 * @class Shader
 * @brief Compiles, links and creates a shader program that can be then be bound. Can set the value
 * of uniforms in the shader program.
 */
class Shader {
public:
    /**
     * @brief Compiles then links and creates the shader program from the vertex and fragment
     * shaders located at the given paths.
     * @param vertexShaderPath The path to the vertex shader.
     * @param fragmentShaderPath The path to the fragment shader.
     */
    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

    /**
     * @brief Deletes the shader program.
     */
    ~Shader();

    /**
     * @brief Uses the shader program.
     */
    void use();

    /**
     * @brief Sets the value of a uniform of type int.
     * @param uniform The uniform's name.
     * @param value The new value of the uniform.
     */
    void setUniform(const std::string& uniform, int value) const;

    /**
     * @brief Sets the value of a uniform of type unsigned int.
     * @param uniform The uniform's name.
     * @param value The new value of the uniform.
     */
    void setUniform(const std::string& uniform, unsigned int value) const;

    /**
     * @brief Sets the value of a uniform of type bool.
     * @param uniform The uniform's name.
     * @param value The new value of the uniform.
     */
    void setUniform(const std::string& uniform, bool value) const;

    /**
     * @brief Sets the value of a uniform of type float.
     * @param uniform The uniform's name.
     * @param value The new value of the uniform.
     */
    void setUniform(const std::string& uniform, float value) const;

    /**
     * @brief Sets the value of a uniform of type vec2.
     * @param uniform The uniform's name.
     * @param x The new x value of the uniform.
     * @param y The new y value of the uniform.
     */
    void setUniform(const std::string& uniform, float x, float y) const;

    /**
     * @brief Sets the value of a uniform of type vec3.
     * @param uniform The uniform's name.
     * @param x The new x value of the uniform.
     * @param y The new y value of the uniform.
     * @param z The new z value of the uniform.
     */
    void setUniform(const std::string& uniform, float x, float y, float z) const;

    /**
     * @brief Sets the value of a uniform of type vec4.
     * @param uniform The uniform's name.
     * @param x The new x value of the uniform.
     * @param y The new y value of the uniform.
     * @param z The new z value of the uniform.
     * @param w The new w value of the uniform.
     */
    void setUniform(const std::string& uniform, float x, float y, float z, float w) const;

    /**
     * @brief Sets the value of a uniform of type vec2.
     * @param uniform The uniform's name.
     * @param vec The new value of the uniform.
     */
    void setUniform(const std::string& uniform, const vec2& vec) const;

    /**
     * @brief Sets the value of a uniform of type vec3.
     * @param uniform The uniform's name.
     * @param vec The new value of the uniform.
     */
    void setUniform(const std::string& uniform, const vec3& vec) const;

    /**
     * @brief Sets the value of a uniform of type vec4.
     * @param uniform The uniform's name.
     * @param vec The new value of the uniform.
     */
    void setUniform(const std::string& uniform, const vec4& vec) const;

    /**
     * @brief Sets the value of a uniform of type mat4.
     * @param uniform The uniform's name.
     * @param matrix The new value of the uniform.
     */
    void setUniform(const std::string& uniform, const Matrix4& matrix) const;

    /**
     * @brief Gets the value of a uniform of type int.
     * @param uniform The uniform's name.
     * @param value The value of the uniform.
     */
    void getUniform(const std::string& uniform, int& value) const;

    /**
     * @brief Gets the value of a uniform of type unsigned int.
     * @param uniform The uniform's name.
     * @param value The value of the uniform.
     */
    void getUniform(const std::string& uniform, unsigned int& value) const;

    /**
     * @brief Gets the value of a uniform of type bool.
     * @param uniform The uniform's name.
     * @param value The value of the uniform.
     */
    void getUniform(const std::string& uniform, bool& value) const;

    /**
     * @brief Gets the value of a uniform of type float.
     * @param uniform The uniform's name.
     * @param value The value of the uniform.
     */
    void getUniform(const std::string& uniform, float& value) const;

    /**
     * @brief Gets the value of a uniform of type vec2.
     * @param uniform The uniform's name.
     * @param vec The value of the uniform.
     */
    void getUniform(const std::string& uniform, vec2& vec) const;

    /**
     * @brief Gets the value of a uniform of type vec3.
     * @param uniform The uniform's name.
     * @param vec The value of the uniform.
     */
    void getUniform(const std::string& uniform, vec3& vec) const;

    /**
     * @brief Gets the value of a uniform of type vec4.
     * @param uniform The uniform's name.
     * @param vec The value of the uniform.
     */
    void getUniform(const std::string& uniform, vec4& vec) const;

    /**
     * @brief Gets the value of a uniform of type mat4.
     * @param uniform The uniform's name.
     * @param matrix The value of the uniform.
     */
    void getUniform(const std::string& uniform, Matrix4& matrix) const;

private:
    unsigned int id; ///< The shader program's id.
};