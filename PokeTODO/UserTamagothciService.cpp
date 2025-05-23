#include "src/services/UserTamagotchiService.h"
#include "src/models/Tamagotchi.h"

//생성자
UserTamagotchiService::UserTamagotchiService()
    : tamagotchi(nullptr)
{}


//소멸자
UserTamagotchiService::~UserTamagotchiService() {
    // 만약 Service가 Tamagotchi 소유권을 가지지 않는다면 delete 하지 않습니다.
    tamagotchi = nullptr;
}


//다마고치 할당
void UserTamagotchiService::assignTamagotchi(Tamagotchi::Tamagotchi* newTamagotchi) {
    if (tamagotchi) {
        // 이미 다마고치가 할당되어 있으므로 추가할 수 없음
        return;
    }
    tamagotchi = newTamagotchi;
}

//다마고치와 상호작용
void UserTamagotchiService::interactWithTamagotchi(TamagotchiAction action) {
    if (!tamagotchi) return;

    switch (action) {
        case TamagotchiAction::FEED:
            tamagotchi->feed();
            break;
        case TamagotchiAction::PLAY:
            tamagotchi->play();
            break;
    }

    tamagotchi->updateStatus();
}

Tamagotchi::Tamagotchi* UserTamagotchiService::getTamagotchi() const {
    return tamagotchi;
}
