#include "UserTamagotchiService.h"
// #include "../models/Tamagotchi.h" // UserTamagotchiService.h 에서 이미 include 함
// #include "../models/TamagotchiState.h" // Tamagotchi.h 또는 UserTamagotchiService.h 에서 이미 include 할 수 있음 (직접 사용하지 않는다면 불필요)
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

// 헤더 파일의 선언과 일치하도록 수정 (Tamagotchi::Tamagotchi*)
void UserTamagotchiService::assignTamagotchi(Tamagotchi::Tamagotchi* newAssignedPet) // 파라미터 이름을 newTamagotchi에서 변경하여 혹시 모를 충돌 방지
{
    if (tamagotchi != newAssignedPet) {
        if (tamagotchi) {
            delete tamagotchi;
        }
        tamagotchi = newAssignedPet;
    }
}

void UserTamagotchiService::interactWithTamagotchi(TamagotchiAction action) // TamagotchiAction은 전역 enum이므로 그대로 사용
{
    if (!tamagotchi) return;

    // tamagotchi 포인터는 Tamagotchi::Tamagotchi* 타입이므로 멤버 접근은 그대로 사용
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

// 헤더 파일의 선언과 일치하도록 수정 (Tamagotchi::Tamagotchi*)
Tamagotchi::Tamagotchi* UserTamagotchiService::getTamagotchi() const
{
    return tamagotchi;
}
