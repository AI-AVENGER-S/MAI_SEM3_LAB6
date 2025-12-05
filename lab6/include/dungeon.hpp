#pragma once

#include <memory>
#include <string>
#include <vector>

#include "battle_visitor.hpp"
#include "npc_factory.hpp"
#include "observer.hpp"

using namespace std;


class Dungeon {
public:
    Dungeon(double battleRange);

    void addNPC(const string& type, const string& name, int x, int y);
    void addObserver(IObserver* observer);

    void printAll() const;
    void battle();
    

    void saveToFile(const string& filename) const;
    void loadFromFile(const string& filename);

private:
    vector<shared_ptr<NPC> > npcs;
    BattleVisitor battleVisitor;
};