#pragma once

#include "observer.hpp"

class ConsoleLogger : public IObserver {
public:
    void onEvent(const std::string& msg) override;
};