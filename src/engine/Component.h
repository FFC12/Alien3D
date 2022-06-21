#ifndef ALIEN3D_COMPONENT_H
#define ALIEN3D_COMPONENT_H
#include <utils/Uuid.h>

class Component {
public:
    Component() {
        this->m_UUID = Utils::getUUID();
    }

protected:
    std::string m_UUID;
private:
};

#endif //ALIEN3D_COMPONENT_H
