#pragma once
#include "Base/Message/pgMessage.h"
#include "pgCore.h"
#include "pgpch.h"

namespace Pagoda::Base {

    // Auto-generate
    /*#define EVENT_CLASS_TYPE(type) static eEventType GetStaticEventType() { return eEventType::type; }\
									   virtual eEventType GetEventType() const override { return GetStaticEventType(); }\
										virtual const char* GetName() const override { return #type; }*/

#define EVENT_CLASS_CATEGORY(bitfield) \
    virtual int GetCategories() const override { return (int)bitfield; }

    enum eEventCategory {
        None = 0,
        EventCategoryApplication = BIT(0),
        EventCategoryInput = BIT(1),
        EventCategoryKeyboard = BIT(2),
        EventCategoryMouse = BIT(3),
        EventCategoryMouseButton = BIT(4)
    };

    class Event : public Message {
    public:
        virtual int GetCategories() const = 0;
    };
}
