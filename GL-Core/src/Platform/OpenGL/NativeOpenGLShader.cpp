#include "NativeOpenGLShader.h"

#include <utility>
/***
static GLenum ShaderTypeFromString(const std::string&type) {
    if (type == "vertex") return GL_VERTEX_SHADER;
    if (type == "geometry") return GL_GEOMETRY_SHADER;
    if (type == "tess_control" || type == "tessellation_control") return GL_TESS_CONTROL_SHADER;
    if (type == "tess_evaluation" || type == "tessellation_evaluation") return GL_TESS_EVALUATION_SHADER;
    if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;
    if (type == "compute") return GL_COMPUTE_SHADER;

    GLCORE_ASSERT(false, "Unknown shader type!");
    return 0;
}

NativeOpenGLShader::NativeOpenGLShader(const std::string&filepath) {
    std::string source = ReadFile(filepath);
}

NativeOpenGLShader::NativeOpenGLShader(std::string name, const std::string&vertexSrc,
                                       const std::string&fragmentSrc): mName(std::move(name)) {
    std::unordered_map<GLenum, std::string> sources;
    sources[GL_VERTEX_SHADER] = vertexSrc;
    sources[GL_FRAGMENT_SHADER] = fragmentSrc;
    Compile(sources);
}

NativeOpenGLShader::~NativeOpenGLShader() {
    glDeleteProgram(mRendererID);
}

void NativeOpenGLShader::Bind() const {
    glUseProgram(mRendererID);
}

void NativeOpenGLShader::Unbind() const {
    glUseProgram(0);
}

std::string NativeOpenGLShader::ReadFile(const std::string&filepath) {
}

std::unordered_map<GLenum, std::string> NativeOpenGLShader::PreProcess(const std::string&source) {
    std::unordered_map<GLenum, std::string> shaderSources;
}

void NativeOpenGLShader::Compile(const std::unordered_map<GLenum, std::string>&shaderSources) {
    GLuint program = glCreateProgram();
}
***/