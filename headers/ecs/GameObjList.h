#pragma once

#include "ecs/GameObj.h"
#include <vector>
#include <string>

class GameObjList {
public:
    GameObjList();
    GameObjList(std::vector<GameObj> objects);

    void addGameObj(GameObj obj);
    size_t size();
    GameObj getGameObj(int index) const;
    void deleteGameObj(int index);
    std::vector<GameObj> getGameObjList() const;

private:
    std::vector<GameObj> data;
    void serializeList();
};