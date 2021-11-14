#include "csv.h"

#include <fstream>

namespace util {
namespace {
std::filesystem::path makeFile(const std::string& name, const std::filesystem::path& dir) {
    if (dir.empty()) {
        return { name + ".csv" };
    }
    if (!std::filesystem::exists(dir) || !std::filesystem::is_directory(dir)) {
        throw std::runtime_error("Invalid CSV dir: " + dir.string());
    }

    auto file = dir;
    file.append(name + ".csv");
    return file;
}
}

Csv::Csv(const std::string& name, const std::filesystem::path& dir) : file(makeFile(name, dir)) {}

bool Csv::exists() const {
    return std::filesystem::exists(file);
}

Csv::Data Csv::load() const {
    if (!exists()) {
        return {};
    }

    std::ifstream iStream;
    iStream.open(file);
    std::string line;
    Data data;

    while (std::getline(iStream, line)) {
        std::vector<std::string> row;
        size_t prevPos = -1;

        while (prevPos != std::string::npos) {
            const auto delPos = line.find(',', prevPos + 1);
            if (delPos == std::string::npos) {
                row.push_back(line.substr(prevPos + 1));
            }
            else {
                row.push_back(line.substr(prevPos + 1, delPos - prevPos + 1));
            }

            prevPos = delPos;
        }

        data.push_back(row);
    }

    iStream.close();
    return data;
}

void Csv::save(const Data& data) {
    std::ofstream oStream;
    oStream.open(file, std::ofstream::trunc);

    bool firstRow = true;
    for (const auto& row : data) {
        if (firstRow) {
            firstRow = false;
        }
        else {
            oStream << std::endl;
        }

        bool firstCol = true;
        for (const auto& value : row) {
            if (firstCol) {
                firstCol = false;
            }
            else {
                oStream << ",";
            }

            oStream << value;
        }
    }

    oStream.close();
}
}
