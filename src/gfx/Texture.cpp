#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

void Texture::generateTexture(const char *path,
                              TextureWrappingMode wrappingModeS,
                              TextureWrappingMode wrappingModeT,
                              TextureFilteringMode filteringModeMin,
                              TextureFilteringMode filteringModeMag) {
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    stbi_set_flip_vertically_on_load(true);

    i32 w, h, ch;
    uchar *data = stbi_load(path, &w, &h, &ch, 0);

    if (!data) {
        ALIEN_ERROR("Data is corrupted!");
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrappingModeS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrappingModeT);
    //TODO: GL_TEXTURE_WRAP_R

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filteringModeMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filteringModeMag);

    stbi_image_free(data);
}

void Texture::generateTexture(const char *path,
                              Gfx_u32 shaderProgram,
                              const char *texName, TextureWrappingMode wrappingModeS,
                              TextureWrappingMode wrappingModeT,
                              TextureFilteringMode filteringModeMin,
                              TextureFilteringMode filteringModeMag) {
    glGenTextures(1, &m_TextureID);
    glActiveTexture(GL_TEXTURE0 + this->m_CurrUnitID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    stbi_set_flip_vertically_on_load(true);

    i32 w, h, ch;
    uchar *data = stbi_load(path, &w, &h, &ch, 0);

    if (!data) {
        ALIEN_ERROR("Data is corrupted!");
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrappingModeS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrappingModeT);
    //TODO: GL_TEXTURE_WRAP_R

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filteringModeMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filteringModeMag);

    glUniform1i(glGetUniformLocation(shaderProgram, texName), this->m_CurrUnitID);

    this->m_CurrUnitID += 1;

    stbi_image_free(data);
}