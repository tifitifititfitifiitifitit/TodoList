/*
#pragma once
#include <string>
#include "TamagotchiState.h"

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

    // State management
    void setState(TamagotchiState* newState);
    std::string getCurrentStateName() const;

    // Actions
    void feed();
    void play();
    void increaseHunger();
    void decreaseHappiness(int amount);
    void levelUp();
    void updateStatus();

    // Getters
    int getPokeId() const;
    std::string getName() const;
    int getLevel() const;
    int getHappiness() const;
    int getHunger() const;
    TamagotchiState* getCurrentState() const;

    // Setters
    void setName(const std::string& newName);
    void setHappiness(int value);
    void setHunger(int value);
}; 
*/ 