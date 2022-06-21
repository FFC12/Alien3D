#ifndef ALIEN3D_CAMERA_H
#define ALIEN3D_CAMERA_H
#include <gfx/WorldCamera.h>
#include <engine/Component.h>

class Camera : public WorldCamera, Component {
public:
    Camera(): WorldCamera(false) {
    }
};

#endif