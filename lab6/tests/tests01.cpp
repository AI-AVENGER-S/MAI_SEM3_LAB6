#include "gtest/gtest.h"

#include <fstream>
#include <sstream>

#include "dungeon.hpp"
#include "elf.hpp"
#include "knight.hpp"
#include "npc_factory.hpp"
#include "observer_console.hpp"
#include "observer_file.hpp"
#include "point.hpp"
#include "rogue.hpp"

class MockObserver : public IObserver {
public:
    std::vector<std::string> log_messages;
    void onEvent(const std::string& msg) override {
        log_messages.push_back(msg);
    }
};

// =========================================================================
// 1. Тесты Фабрики (NPCFactory)
// =========================================================================

TEST(NPCFactoryTest, CreateValidNPC) {
    auto rogue = NPCFactory::create("Rogue", "TestRogue", 10, 20);
    ASSERT_NE(rogue, nullptr);
    ASSERT_EQ(rogue->getType(), "Rogue");
    ASSERT_EQ(rogue->getName(), "TestRogue");
    ASSERT_EQ(rogue->getPosition().getX(), 10);

    auto knight = NPCFactory::create("Knight", "TestKnight", 50, 50);
    ASSERT_NE(knight, nullptr);
    ASSERT_EQ(knight->getType(), "Knight");
}

TEST(NPCFactoryTest, CreateUnknownType) {
    auto unknown = NPCFactory::create("Wizard", "Gandalf", 10, 20);
    ASSERT_EQ(unknown, nullptr);
}

TEST(NPCFactoryTest, FromStringValid) {
    auto elf = NPCFactory::fromString("Elf Legolas 100 200");
    ASSERT_NE(elf, nullptr);
    ASSERT_EQ(elf->getType(), "Elf");
    ASSERT_EQ(elf->getName(), "Legolas");
}

TEST(NPCFactoryTest, FromStringOutOfBounds) {
    auto rogue_out = NPCFactory::fromString("Rogue Outlaw 10 501");
    ASSERT_EQ(rogue_out, nullptr);
}

// =========================================================================
// 2. Тесты NPC (Логика Kills - LSP)
// =========================================================================

TEST(NPCRulesTest, RogueKillsElf) {
    Rogue rogue("R", Point(0, 0));
    Elf elf("E", Point(0, 0));
    Knight knight("K", Point(0, 0));

    ASSERT_TRUE(rogue.kills(elf));
    ASSERT_FALSE(rogue.kills(knight));
    ASSERT_FALSE(rogue.kills(rogue));
}

TEST(NPCRulesTest, ElfKillsKnight) {
    Rogue rogue("R", Point(0, 0));
    Elf elf("E", Point(0, 0));
    Knight knight("K", Point(0, 0));

    ASSERT_TRUE(elf.kills(knight));
    ASSERT_FALSE(elf.kills(rogue));
    ASSERT_FALSE(elf.kills(elf));
}

TEST(NPCRulesTest, KnightKillsRogue) {
    Rogue rogue("R", Point(0, 0));
    Elf elf("E", Point(0, 0));
    Knight knight("K", Point(0, 0));

    ASSERT_TRUE(knight.kills(rogue));
    ASSERT_FALSE(knight.kills(elf));
    ASSERT_FALSE(knight.kills(knight));
}

// =========================================================================
// 3. Тесты Dungeon (Battle Logic / Visitor / Observer)
// =========================================================================

class DungeonTest : public ::testing::Test {
protected:
    Dungeon dungeon{50.0};
    MockObserver mock_observer;

    void SetUp() override {
        dungeon.addObserver(&mock_observer);
    }
};

TEST_F(DungeonTest, NoBattleOutOfRange) {
    // 1. Rogue (0, 0)
    dungeon.addNPC("Rogue", "Rogue_A", 0, 0);
    // 2. Knight (100, 0) - Distance 100m. Range is 50m.
    dungeon.addNPC("Knight", "Knight_B", 100, 0);

    dungeon.battle();

    ASSERT_EQ(mock_observer.log_messages.size(), 0);

    std::stringstream ss;
    std::cout.rdbuf(ss.rdbuf());
    dungeon.printAll();
    std::cout.rdbuf(nullptr);

    ASSERT_TRUE(ss.str().find("Текущие NPC в подземелье (2)") != std::string::npos);
}


TEST_F(DungeonTest, SuccessfulKillInRange) {
    dungeon.addNPC("Rogue", "Rogue_A", 0, 0);
    dungeon.addNPC("Elf", "Elf_B", 10, 0);

    dungeon.battle();

    ASSERT_EQ(mock_observer.log_messages.size(), 1);
    ASSERT_TRUE(mock_observer.log_messages[0].find("Rogue_A (Rogue) killed Elf_B (Elf)") != std::string::npos);

    std::stringstream ss;
    std::cout.rdbuf(ss.rdbuf());
    dungeon.printAll();
    std::cout.rdbuf(nullptr);

    ASSERT_TRUE(ss.str().find("Текущие NPC в подземелье (1)") != std::string::npos);
    ASSERT_TRUE(ss.str().find("Rogue_A") != std::string::npos);
    ASSERT_FALSE(ss.str().find("Elf_B") != std::string::npos);
}

TEST_F(DungeonTest, DoubleKillScenario) {
    dungeon.addNPC("Rogue", "Rogue_A", 0, 0);
    dungeon.addNPC("Knight", "Knight_B", 10, 0);
    
    dungeon.battle();
    
    ASSERT_EQ(mock_observer.log_messages.size(), 1);
    ASSERT_TRUE(mock_observer.log_messages[0].find("Knight_B (Knight) killed Rogue_A (Rogue)") != std::string::npos);
    
    std::stringstream ss;
    std::cout.rdbuf(ss.rdbuf());
    dungeon.printAll();
    std::cout.rdbuf(nullptr);
    
    ASSERT_TRUE(ss.str().find("Текущие NPC в подземелье (1)") != std::string::npos);
    ASSERT_TRUE(ss.str().find("Knight_B") != std::string::npos);
    ASSERT_FALSE(ss.str().find("Rogue_A") != std::string::npos);
}

TEST_F(DungeonTest, SaveLoadTest) {
    dungeon.addNPC("Rogue", "R1", 10, 10);
    dungeon.addNPC("Elf", "E1", 20, 20);
    dungeon.saveToFile("temp_save.txt");

    Dungeon new_dungeon(50.0);
    new_dungeon.loadFromFile("temp_save.txt");

    std::stringstream ss;
    std::cout.rdbuf(ss.rdbuf());
    new_dungeon.printAll();
    std::cout.rdbuf(nullptr);

    ASSERT_TRUE(ss.str().find("Текущие NPC в подземелье (2)") != std::string::npos);
    ASSERT_TRUE(ss.str().find("R1") != std::string::npos);
    ASSERT_TRUE(ss.str().find("E1") != std::string::npos);

    std::remove("temp_save.txt");
}

// =========================================================================
// 4. Тест Observer FileLogger
// =========================================================================

TEST_F(DungeonTest, FileLoggerWritesToFile) {
    std::remove("log.txt");

    FileLogger file_log;
    dungeon.addObserver(&file_log);

    dungeon.addNPC("Rogue", "Rogue_File", 0, 0);
    dungeon.addNPC("Elf", "Elf_File", 10, 0);

    dungeon.battle();

    std::ifstream file("log.txt");
    std::string line;
    std::getline(file, line);

    ASSERT_TRUE(line.find("Rogue_File (Rogue) killed Elf_File (Elf)") != std::string::npos);

    std::remove("log.txt");
}