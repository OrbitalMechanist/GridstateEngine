#ifndef MAPGRID_H
#define MAPGRID_H

#include <vector>
#include <string>

class MapGrid {
private:
    int rows;
    int cols;
    std::vector<std::vector<std::string>> data; //placeholder strings, to be changed to game objects

    bool isValidCell(int x, int y) const;

public:
    MapGrid(int rows, int cols);

    //placeholder strings, to be changed to game objects
    void setCellContent(int x, int y, const std::string& value);
    void addCellContent(int x, int y, const std::string& value);

    std::string getCellValue(int x, int y) const;
};

#endif