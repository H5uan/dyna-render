#include "GLCore/Library/ShaderLibrary.h"
#include "GLCore/Resource/AssetManager.h"


Library<Shader>::Library() {
    //Add("Base", Shader::CreateNative(AssetManager::GetFullPath("Shaders/BasePBR.glsl")));
    Add("SkyBox", Shader::CreateNative(AssetManager::GetFullPath("Shaders/SkyBox.glsl")));
}

Ref<Shader> Library<Shader>::GetDefaultShader() {
    return mLibrary["BasePBR"];
}

Ref<Shader> Library<Shader>::GetSkyBoxShader() {
    return mLibrary["SkyBox"];
}
