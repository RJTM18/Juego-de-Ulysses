#include <vector>
#include <string>
#include <fstream> //para leer .txt

class BackgroundMap {
private:
    std::vector grid;
    int width, height;
public:
    bool loadFromFile(const std::string& filepath) {
        std::ifstream file(filepath);
        std::string line;
        while (std::getline(file, line)) {
            grid.push_back(line);
        }
        height = grid.size();
        width = grid.empty() ? 0 : grid[0].size();
        //en caso de que grid no tenga nada, retornara o (? 0)

        return true;
    }

    char getPixel(int worldX, int worldY) const {
        if (worldY < 0 || worldY >= height || worldX < 0 || worldX >= width) {
            return ' '; // Out of bounds fallback
        }
        return grid[worldY][worldX];
    }
};