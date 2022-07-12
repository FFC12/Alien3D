#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

void Image::loadImage(const char *path, bool isAlpha) {
    stbi_set_flip_vertically_on_load(true);

    m_Data = std::shared_ptr<uchar>(stbi_load(path, &m_Width, &m_Height, &m_Channels, 0),
                                    [=](uchar *data) { stbi_image_free(data); });
}
