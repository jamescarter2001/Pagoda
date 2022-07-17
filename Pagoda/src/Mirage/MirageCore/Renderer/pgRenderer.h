#pragma once
#include "Mirage/MirageCore/Model/pgModel.h"

namespace Pagoda::Mirage {
    class Renderer {
    public:
        Renderer();
        ~Renderer();

        virtual void Clear() const = 0;
        virtual void Draw(Model& model) const = 0;
    };
}