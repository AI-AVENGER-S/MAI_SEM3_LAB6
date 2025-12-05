#pragma once

#include <cmath>
#include <iostream>

class Point {
public:
    Point(int x, int y);

    int getX() const;
    int getY() const;

    double distanceTo(const Point& other) const;

private:
    int x;
    int y;
};