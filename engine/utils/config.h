#pragma once

#include <string>
#include <map>

namespace bure {

class config {
 public:
    void readFile(std::string path);
    bool getBoolValueOf(std::string of);
    void setValueOf(std::string of, std::string on);

    static config instance;
 private:
    std::map<std::string, std::string> configs;
};

}  // namespace bure
