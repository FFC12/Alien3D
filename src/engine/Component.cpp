#include "Component.h"
#include <gfx/Texture.h>
#include <gfx/Shader.h>
#include <engine/SpriteAnimation.h>
#include <engine/PhysicsBody.h>
#include <engine/Transform.h>

static Component *Create(const ComponentType &type, Sprite &sprite) {
    switch (type) {
        case SPRITE_ANIM:
            return new SpriteAnimation(sprite);
        case PHYSICS_BODY:
            break;
        case TRANSFORM:
            break;
        case GAME_SCRIPT:
            break;
        case SHADER:
            break;
        case TEXTURE:
            break;
        case NO_COMP:
            break;
    }
}
