#pragma once
#include <string>
#include <vector>
#include "TamagotchiState.h"
#include "Tamagotchi.h"

namespace Tamagotchi {

    class Tamagotchi {
    private:
        int pokeId;
        int level;
        int happiness;
        int hunger;
        std::string name;
        int stage; // 현재 진화 단계 (0, 1, 2 ...)
        int whatPoke;
        TamagotchiState* currentState;

        struct EvolutionStage {
            int requiredLevel; // 진화 조건 레벨
            std::vector<std::string> art; // 아스키 아트
            std::string nowName;
        };

        std::vector<EvolutionStage> stages; // 단계별 아트와 조건 저장

        // 복사 생성자와 대입 연산자 삭제
        Tamagotchi(const Tamagotchi&) = delete;
        Tamagotchi& operator=(const Tamagotchi&) = delete;

    public:
        // 생성자, 소멸자
        Tamagotchi(int id, const std::string& pokeName);
        ~Tamagotchi();

        // 상태 관리
        void setState(TamagotchiState* newState);
        std::string getCurrentStateName() const;
        bool tryEvolve();

        // 행동
        void feed();
        void play();
        void increaseHunger();
        void decreaseHappiness(int amount);
        void levelUp();
        void updateStatus();

        // 아트 관련
        const std::vector<std::string>& getCurrentArt() const;

        // getter
        int getPokeId() const;
        std::string getName() const;
        int getLevel() const;
        int getHappiness() const;
        int getHunger() const;
        TamagotchiState* getCurrentState() const;

        // setter
        void setName();
        void setHappiness(int v);
        void setHunger(int v);
        void setPokeId(int id);

        // 단계별 아트와 조건 초기화
        void initializeStages();
    };

} // namespace Tamagotchi
