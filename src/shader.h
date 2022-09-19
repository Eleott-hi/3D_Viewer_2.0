#ifndef SHADER_H
#define SHADER_H

#include <QOpenGLFunctions>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

using namespace std;

struct ShaderProgramSource {
    string VertexSource;
    string FragmentSource;
};

class Shader : protected QOpenGLFunctions {
   private:
    std::string m_Filepath;
    unsigned int m_RendererID;
    unordered_map<string, int> m_UniformLocationCash;

   public:
    Shader(const std::string &filepath);
    ~Shader();

    void Bind();
    void Unbind();

    // set uniforms
    void SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3);
    void SetUniformMat4f(const std::string &name, float *data);
    void SetUniformInt(const std::string &name, int data);
    void SetUniform1f(const std::string &name, float v);

   private:
    unsigned int GetUniformLocation(const std::string &name);
    unsigned int CompileShader(unsigned int type, const string &source);
    unsigned int CreateShader(const string &vertexShader, const string &fragmentShader);
    ShaderProgramSource ParseShader(const string &filepath);
};

#endif  // SHADER_H
