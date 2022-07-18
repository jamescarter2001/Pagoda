#include "pgpch.h"

#include "pgActor.h"

namespace Pagoda::Universe {
    void Actor::Translate(float x, float y, float z) {
        this->m_Position += Eigen::Vector3f(x, y, z);
    }

    void Actor::Rotate(float v1, float v2, float v3) {

    }
}