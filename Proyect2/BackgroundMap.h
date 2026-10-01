#pragma once
#include <fstream>
#include <string>
#include <vector>

class BackgroundMap {
private:
    std::vector<std::string> grid;
    int width = 0;
    int height = 0;

public:
    bool loadFromFile(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) return false;

        grid.clear();
        width = 0;
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            if ((int)line.size() > width) width = (int)line.size();
            grid.push_back(line);
        }

        height = (int)grid.size();
        for (auto& row : grid)
            if ((int)row.size() < width) row.resize(width, ' ');

        return height > 0 && width > 0;
    }

    char getPixel(int worldX, int worldY) const {
        if (worldX < 0 || worldY < 0 || worldY >= height || worldX >= width)
            return ' ';
        return grid[worldY][worldX];
    }

    int getWidth() const { return width; }
    int getHeight() const { return height; }
};
