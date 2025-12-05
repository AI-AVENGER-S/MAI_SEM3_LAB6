#include "elf.hpp"

#include "visitor.hpp"

Elf::Elf(const std::string& name, const Point& pos) : NPC(name, pos) {}

std::string Elf::getType() const {
    return "Elf";
}

bool Elf::kills(const NPC& other) const {
    return other.getType() == "Knight";
}

void Elf::accept(Visitor& v, NPC& other) {
    v.visit(*this, other);
}