#include "ShutdownHandler.h"
#include "ErrorCodes.h"

#include <cstdlib>
#include <signal.h>
#include <thread>

#include <iostream>

void Homework::startShutdownHandler(std::shared_ptr<CommandProcessingContext>& context) {
    sigset_t sigset;
    if (sigemptyset(&sigset)) {
        std::cerr << "Error cleaning signal set." << std::endl;
        std::exit(INTERNAL_ERROR);
    }

    if (sigaddset(&sigset, SIGINT)) {
        std::cerr << "Error adding SIGINT." << std::endl;
        std::exit(INTERNAL_ERROR);
    }

    if (sigaddset(&sigset, SIGTERM)) {
        std::cerr << "Error adding SIGTERM." << std::endl;
        std::exit(INTERNAL_ERROR);
    }

    if (pthread_sigmask(SIG_BLOCK, &sigset, nullptr)) {
        std::cerr << "Error blocking signals." << std::endl;
        std::exit(INTERNAL_ERROR);
    }

    std::thread shutdownThread([context, sigset] {
        int exitCode = INTERNAL_ERROR;

        if (sigwait(&sigset, &exitCode)) {
            std::cerr << "Error waiting signal." << std::endl;
        } else {
            context->stop();
        }
        std::exit(exitCode);
    });

    shutdownThread.detach();
}
