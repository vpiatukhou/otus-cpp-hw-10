#pragma once

#include "CommandBlock.h"

namespace Homework {

    class FlushCommandListener {
    public:
        virtual ~FlushCommandListener() {
        }

        virtual void onFlush(const CommandBlock& commands) = 0;
    };

}
