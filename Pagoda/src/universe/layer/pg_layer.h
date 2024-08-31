#pragma once
#include "pgpch.h"
#include "pg_core.h"

#include "base/event/pg_event.h"

#include "universe/context/pg_application_context.h"

namespace Pagoda::Universe {
    class PAGODA_API Layer {
    public:
        Layer(const std::string& name);
        virtual ~Layer();
        virtual void OnEvent(ApplicationContext& ctx, Base::Event& e) const = 0;
        virtual void OnUpdate(ApplicationContext& ctx) = 0;
        virtual void OnAttach(ApplicationContext& ctx) = 0;

        inline std::string GetName() {
            return this->m_Name;
        }

    private:
        std::string m_Name;
    };
}