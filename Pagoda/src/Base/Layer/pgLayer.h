#pragma once
#include "pgCore.h"
#include "Base/Event/pgEvent.h"

namespace Pagoda::Base {
    class PAGODA_API Layer {
    public:
        Layer(const std::string& name);
        virtual ~Layer();
        virtual void OnEvent(Event& e) const = 0;
        virtual void OnUpdate() const = 0;
        virtual void OnAttach() const = 0;

        inline std::string GetName() {
            return this->m_Name;
        }

    private:
        std::string m_Name;
    };
}