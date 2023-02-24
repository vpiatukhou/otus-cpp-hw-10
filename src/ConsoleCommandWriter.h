#pragma once

#include "AsyncCommandWriter.h"

namespace Homework {

    /**
     * Writes a given command block to a standart output stream.
     */
    class ConsoleCommandWriter : public AsyncCommandWriter {
    public:
        ConsoleCommandWriter();

    protected:
        void write(const CommandBlock& commands, NumberOfThreads threadNumber) override;

    };

}
