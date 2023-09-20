#pragma once
#include "pg_core.h"
#include "base/event/pg_event.h"

namespace Pagoda::Base {
    class PAGODA_API Layer {
    public:
        Layer(const std::string& name);
        virtual ~Layer();
        virtual void OnEvent(Event& e) const = 0;
        virtual void OnUpdate() const = 0;
        virtual void OnAttach() = 0;

        inline std::string GetName() {
            return this->m_Name;
        }

    private:
        std::string m_Name;
    };
}