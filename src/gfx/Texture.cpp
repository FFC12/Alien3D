#include "Texture.h"

void Texture::generateTexture(const char *path,
                              Gfx_u32 shaderProgram,
                              const char *texName,
                              bool shaderProgramNeeded, 
                              TextureWrappingMode wrappingModeS,
                              TextureWrappingMode wrappingModeT,
                              TextureFilteringMode filteringModeMin,
                              TextureFilteringMode filteringModeMag) {
    if (path == nullptr) {
        return;
    }

    this->m_ImagePath = std::string(path);

    Gfx_u32 textureId;
    glGenTextures(1, &textureId);
    glActiveTexture(GL_TEXTURE0 + this->m_CurrUnitID);
    glBindTexture(GL_TEXTURE_2D, textureId);

    std::shared_ptr<Image> image = std::make_shared<Image>(path);
    uchar *data = image->getData().get();

    if (!data) {
        ALIEN_ERROR("Data is corrupted!");
        glDeleteTextures(1, &textureId);
        return;
    } else {
        TextureDataCache[path] = image;

        auto imageSize = image->getImageSize();
        i32 w = imageSize.first;
        i32 h = imageSize.second;

        i32 c = image->getImageChannels();

        if (c > 3) {
            // alpha
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        } else {
            // not-alpha
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }

        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrappingModeS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrappingModeT);
    //TODO: GL_TEXTURE_WRAP_R

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filteringModeMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filteringModeMag);

    if (shaderProgramNeeded) { 
      auto loc = glGetUniformLocation(shaderProgram, texName);
      glUniform1i(loc, this->m_CurrUnitID);
    }
    else {
      auto loc = glGetUniformLocation(m_AssocProgram, texName);
      glUniform1i(loc, this->m_CurrUnitID);
    }

    m_TextureID = textureId;
    m_UnitID = m_CurrUnitID;

    this->m_CurrUnitID += 1;

    if (!m_Initialized) {
        m_Initialized = true;
        m_AssocProgram = shaderProgram;
    }
}

Texture::~Texture() {
    glDeleteTextures(1, &m_TextureID);
}
 
