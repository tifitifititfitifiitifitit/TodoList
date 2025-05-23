<<<<<<< HEAD
﻿// UserTamagotchiService.h
#pragma once
#include "../models/Tamagotchi.h"  // 이제 models/Tamagotchi.h 안에 namespace Tamagotchi { class Tamagotchi; ... }
=======
﻿#pragma once
#include "../models/Tamagotchi.h"
>>>>>>> main

/*namespace Tamagotchi {
    // 전방 선언 (헤더만 포함할 때 필요)
    class Tamagotchi;
}*/

// 행동 타입은 그대로 전역으로 둬도 되고, namespace 안에 넣어도 됩니다.
enum class TamagotchiAction { FEED, PLAY };

class UserTamagotchiService {
private:
    // fully-qualified name으로 바꿔 줍니다.
    Tamagotchi::Tamagotchi* tamagotchi;

public:
    UserTamagotchiService();
    ~UserTamagotchiService();
<<<<<<< HEAD

    // Tamagotchi 관리
    void assignTamagotchi(Tamagotchi::Tamagotchi* newTamagotchi);
    void interactWithTamagotchi(TamagotchiAction action);

    // Getter
    Tamagotchi::Tamagotchi* getTamagotchi() const;
=======
   
    void assignTamagotchi(Tamagotchi* newTamagotchi);
    void interactWithTamagotchi(TamagotchiAction action);

    Tamagotchi* getTamagotchi() const;
>>>>>>> main
};
