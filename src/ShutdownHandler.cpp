#include "ShutdownHandler.h"

#include <signal.h>
#include <thread> //TODO

void Homework::startShutdownHandler(CommandProcessingContext& context) {
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);
    sigaddset(&sigset, SIGTERM);
    pthread_sigmask(SIG_BLOCK, &sigset, nullptr);

    std::thread signalHandler([&context, &sigset]() {
        int signum = 0;
        sigwait(&sigset, &signum);

        context.stop();
        exit(signum);
    });

    signalHandler.detach();
}
