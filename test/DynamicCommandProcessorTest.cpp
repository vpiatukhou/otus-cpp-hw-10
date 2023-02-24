#include "DynamicCommandProcessor.h"

#include "gmock/gmock.h"

namespace {
    using namespace Homework;
    using namespace std;
    using ::testing::_;

    class FlushCommandListenerMock : public FlushCommandListener {
    public:
        MOCK_METHOD(void, onFlush, (const vector<string>& commands), (override));
    };
}

TEST(dynamicCommandProcessorTest, processPositive) {
    //given
    auto commandListener1 = std::make_shared<FlushCommandListenerMock>();
    auto commandListener2 = std::make_shared<FlushCommandListenerMock>();
    std::vector<std::shared_ptr<FlushCommandListener>> listeners = { commandListener1, commandListener2 };
    DynamicCommandProcessor commandProcessor(listeners);

    //expect
    vector<string> block = { "cmd1", "cmd2" };

    EXPECT_CALL(*commandListener1, onFlush(block));
    EXPECT_CALL(*commandListener2, onFlush(block));

    //when
    EXPECT_TRUE(commandProcessor.process("{"));
    EXPECT_TRUE(commandProcessor.process("cmd1"));
    EXPECT_TRUE(commandProcessor.process("cmd2"));
    EXPECT_TRUE(commandProcessor.process("}"));
    EXPECT_FALSE(commandProcessor.process("cmd3"));
}

TEST(dynamicCommandProcessorTest, ignoreCommandIfDynamicBlockIsNotOpened) {
    //given
    auto commandListener = std::make_shared<FlushCommandListenerMock>();
    std::vector<std::shared_ptr<FlushCommandListener>> listeners = { commandListener };

    DynamicCommandProcessor commandProcessor(listeners);

    //expect
    EXPECT_CALL(*commandListener, onFlush(_)).Times(0);

    //when
    EXPECT_FALSE(commandProcessor.process("cmd1"));
}

TEST(dynamicCommandProcessorTest, nestedDynamicBlock) {
    //given
    auto commandListener = std::make_shared<FlushCommandListenerMock>();
    std::vector<std::shared_ptr<FlushCommandListener>> listeners = { commandListener };

    DynamicCommandProcessor commandProcessor(listeners);

    //expect
    vector<string> block = { "cmd1", "cmd2", "cmd3", "cmd4", "cmd5" };
    EXPECT_CALL(*commandListener, onFlush(block));

    //when
    EXPECT_TRUE(commandProcessor.process("{"));
    EXPECT_TRUE(commandProcessor.process("cmd1"));
    EXPECT_TRUE(commandProcessor.process("cmd2"));
    EXPECT_TRUE(commandProcessor.process("{"));
    EXPECT_TRUE(commandProcessor.process("cmd3"));
    EXPECT_TRUE(commandProcessor.process("cmd4"));
    EXPECT_TRUE(commandProcessor.process("}"));
    EXPECT_TRUE(commandProcessor.process("cmd5"));
    EXPECT_TRUE(commandProcessor.process("}"));
}
