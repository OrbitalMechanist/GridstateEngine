#pragma once

#include <utility>
#include <memory>
#include <string>

class GameObj {
public:
    std::string gameName;

    GameObj(std::string inputName);

	GameObj();

	~GameObj();

    void setSerializedName(std::string input);

	void update(float deltaTime);


private:
    std::string serializedName;
};