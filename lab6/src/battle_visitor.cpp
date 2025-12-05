#include "battle_visitor.hpp"

#include <cmath>
#include <iostream>

#include "elf.hpp"
#include "knight.hpp"
#include "npc.hpp"
#include "rogue.hpp"

BattleVisitor::BattleVisitor(double range) : attackRange(range) {}

void BattleVisitor::addObserver(IObserver* obs) {
    observers.push_back(obs);
}

void BattleVisitor::notify(const std::string& msg) {
    for (auto* o : observers)
        o->onEvent(msg);
}

BattleResult BattleVisitor::resolve(NPC& A, NPC& B) {
    BattleResult result;

    if (A.distanceTo(B) > attackRange)
        return result;

    bool aKills = A.kills(B);
    bool bKills = B.kills(A);

    if (aKills && bKills) {
        result.dead1 = &A;
        result.dead2 = &B;
        notify(A.getName() + " (" + A.getType() + ") and " + B.getName() + " (" + B.getType() + ") killed each other.");
    } else if (aKills) {
        result.dead2 = &B;
        notify(A.getName() + " (" + A.getType() + ") killed " + B.getName() + " (" + B.getType() + ").");
    } else if (bKills) {
        result.dead1 = &A;
        notify(B.getName() + " (" + B.getType() + ") killed " + A.getName() + " (" + A.getType() + ").");
    }

    return result;
}

void BattleVisitor::visit(Rogue& attacker, NPC& defender) {
    resolve(attacker, defender);
}

void BattleVisitor::visit(Knight& attacker, NPC& defender) {
    resolve(attacker, defender);
}

void BattleVisitor::visit(Elf& attacker, NPC& defender) {
    resolve(attacker, defender);
}