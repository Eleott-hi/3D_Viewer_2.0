#include "shader.h"

Shader::Shader(const std::string &filepath) : m_Filepath(filepath), m_RendererID(0) {
    initializeOpenGLFunctions();
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() {
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() {
    glUseProgram(m_RendererID);
}

void Shader::Unbind() {
    glUseProgram(0);
}

void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3) {
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniform1f(const std::string &name, float v) {
    glUniform1f(GetUniformLocation(name), v);
}

void Shader::SetUniformMat4f(const std::string &name, float *data) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, data);
}

void Shader::SetUniformInt(const std::string &name, int data) {
    glUniform1i(GetUniformLocation(name), data);
}

unsigned int Shader::GetUniformLocation(const std::string &name) {
    //    if (m_UniformLocationCash.find(name) == m_UniformLocationCash.end())
    //    {
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    Q_ASSERT(location != -1);
    m_UniformLocationCash[name] = location;
    //    }
    //    return m_UniformLocationCash[name];
    return location;
}

unsigned int Shader::CompileShader(unsigned int type, const string &source) {
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        qDebug() << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                 << "shader!" << Qt::endl;
        qDebug() << message << Qt::endl;
        glDeleteShader(id);
        id = 0;
    }

    return id;
}

ShaderProgramSource Shader::ParseShader(const string &filepath) {
    ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    string line;
    stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    if (!stream)
        cout << "No such file or directory: " << filepath << endl;
    while (getline(stream, line)) {
        if (line.find("#shader") != string::npos) {
            if (line.find("vertex") != string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != string::npos) {
                type = ShaderType::FRAGMENT;
            }
        } else {
            ss[(int)type] << line << '\n';
        }
    }

    return {ss[0].str(), ss[1].str()};
}

unsigned int Shader::CreateShader(const string &vertexShader, const string &fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    int result;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(program, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *)alloca(length * sizeof(char));
        glGetShaderInfoLog(program, length, &length, message);
        qDebug() << "Failed to link shader!" << Qt::endl;
        qDebug() << message << Qt::endl;
        glDeleteShader(program);
        program = 0;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}
