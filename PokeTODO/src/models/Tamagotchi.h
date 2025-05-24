#pragma once
#include <string>
#include "TamagotchiState.h"

namespace Tamagotchi { // 네임스페이스 시작

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

public: // 접근 제한자 변경
    // 생성자 (public으로 이동)
    Tamagotchi(int id, const std::string& pokeName);
    // 소멸자 (public으로 이동)
    ~Tamagotchi();

    // 상태 관리
    void setState(TamagotchiState* newState);
    std::string getCurrentStateName() const;

    // 행동
    void feed();
    void play();
    void increaseHunger(); // TamagotchiState.cpp 에서 직접 멤버에 접근하지 않도록 public으로 변경 가능성 있음 (혹은 친구 클래스)
    void decreaseHappiness(int amount); // TamagotchiState.cpp 에서 직접 멤버에 접근하지 않도록 public으로 변경 가능성 있음
    void levelUp(); // TamagotchiState.cpp 에서 직접 멤버에 접근하지 않도록 public으로 변경 가능성 있음
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

} // namespace Tamagotchi // 네임스페이스 종료
