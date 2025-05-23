// models/TamagotchiState.h
#pragma once
#include "Tamagotchi.h"
#include <string>

namespace Tamagotchi {

    // 감정 상태를 나타내는 열거형
    enum class EmotionalState {
        HAPPY,
        NEUTRAL,
        SAD,
        EXCITED,
        ANGRY,
        SLEEPY
    };

    // 신체 상태를 나타내는 열거형
    enum class PhysicalState {
        HEALTHY,
        HUNGRY,
        SICK,
        TIRED,
        ENERGETIC,
        FULL
    };

    // 활동 상태를 나타내는 열거형
    enum class ActivityState {
        IDLE,
        PLAYING,
        EATING,
        SLEEPING,
        EXERCISING
    };

    // 기본 상태 인터페이스
    class TamagotchiState {
    public:
        virtual ~TamagotchiState() = default;
        virtual void handleFeed(Tamagotchi* tamagotchi) = 0;
        virtual void handlePlay(Tamagotchi* tamagotchi) = 0;
        virtual void checkCondition(Tamagotchi* tamagotchi) = 0;
        virtual void onEnterState(Tamagotchi* tamagotchi) = 0;
        virtual void onExitState(Tamagotchi* tamagotchi) = 0;
        virtual std::string getStateName() const = 0;

        // 상태 값 보조 메서드들...
    protected:
        EmotionalState emotionalState;
        PhysicalState physicalState;
        ActivityState activityState;
        int happiness;
        int energy;
        int health;
    };

    // — 상태 클래스 선언들 —

    // 행복 상태
    class HappyState : public TamagotchiState {
    public:
        void handleFeed(Tamagotchi* tamagotchi) override;
        void handlePlay(Tamagotchi* tamagotchi) override;
        void checkCondition(Tamagotchi* tamagotchi) override;
        void onEnterState(Tamagotchi* tamagotchi) override;
        void onExitState(Tamagotchi* tamagotchi) override;
        std::string getStateName() const override;
    };

    // 일반 상태
    class NormalState : public TamagotchiState {
    public:
        void handleFeed(Tamagotchi* tamagotchi) override;
        void handlePlay(Tamagotchi* tamagotchi) override;
        void checkCondition(Tamagotchi* tamagotchi) override;
        void onEnterState(Tamagotchi* tamagotchi) override;
        void onExitState(Tamagotchi* tamagotchi) override;
        std::string getStateName() const override;
    };

    // 슬픈 상태  ← 헤더에 SadState 추가!
    class SadState : public TamagotchiState {
    public:
        void handleFeed(Tamagotchi* tamagotchi) override;
        void handlePlay(Tamagotchi* tamagotchi) override;
        void checkCondition(Tamagotchi* tamagotchi) override;
        void onEnterState(Tamagotchi* tamagotchi) override;
        void onExitState(Tamagotchi* tamagotchi) override;
        std::string getStateName() const override;
    };

    // 배고픈 상태
    class HungryState : public TamagotchiState {
    public:
        void handleFeed(Tamagotchi* tamagotchi) override;
        void handlePlay(Tamagotchi* tamagotchi) override;
        void checkCondition(Tamagotchi* tamagotchi) override;
        void onEnterState(Tamagotchi* tamagotchi) override;
        void onExitState(Tamagotchi* tamagotchi) override;
        std::string getStateName() const override;
    };

    // 피곤한 상태 (옵션)
    class TiredState : public TamagotchiState {
    public:
        void handleFeed(Tamagotchi* tamagotchi) override;
        void handlePlay(Tamagotchi* tamagotchi) override;
        void checkCondition(Tamagotchi* tamagotchi) override;
        void onEnterState(Tamagotchi* tamagotchi) override;
        void onExitState(Tamagotchi* tamagotchi) override;
        std::string getStateName() const override;
    };

} // namespace Tamagotchi
