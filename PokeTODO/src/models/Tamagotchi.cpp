#include "Tamagotchi.h"
#include "TamagotchiState.h"
#include <algorithm>  // for std::max

namespace Tamagotchi {

    // 생성자: 아이디랑 이름 받고, 초기 상태로 NormalState 할당
    Tamagotchi::Tamagotchi(int id, const std::string& pokeName)
        : pokeId(id)
        , name(pokeName)
        , level(1)
        , happiness(100)
        , hunger(0)
        , currentState(nullptr)
    {
        setState(new NormalState());
    }

    // 소멸자
    Tamagotchi::~Tamagotchi() {
        delete currentState;
    }

    // 상태 전환
    void Tamagotchi::setState(TamagotchiState* newState) {
        if (currentState) {
            currentState->onExitState(this);
            delete currentState;
        }
        currentState = newState;
        if (currentState) {
            currentState->onEnterState(this);
        }
    }

    // 현재 상태 이름 얻기
    std::string Tamagotchi::getCurrentStateName() const {
        return currentState
            ? currentState->getStateName()
            : std::string{};
    }

    // 행동
    void Tamagotchi::feed() {
        if (currentState) currentState->handleFeed(this);
    }
    void Tamagotchi::play() {
        if (currentState) currentState->handlePlay(this);
    }
    void Tamagotchi::increaseHunger() {
        ++hunger;
    }
    void Tamagotchi::decreaseHappiness(int amount) {
        happiness = std::max(0, happiness - amount);
    }
    void Tamagotchi::levelUp() {
        ++level;
    }
    void Tamagotchi::updateStatus() {
        if (currentState) currentState->checkCondition(this);
    }

    // ─── getters ───
    int Tamagotchi::getPokeId() const { return pokeId; }
    std::string Tamagotchi::getName() const { return name; }
    int Tamagotchi::getLevel() const { return level; }
    int Tamagotchi::getHappiness() const { return happiness; }
    int Tamagotchi::getHunger() const { return hunger; }
    TamagotchiState* Tamagotchi::getCurrentState() const { return currentState; }

    // ─── setters ───
    void Tamagotchi::setName(const std::string& n) { name = n; }
    void Tamagotchi::setHappiness(int v) { happiness = v; }
    void Tamagotchi::setHunger(int v) { hunger = v; }

} // namespace Tamagotchi
