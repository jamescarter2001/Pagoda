#pragma once

#include "pgpch.h"

#include "Mirage/MirageCore/Object/pgObject.h"

namespace Pagoda::Mirage {
    class pgCamera {
    private:
        float m_AspectRatio;
        float m_Near;
        float m_Far;
    };
}