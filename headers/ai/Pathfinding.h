#pragma once
/*
Edited by Mike
*/



#include <ctime>
#include <set>
#include <vector>
/* Source:https://stackoverflow.com/questions/19193413/pathfinding-in-a-grid-system */


// Creating a shortcut for int, int pair type
typedef std::pair<int, int> Pair;
// Creating a shortcut for pair<int, pair<int, int>> type
typedef std::pair<double, std::pair<int, int> > pPair;

class Pathfinding{
private:
    // Map Grid size - replace this later
    #define ROW 7
    #define COL 7


    // Creating a path list
    int dirMap[ROW][COL] = {0};
    std::vector<std::pair<int,int>> dirVec;
    // A structure to hold the neccesary parameters
    struct cell;
    bool isValid(int row, int col);
    bool isUnBlocked(int grid[][COL], int row, int col);
    bool isDestination(int row, int col, Pair dest);
    double calculateHValue(int row, int col, Pair dest);
    void tracePath(cell cellDetails[][COL], Pair dest);

public:
    void aStarSearch(int grid[][COL], Pair src, Pair dest);
    void printDirMap();
    void printDirVec();
    std::vector<std::pair<int,int>> GetDirMap();
    std::pair<int, int> getNewPosition(int moveRange);
    

};





