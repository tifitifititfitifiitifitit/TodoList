// models/Tamagotchi.h
#pragma once
#include <string>
#include "TamagotchiState.h"

namespace Tamagotchi {

    class Tamagotchi {
    private:
        int pokeId;
        std::string name;
        int level;
        int happiness;
        int hunger;
        TamagotchiState* currentState;

    public:
        Tamagotchi(int id, const std::string& pokeName);
        ~Tamagotchi();
        void setState(TamagotchiState* newState);
        std::string getCurrentStateName() const;
        void feed();
        void play();
        void increaseHunger();
        void decreaseHappiness(int amount);
        void levelUp();
        void updateStatus();
        int getPokeId() const;
        std::string getName() const;
        int getLevel() const;
        int getHappiness() const;
        int getHunger() const;
        TamagotchiState* getCurrentState() const;
        void setName(const std::string& newName);
        void setHappiness(int value);
        void setHunger(int value);
    };

} // namespace Tamagotchi
