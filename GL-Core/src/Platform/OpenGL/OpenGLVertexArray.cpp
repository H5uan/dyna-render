#include "OpenGLVertexArray.h"

#include <glad/glad.h>

#include "OpenGLIndexBuffer.h"

std::unordered_map<ShaderDataType, GLenum> shaderDataTypeToOpenGLBaseTypeMap = {
    {ShaderDataType::Float, GL_FLOAT},
    {ShaderDataType::Float2, GL_FLOAT},
    {ShaderDataType::Float3, GL_FLOAT},
    {ShaderDataType::Float4, GL_FLOAT},
    {ShaderDataType::Mat3, GL_FLOAT},
    {ShaderDataType::Mat4, GL_FLOAT},
    {ShaderDataType::Int, GL_INT},
    {ShaderDataType::Int2, GL_INT},
    {ShaderDataType::Int3, GL_INT},
    {ShaderDataType::Int4, GL_INT},
    {ShaderDataType::Bool, GL_BOOL}
};

using AttribSetter = void (*)(GLuint vao, GLuint index, const BufferElement&element, GLuint bufferID, GLuint stride);

void SetFloatAttribute(GLuint vao, GLuint index, const BufferElement&element, GLuint bufferID, GLuint stride) {
    glEnableVertexArrayAttrib(vao, index);
    glVertexArrayAttribFormat(vao, index, element.GetComponentCount(), GL_FLOAT,
                              element.Normalized ? GL_TRUE : GL_FALSE, element.Offset);
    glVertexArrayAttribBinding(vao, index, index);
    glVertexArrayVertexBuffer(vao, index, bufferID, 0, stride);
}

void SetIntAttribute(GLuint vao, GLuint index, const BufferElement&element, GLuint bufferID, GLuint stride) {
    glEnableVertexArrayAttrib(vao, index);
    glVertexArrayAttribIFormat(vao, index, element.GetComponentCount(), GL_INT, element.Offset);
    glVertexArrayAttribBinding(vao, index, index);
    glVertexArrayVertexBuffer(vao, index, bufferID, 0, stride);
}

void SetMatrixAttribute(GLuint vao, GLuint index, const BufferElement&element, GLuint bufferID, GLuint stride) {
    GLuint count = element.GetComponentCount() / (element.Type == ShaderDataType::Mat3 ? 3 : 4);
    for (GLuint i = 0; i < count; ++i) {
        glEnableVertexArrayAttrib(vao, index + i);
        glVertexArrayAttribFormat(vao, index + i, count, GL_FLOAT, GL_FALSE,
                                  element.Offset + sizeof(float) * count * i);
        glVertexArrayAttribBinding(vao, index + i, index);
        glVertexArrayVertexBuffer(vao, index + i, bufferID, 0, stride);
        glVertexAttribDivisor(index + i, 1); // if using instancing
    }
}

std::unordered_map<ShaderDataType, AttribSetter> attribSetters = {
    {ShaderDataType::Float, SetFloatAttribute},
    {ShaderDataType::Float2, SetFloatAttribute},
    {ShaderDataType::Float3, SetFloatAttribute},
    {ShaderDataType::Float4, SetFloatAttribute},

    {ShaderDataType::Int, SetIntAttribute},
    {ShaderDataType::Int2, SetIntAttribute},
    {ShaderDataType::Int3, SetIntAttribute},
    {ShaderDataType::Int4, SetIntAttribute},

    {ShaderDataType::Bool, SetIntAttribute}, // In opengl, bool is treated as GL_INT

    {ShaderDataType::Mat3, SetMatrixAttribute},
    {ShaderDataType::Mat4, SetMatrixAttribute},
};


OpenGLVertexArray::OpenGLVertexArray() {
    glCreateVertexArrays(1, &mRendererID);
}

OpenGLVertexArray::~OpenGLVertexArray() {
    glDeleteVertexArrays(1, &mRendererID);
}

void OpenGLVertexArray::Bind() const {
    glBindVertexArray(mRendererID);
}

void OpenGLVertexArray::Unbind() const {
    glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>&vertexBuffer) {
    GLCORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout!");

    const BufferLayout&layout = vertexBuffer->GetLayout();
    GLuint bufferID = vertexBuffer->GetID();
    GLuint stride = layout.GetStride();


    for (const BufferElement&element: layout) {
        auto setter = attribSetters.find(element.Type);
        if (setter != attribSetters.end()) {
            setter->second(mRendererID, mVertexBufferIndex, element, bufferID, stride);
            if (element.Type == ShaderDataType::Mat3 || element.Type == ShaderDataType::Mat4) {
                mVertexBufferIndex += element.GetComponentCount() / (element.Type == ShaderDataType::Mat3 ? 3 : 4);
            }
            else {
                mVertexBufferIndex++;
            }
        }
        else {
            GLCORE_ASSERT(false, "Unknown ShaderDataType!");
        }
    }
    mVertexBuffers.push_back(vertexBuffer);
}

void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>&indexBuffer) {
    GLuint buffer_ID = indexBuffer->GetID();
    glVertexArrayElementBuffer(mRendererID, buffer_ID);
    mIndexBuffer = indexBuffer;
}
