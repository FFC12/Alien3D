#ifndef ALIEN3D_IMAGE_H
#define ALIEN3D_IMAGE_H

#include <string>
#include <Base.hpp>
#include <memory>

// This is a simple Image class
// for wrapping to the data
class Image {
public:
    explicit Image(const std::string &path, bool isAlpha = true) : m_ImagePath(path), m_IsAlphaTransparent(isAlpha) {
        loadImage(path.c_str(), isAlpha);
    }

    std::shared_ptr<uchar> &getData() {
        return this->m_Data;
    }

    std::pair<i32, i32> getImageSize() {
        return {m_Width, m_Height};
    }

    i32 getImageChannels() {
        return m_Channels;
    }

private:
    void loadImage(const char *path, bool isAlpha);

    std::shared_ptr<uchar> m_Data;

    i32 m_Width;
    i32 m_Height;
    i32 m_Channels;

    std::string m_ImagePath;
    bool m_IsAlphaTransparent;
};


#endif //ALIEN3D_IMAGE_H
