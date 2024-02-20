#pragma once

#include "../../GLCore/Rendering/Framebuffer.h"
#include "../../GLCore/Core/Core.h"

class OpenGLFramebuffer : public Framebuffer {
public:
    explicit OpenGLFramebuffer(const FramebufferSpecification&spec);

    ~OpenGLFramebuffer() override;

    void InValidate();

    void Bind() override;

    void Unbind() override;

    void BindReadFramebuffer() override;

    void BindDrawFramebuffer() override;

    virtual void Resize(uint32_t width, uint32_t height) override;

    virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

    virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

    [[nodiscard]] uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override {
        GLCORE_ASSERT(index < mColorAttachments.size());
        return mColorAttachments[index];
    }

    [[nodiscard]] uint32_t GetDepthAttachmentRendererID() const override { return mDepthAttachment; }

    [[nodiscard]] const FramebufferSpecification& GetSpecification() const override { return mSpecification; }

    void FramebufferTexture2D(uint32_t cubemapIndex, uint32_t cubemapID, uint32_t slot = 0) override;

    [[nodiscard]] Ref<Texture3D> GetDepthTex3D() const override; // have bugs
    void BindDepthTex3D(uint32_t slot) override;

    void UnBindDepthTex3D(uint32_t slot) override;

private:
    uint32_t mRendererID = 0;
    FramebufferSpecification mSpecification;

    std::vector<FramebufferTextureSpecification> mColorAttachmentSpecifications;

    FramebufferTextureSpecification mDepthAttachmentSpecification{FramebufferTextureFormat::None};

    std::vector<uint32_t> mColorAttachments;
    uint32_t mDepthAttachment = 0;
};
