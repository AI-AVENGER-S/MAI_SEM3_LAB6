#pragma once

#include <string>
#include <vector>

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void onEvent(const std::string& msg) = 0;
};