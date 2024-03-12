#pragma once

#include "../../GLCore/Rendering/Framebuffer.h"
#include "../../GLCore/Core/Core.h"

class OpenGLFramebuffer : public Framebuffer {
public:
    OpenGLFramebuffer(FramebufferSpecification spec);

    virtual ~OpenGLFramebuffer();

    void Invalidate();

    void Bind() override;

    void Unbind() override;

    void Resize(uint32_t width, uint32_t height) override;

    int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

    void ClearAttachment(uint32_t attachmentIndex, int value) override;

    [[nodiscard]] uint32_t GetColorAttachmentRendererID(uint32_t index) const override {
        GLCORE_ASSERT(index < m_ColorAttachments.size());
        return m_ColorAttachments[index];
    }

    [[nodiscard]] uint32_t GetDepthAttachmentRendererID() const override {
        return m_DepthAttachment;
    }

    [[nodiscard]] const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

private:
    uint32_t m_RendererID = 0;
    FramebufferSpecification m_Specification;

    std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
    FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

    std::vector<uint32_t> m_ColorAttachments;
    uint32_t m_DepthAttachment = 0;
};
