#pragma once

#include <vector>

#include "observer.hpp"
#include "visitor.hpp"

using namespace std;


class BattleVisitor : public Visitor {
public:
    BattleVisitor(double range);

    void addObserver(IObserver* obs);

    void visit(Rogue& attacker, NPC& defender) override;
    void visit(Knight& attacker, NPC& defender) override;
    void visit(Elf& attacker, NPC& defender) override;
    
    double getAttackRange() const { return attackRange; }

private:
    void notify(const string& msg);
    
    BattleResult resolve(NPC& A, NPC& B);

    double attackRange;
    vector<IObserver*> observers;
};