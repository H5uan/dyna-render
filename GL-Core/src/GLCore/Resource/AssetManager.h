#pragma once

#include <GLCore/Core/PublicSingleton.h>

#include <filesystem>

class AssetManager : public PublicSingleton<AssetManager> {
public:
    [[nodiscard]] static std::filesystem::path GetFullPath(const std::string&RelativePath);
};
