#pragma once

class Rogue;
class Knight;
class Elf;
class NPC;

class Visitor {
public:
    virtual ~Visitor() = default;

    virtual void visit(Rogue& attacker, NPC& defender) = 0;
    virtual void visit(Knight& attacker, NPC& defender) = 0;
    virtual void visit(Elf& attacker, NPC& defender) = 0;
};

struct BattleResult {
    NPC* dead1 = nullptr;
    NPC* dead2 = nullptr;
};