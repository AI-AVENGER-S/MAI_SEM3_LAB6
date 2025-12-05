#pragma once

#include "observer.hpp"

class FileLogger : public IObserver {
public:
    void onEvent(const std::string& message) override;
};