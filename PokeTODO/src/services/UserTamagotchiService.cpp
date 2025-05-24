#include "UserTamagotchiService.h"
#include "../models/Tamagotchi.h"
#include "../models/TamagotchiState.h"
#include <iostream> 
#include <string>   
#include <vector>   
#include <map>
#include <algorithm>

UserTamagotchiService::UserTamagotchiService()
    : tamagotchi(nullptr)
{
}

UserTamagotchiService::~UserTamagotchiService()
{
    if (tamagotchi) {
        delete tamagotchi;
        tamagotchi = nullptr;
    }
}

void UserTamagotchiService::assignTamagotchi(Tamagotchi* newTamagotchi)
{
    if (tamagotchi != newTamagotchi) {
        if (tamagotchi) {
            delete tamagotchi;
        }
        tamagotchi = newTamagotchi;
    }
}

void UserTamagotchiService::interactWithTamagotchi(TamagotchiAction action)
{
    if (!tamagotchi) return;

    switch (action) {
    case TamagotchiAction::FEED:
        tamagotchi->feed();
        break;
    case TamagotchiAction::PLAY:
        tamagotchi->play();
        break;
    default:
        break;
    }

    tamagotchi->updateStatus();
}

Tamagotchi* UserTamagotchiService::getTamagotchi() const
{
    return tamagotchi;
}
