#pragma once
#include "../models/Tamagotchi.h"

enum class TamagotchiAction { FEED, PLAY };

class UserTamagotchiService {
private:
    Tamagotchi* tamagotchi;

public:
    UserTamagotchiService();
    ~UserTamagotchiService();
   
    void assignTamagotchi(Tamagotchi* newTamagotchi);
    void interactWithTamagotchi(TamagotchiAction action);

    Tamagotchi* getTamagotchi() const;
};
