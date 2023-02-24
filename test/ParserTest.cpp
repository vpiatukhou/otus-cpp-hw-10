#include "RequestParser.h"

#include "gtest/gtest.h"

#include <vector>

void testParser(const std::vector<std::string>& toParse, const std::vector<std::string>& expectedCommands) {
    using namespace Homework;

    std::vector<std::string> receivedCommands;

    RequestParser parser;
    parser.onCommandReceived = [&receivedCommands](const std::string& command) {
        receivedCommands.push_back(command);
    };

    for (auto& line : toParse) {
        parser.parse(line);
    }
    ASSERT_EQ(expectedCommands, receivedCommands);
}

void testParser(const std::string& toParse, const std::vector<Homework::Request>& expectedCommands) {
    std::vector<std::string> lines = { toParse };
    testParser(lines, expectedCommands);
}

TEST(parserTest, singleCommandWithoutArguments) {
    using namespace Homework;

    std::vector<std::string> expectedCommands = { "cmd" };
    testParser("cmd\n", expectedCommands);
}

TEST(parserTest, commandSequence) {
    using namespace Homework;

    std::vector<std::string> expectedCommands = { "cmd1", "cmd2", "cmd3" };
    testParser("cmd1\ncmd2\ncmd3\n", expectedCommands);
}

TEST(parserTest, commandSequenceParts) {
    using namespace Homework;

    std::vector<std::string> toParse = { "cm", "d1\n", "cmd2", "\ncmd3", "\n" };
    std::vector<std::string> expectedCommands = { "cmd1", "cmd2", "cmd3" };
    testParser(toParse, expectedCommands);
}