#include "StaticCommandProcessor.h"

#include "gmock/gmock.h"

namespace {
    using namespace Homework;
    using namespace std;
    using ::testing::_;

    const size_t BLOCK_SIZE = 3;

    class FlushCommandListenerMock : public FlushCommandListener {
    public:
        MOCK_METHOD(void, onFlush, (const vector<string>& commands), (override));
    };
}

TEST(staticCommandProcessorTest, processPositive) {
    //given
    auto commandListener1 = std::make_shared<FlushCommandListenerMock>();
    auto commandListener2 = std::make_shared<FlushCommandListenerMock>();
    std::vector<std::shared_ptr<FlushCommandListener>> listeners = { commandListener1, commandListener2 };
    StaticCommandProcessor commandProcessor(listeners, BLOCK_SIZE);

    //expect
    vector<string> block = { "cmd1", "cmd2", "cmd3" };

    EXPECT_CALL(*commandListener1, onFlush(block));
    EXPECT_CALL(*commandListener2, onFlush(block));

    //when
    commandProcessor.process("cmd1");
    commandProcessor.process("cmd2");
    commandProcessor.process("cmd3");
}

TEST(staticCommandProcessorTest, processTwoBlocksAndIgnoreIncompleteBlock) {
    //given
    auto commandListener = std::make_shared<FlushCommandListenerMock>();
    std::vector<std::shared_ptr<FlushCommandListener>> listeners = { commandListener };
    StaticCommandProcessor commandProcessor(listeners, BLOCK_SIZE);

    //expect
    EXPECT_CALL(*commandListener, onFlush(CommandBlock{ "cmd1", "cmd2", "cmd3" }));
    EXPECT_CALL(*commandListener, onFlush(CommandBlock{ "cmd4", "cmd5", "cmd6" }));

    //when
    vector<string> block = { "cmd1", "cmd2", "cmd3", "cmd4", "cmd5", "cmd6" };
    for (auto& command : block) {
        commandProcessor.process(command);
    }
    //incomplete block will be ignored
    commandProcessor.process("cmd7");
    commandProcessor.process("cmd8");
}

TEST(staticCommandProcessorTest, flush) {
    //given
    auto commandListener = std::make_shared<FlushCommandListenerMock>();
    std::vector<std::shared_ptr<FlushCommandListener>> listeners = { commandListener };
    StaticCommandProcessor commandProcessor(listeners, BLOCK_SIZE);

    //expect
    vector<string> block = { "cmd1", "cmd2" };

    EXPECT_CALL(*commandListener, onFlush(block));

    //when
    commandProcessor.process("cmd1");
    commandProcessor.process("cmd2");
    commandProcessor.flush();
}
