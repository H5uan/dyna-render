#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

class SimpleLayer final : public GLCore::Layer
{
public:
    SimpleLayer();
    ~SimpleLayer() override;

    void OnAttach() override;
    void OnDetach() override;
    void OnEvent(GLCore::Event& event) override;
    void OnUpdate(GLCore::Timestep ts) override;
    void OnImGuiRender() override;
private:

};