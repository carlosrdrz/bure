#pragma once

#include "event.h"

namespace events {
    class keyboard_event : public event {
     public:
        static const int EVENT_ID = 1;
    };
};