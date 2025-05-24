#pragma once
#include "../models/Tamagotchi.h"

/*namespace Tamagotchi {
    // 전방 선언 (헤더만 포함할 때 필요)
    class Tamagotchi;
}*/

// 행동 타입은 그대로 전역으로 둬도 되고, namespace 안에 넣어도 됩니다.
enum class TamagotchiAction { FEED, PLAY };

class UserTamagotchiService {
private:
    // fully-qualified name으로 바꿔 줍니다.
    Tamagotchi* tamagotchi;

public:
    UserTamagotchiService();
    ~UserTamagotchiService();
   
    void assignTamagotchi(Tamagotchi* newTamagotchi);
    void interactWithTamagotchi(TamagotchiAction action);

    Tamagotchi* getTamagotchi() const;
};
