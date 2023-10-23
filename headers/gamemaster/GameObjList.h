#pragma once

#include "GameObj.h"
#include <vector>
#include <string>

class GameObjList {
private:
    std::vector<GameObj> data;

public:
    GameObjList();
    GameObjList(std::vector<GameObj> objects);

    void addGameObj(GameObj obj);
    int size();
    GameObj getGameObj(int index) const;
    void deleteGameObj(int index);
    std::vector<GameObj> getGameObjList() const;

private:
    void serializeList();
};