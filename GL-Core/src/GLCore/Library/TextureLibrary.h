#pragma once

#include "GLCore/Rendering/Texture.h"
#include "GLCore/Library/Library.h"


template<>
class Library<Texture2D> : public LibraryBase<Library, Texture2D> {
public:
    Library();

    [[nodiscard]] Ref<Texture2D> GetDefaultTexture();

    [[nodiscard]] Ref<Texture2D> GetWhiteTexture();
};
