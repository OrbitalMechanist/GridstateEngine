#include "mapgridbuilder/MapGrid.h"

/*
    This class defines the game's map, currently each cell only holds strings,
    later cells will hold references to game objects.
    Currently very placeholder.
*/

// check if a cell is actually part of the grid, just in case
bool MapGrid::isValidCell(int x, int y) const {
    return (x >= 0 && x < rows && y >= 0 && y < cols);
}

// Initialization function
MapGrid::MapGrid(int rows, int cols) : rows(rows), cols(cols) {
    // Initialize the grid and place an empty string in each cell (strings are a placeholder datatype)
    data.resize(rows, std::vector<std::string>(cols, ""));
}

// Replaces a Cell's contents
void MapGrid::setCellContent(int x, int y, const std::string& value) {
    if (isValidCell(x, y)) {
        data[x][y] = value;
    }
}

// PLACEHOLDER as hell, represents adding multiple objects to one cell
void MapGrid::addCellContent(int x, int y, const std::string& value) {
    if (isValidCell(x, y)) {
        data[x][y] += value;
    }
}

// PLACEHOLDER, currently retrieves strings, will later retrieve a list of dataobjects occupying the cell
// Get the value from a cell at a specific row and column
std::string MapGrid::getCellValue(int x, int y) const {
    if (isValidCell(x, y)) {
        return data[x][y];
    }
    else {
        return ""; // PLACEHOLDER returns empty for an invalid cell, this should raise an error
    }
}