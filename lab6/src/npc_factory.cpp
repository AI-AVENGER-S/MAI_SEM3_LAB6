#include "npc_factory.hpp"

#include <memory>
#include <sstream>

#include "elf.hpp"
#include "knight.hpp"
#include "rogue.hpp"

using namespace std;

shared_ptr<NPC> NPCFactory::create(const string& type, const string& name, int x, int y) {
    if (type == "Rogue")
        return make_shared<Rogue>(name, Point(x, y));
    if (type == "Knight")
        return make_shared<Knight>(name, Point(x, y));
    if (type == "Elf")
        return make_shared<Elf>(name, Point(x, y));
    return nullptr;
}

shared_ptr<NPC> NPCFactory::fromString(const string& line) {
    stringstream ss(line);
    string type, name;
    int x, y;
    ss >> type >> name >> x >> y;
    
    if (x < 0 || x > 500 || y < 0 || y > 500) {
        cerr << "Error loading NPC: Coordinates (" << x << ", " << y << ") out of bounds." << endl;
        return nullptr;
    }
    
    return create(type, name, x, y);
}