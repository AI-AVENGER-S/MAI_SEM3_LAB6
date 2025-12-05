#include "dungeon.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>

#include "npc.hpp"
#include "rogue.hpp"
#include "knight.hpp"
#include "elf.hpp"

using namespace std;

Dungeon::Dungeon(double battleRange) : battleVisitor(battleRange) {}

void Dungeon::addNPC(const string& type, const string& name, int x, int y) {
    if (x < 0 || x > 500 || y < 0 || y > 500) {
        cerr << "Ошибка: координаты NPC (" << x << ", " << y
             << ") выходят за допустимые пределы (0–500)." << endl;
        return;
    }
    
    auto npc = NPCFactory::create(type, name, x, y);
    if (npc)
        npcs.push_back(npc);
    else
        cerr << "Неизвестный тип NPC: " << type << endl;
}

void Dungeon::addObserver(IObserver* observer) {
    battleVisitor.addObserver(observer);
}

// Печать перечня объектов
void Dungeon::printAll() const {
    cout << "\n--- Текущие NPC в подземелье (" << npcs.size() << ") ---\n";
    for (const auto& npc : npcs) {
        cout << "Тип: " << npc->getType()
             << ", Имя: " << npc->getName()
             << ", Позиция: (" << npc->getPosition().getX()
             << ", " << npc->getPosition().getY() << ")" << endl;
    }
    cout << "-------------------------------------------\n";
}

// Запуск боевого режима
void Dungeon::battle() {
    vector<shared_ptr<NPC>> alive = npcs;

    vector<NPC*> dead_in_battle;

    cout << "\n--- БИТВА НАЧАЛАСЬ (Дальность: " << battleVisitor.getAttackRange() << " м) ---\n";

    for (size_t i = 0; i < alive.size(); ++i) {
        for (size_t j = i + 1; j < alive.size(); ++j) {
            
            if (!alive[i] || !alive[j])
                continue;

            alive[i]->accept(battleVisitor, *alive[j]);
            
            bool iKills = alive[i]->kills(*alive[j]) && alive[i]->distanceTo(*alive[j]) <= battleVisitor.getAttackRange();
            bool jKills = alive[j]->kills(*alive[i]) && alive[j]->distanceTo(*alive[i]) <= battleVisitor.getAttackRange();

            if (iKills || jKills) {
                if (iKills && jKills) {
                    dead_in_battle.push_back(alive[i].get());
                    dead_in_battle.push_back(alive[j].get());
                } else if (iKills) {
                    dead_in_battle.push_back(alive[j].get());
                } else if (jKills) {
                    dead_in_battle.push_back(alive[i].get());
                }
            }
        }
    }
    
    alive.erase(remove_if(alive.begin(), alive.end(),
                          [&dead_in_battle](const shared_ptr<NPC>& npc) {
                              if (!npc) return false;
                              return find(dead_in_battle.begin(), dead_in_battle.end(), npc.get()) != dead_in_battle.end();
                          }),
                alive.end());

    cout << "--- БИТВА ЗАВЕРШЕНА. Убрано NPC: " << (npcs.size() - alive.size()) << " ---\n";
    npcs = alive;
}

void Dungeon::saveToFile(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: не удалось открыть файл для сохранения: " << filename << endl;
        return;
    }

    for (const auto& npc : npcs) {
        file << npc->getType() << " " << npc->getName() << " "
             << npc->getPosition().getX() << " "
             << npc->getPosition().getY() << endl;
    }
    cout << "\nДанные успешно сохранены в файл: " << filename << endl;
}

void Dungeon::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: не удалось открыть файл для загрузки: " << filename << endl;
        return;
    }

    npcs.clear();
    string line;
    while (getline(file, line)) {
        auto npc = NPCFactory::fromString(line);
        if (npc)
            npcs.push_back(npc);
    }
    cout << "\nДанные успешно загружены из файла: " << filename
         << ". Всего NPC: " << npcs.size() << endl;
}
