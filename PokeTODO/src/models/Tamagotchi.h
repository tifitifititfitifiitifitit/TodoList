#pragma once
#include <string>
#include "TamagotchiState.h"

class TamagotchiState;  // 전방 선언(포인터만 사용하므로 가능)

class Tamagotchi {
private:
    int pokeId;
    std::string name;
    int level;
    int happiness;
    int hunger;
    TamagotchiState* currentState;

    // 복사 생성자와 복사 대입 연산자 삭제 (포인터 관리 때문에)
    Tamagotchi(const Tamagotchi&) = delete;
    Tamagotchi& operator=(const Tamagotchi&) = delete;

    // 상태 관리
    void setState(TamagotchiState* newState);
    std::string getCurrentStateName() const;

    // 행동
    void feed();
    void play();
    void increaseHunger();
    void decreaseHappiness(int amount);
    void levelUp();
    void updateStatus();

    // getter
    int getPokeId() const;
    std::string getName() const;
    int getLevel() const;
    int getHappiness() const;
    int getHunger() const;
    TamagotchiState* getCurrentState() const;

    // setter
    void setName(const std::string& newName);
    void setHappiness(int value);
    void setHunger(int value);
};
