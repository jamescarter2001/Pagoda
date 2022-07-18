#pragma once

#include "pgpch.h"

#include "Math/pgMath.h"
#include "Mirage/MirageCore/Model/pgModel.h"

namespace Pagoda::Mirage {
    class Object {
    protected:        
        Eigen::Vector3f m_Position;
        Eigen::Vector3f m_Rotation;
    };
}