#pragma once

#include "pgpch.h"

#include "Math/pgMath.h"
#include "Mirage/MirageCore/Model/pgModel.h"
#include "Mirage/MirageCore/Object/pgObject.h"

namespace Pagoda::Universe {
    class Actor : public Mirage::Object {
    public:
        void Translate(float x, float y, float z);
        void Rotate(float v1, float v2, float v3);

        inline Mirage::Model& GetModel() {
            return this->m_Model;
        }

    private:
        Mirage::Model& m_Model;
    };
}