#pragma once

#include <GLCore/Rendering/VertexArray.h>

    class OpenGLVertexArray : public VertexArray {
    public:
        OpenGLVertexArray();

        virtual ~OpenGLVertexArray();

        void Bind() const override;

        void Unbind() const override;

        void AddVertexBuffer(const Ref<VertexBuffer>&vertexBuffer) override;

        void SetIndexBuffer(const Ref<IndexBuffer>&indexBuffer) override;

        const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return mVertexBuffers; }
        const Ref<IndexBuffer>& GetIndexBuffer() const override { return mIndexBuffer; }

    private:
        uint32_t mRendererID;
        uint32_t mVertexBufferIndex = 0;
        std::vector<Ref<VertexBuffer>> mVertexBuffers;
        Ref<IndexBuffer> mIndexBuffer;
    };

