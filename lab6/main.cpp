#include <iostream>

#include "dungeon.hpp"
#include "observer_console.hpp"
#include "observer_file.hpp"

int main() {
    Dungeon editor(100.0);

    ConsoleLogger console_log;
    FileLogger file_log;
    editor.addObserver(&console_log);
    editor.addObserver(&file_log);

    std::cout << "--- Adding NPCs ---\n";
    editor.addNPC("Rogue", "Rogue_A", 10, 10);
    editor.addNPC("Rogue", "Rogue_B", 60, 10);
    
    editor.addNPC("Knight", "Knight_X", 10, 50);
    editor.addNPC("Knight", "Knight_Y", 100, 100); 

    editor.addNPC("Elf", "Elf_P", 10, 20); 
    editor.addNPC("Elf", "Elf_Q", 300, 300);

    editor.printAll();

    editor.saveToFile("dungeon_state.txt");
    
    editor.loadFromFile("dungeon_state.txt");
    editor.printAll();

    
    // Ожидаемые бои (в пределах 100м):
    // Rogue_A (10, 10) vs Elf_P (10, 20) -> Rogue_A kills Elf_P (Distance 10m)
    // Knight_X (10, 50) vs Rogue_A (10, 10) -> Knight_X kills Rogue_A (Distance 40m)
    // Rogue_B (60, 10) vs Knight_Y (100, 100) -> Rogue_B vs Knight_Y (Distance 98.48m) -> Knight_Y kills Rogue_B

    editor.battle();

    editor.printAll();

    return 0;
}