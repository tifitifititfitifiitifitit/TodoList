#pragma once
#include "../models/Tamagotchi.h"

enum class TamagotchiAction { FEED, PLAY };

class UserTamagotchiService {
private:
    Tamagotchi* tamagotchi;

public:
    UserTamagotchiService();
    ~UserTamagotchiService();

    // Tamagotchi management
    void assignTamagotchi(Tamagotchi* newTamagotchi);
    void interactWithTamagotchi(TamagotchiAction action);

    // Getters
    Tamagotchi* getTamagotchi() const;
}; 