#pragma once
#include "Mirage/MirageCore/Model/pgModel.h"

namespace Pagoda::Mirage {
    class Renderer {
    public:
        Renderer(Model& model);
        virtual ~Renderer();

        virtual void Draw() const = 0;

    protected:
        Model& m_Model;
    };
}