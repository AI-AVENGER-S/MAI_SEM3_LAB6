#pragma once

#include "npc.hpp"

class Elf : public NPC {
public:
    Elf(const std::string& name, const Point& pos);

    std::string getType() const override;
    bool kills(const NPC& other) const override;

    void accept(Visitor& v, NPC& other) override;
};