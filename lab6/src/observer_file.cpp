#include "observer_file.hpp"

#include <fstream>

void FileLogger::onEvent(const std::string& message) {
    std::ofstream file("log.txt", std::ios::app);
    if (file.is_open())
        file << message << std::endl;
}