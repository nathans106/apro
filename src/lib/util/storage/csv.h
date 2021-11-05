#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace stmlib {
class Csv {
public:
    using Data = std::vector<std::vector<std::string>>;

    Csv(const std::string& name, const std::filesystem::path& dir = {});

    bool exists() const;
    Data load() const;
    void save(const Data& data);

private:
    const std::filesystem::path file;
};
}