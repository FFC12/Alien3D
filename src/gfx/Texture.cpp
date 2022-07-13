#include "Texture.h"

void Texture::generateTexture(const char *path,
                              Gfx_u32 shaderProgram,
                              const char *texName, TextureWrappingMode wrappingModeS,
                              TextureWrappingMode wrappingModeT,
                              TextureFilteringMode filteringModeMin,
                              TextureFilteringMode filteringModeMag) {
    if (path == nullptr) {
        return;
    }

    this->m_ImagePath = std::string(path);

    if (TextureCaches.count(path) > 0) {
        auto textureCache = TextureCaches.find(path);
        m_TextureIDs[textureCache->second.first] = textureCache->second.second;
    } else {
        Gfx_u32 textureId;
        glGenTextures(1, &textureId);
        glActiveTexture(GL_TEXTURE0 + this->m_CurrUnitID);
        glBindTexture(GL_TEXTURE_2D, textureId);

        std::shared_ptr<Image> image = std::make_shared<Image>(path);
        TextureDataCache[path] = image;
        uchar *data = image->getData().get();
        auto imageSize = image->getImageSize();
        i32 w = imageSize.first;
        i32 h = imageSize.second;

        if (!data) {
            ALIEN_ERROR("Data is corrupted!");
        } else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrappingModeT);
        //TODO: GL_TEXTURE_WRAP_R

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filteringModeMag);

        auto loc = glGetUniformLocation(shaderProgram, texName);
        glUniform1i(loc, this->m_CurrUnitID);

        this->m_TextureIDs[textureId] = m_CurrUnitID;
        TextureCaches[path] = {textureId, m_CurrUnitID};

        this->m_CurrUnitID += 1;
    }
}

// Does not cache
Gfx_u32 Texture::generateTexture(const char *path,
                                 const char *texName,
                                 Gfx_u32 shaderProgram,
                                 bool isAlphaTransparent,
                                 TextureWrappingMode wrappingModeS,
                                 TextureWrappingMode wrappingModeT,
                                 TextureFilteringMode filteringModeMin,
                                 TextureFilteringMode filteringModeMag) {
//    this->m_ImagePath = std::string(path);
    m_BatchImagePaths.emplace_back(path);

    Gfx_u32 textureId;
    glGenTextures(1, &textureId);
    glCheckError();
    glActiveTexture(GL_TEXTURE0 + this->m_CurrUnitID);
    glCheckError();
    glBindTexture(GL_TEXTURE_2D, textureId);
    glCheckError();

    i32 w, h, ch;
    std::shared_ptr<uchar> data;

    std::string pathStr(path);
    if (TextureDataCache.count(pathStr) > 0) {
        auto texture = TextureDataCache.find(pathStr);
        data = texture->second->getData();
        auto imageSize = texture->second->getImageSize();
        w = imageSize.first;
        h = imageSize.second;
    } else {
        TextureDataCache[pathStr] = std::make_shared<Image>(path, isAlphaTransparent);
        data = TextureDataCache[pathStr]->getData();
        auto imageSize = TextureDataCache[pathStr]->getImageSize();
        w = imageSize.first;
        h = imageSize.second;
    }

    if (!data) {
        ALIEN_ERROR("Data is corrupted!");
    }

    if (isAlphaTransparent) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.get());
        glCheckError();
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data.get());
        glCheckError();
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    glCheckError();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrappingModeS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrappingModeT);
    //TODO: GL_TEXTURE_WRAP_R

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filteringModeMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filteringModeMag);

//        auto loc = glGetUniformLocation(shaderProgram, texName);
//    glUniform1i(loc, this->m_CurrUnitID);
    glCheckError();

    this->m_TextureIDs[textureId] = m_CurrUnitID;

    this->m_CurrUnitID += 1;

    return this->m_CurrUnitID - 1;
}

Texture::~Texture() {
    for (auto texture: m_TextureIDs) {
        glDeleteTextures(1, &texture.first);
    }
}
