#pragma once

#include <memory>
#include <string>

#include "point.hpp"
#include "visitor.hpp"

using namespace std;

class Visitor;

class NPC : public enable_shared_from_this<NPC> {
public:
    NPC(string name, const Point& pos);
    virtual ~NPC() = default;

    const string& getName() const;
    const Point& getPosition() const;

    double distanceTo(const NPC& other) const;

    virtual string getType() const = 0;
    virtual bool kills(const NPC& other) const = 0;

    virtual void accept(Visitor& v, NPC& other) = 0;

private:
    string name;
    Point position;
};