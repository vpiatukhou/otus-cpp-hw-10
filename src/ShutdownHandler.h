#pragma once

#include "CommandProcessingContext.h"

namespace Homework {

    /**
     * The handler catches SIGINT and SIGTERM and does graceful shutdown: it flushes remained commands and waits until they are processed.
     * 
     * @param context the context is used to process remained commands.
     *                ATTENTION: make sure that the instance is not destroyed after this method returns control.
     */
    void startShutdownHandler(CommandProcessingContext& context);
}
