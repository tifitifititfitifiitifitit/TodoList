/*
#pragma once

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

    // 상태 getter 메서드들
    EmotionalState getEmotionalState() const { return emotionalState; }
    PhysicalState getPhysicalState() const { return physicalState; }
    ActivityState getActivityState() const { return activityState; }

    // 상태 setter 메서드들
    void setEmotionalState(EmotionalState state) { emotionalState = state; }
    void setPhysicalState(PhysicalState state) { physicalState = state; }
    void setActivityState(ActivityState state) { activityState = state; }

    // 상태를 문자열로 변환하는 메서드들
    std::string getEmotionalStateString() const;
    std::string getPhysicalStateString() const;
    std::string getActivityStateString() const;

    // 상태 업데이트 메서드들
    void updateHappiness(int delta);
    void updateEnergy(int delta);
    void updateHealth(int delta);

protected:
    EmotionalState emotionalState;
    PhysicalState physicalState;
    ActivityState activityState;

    int happiness;  // 0-100
    int energy;     // 0-100
    int health;     // 0-100

    // 상태 전이 로직을 처리하는 내부 메서드들
    void updateStatesBasedOnStats();
    void clampStats();
};

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

// 아픈 상태
class SickState : public TamagotchiState {
public:
    void handleFeed(Tamagotchi* tamagotchi) override;
    void handlePlay(Tamagotchi* tamagotchi) override;
    void checkCondition(Tamagotchi* tamagotchi) override;
    void onEnterState(Tamagotchi* tamagotchi) override;
    void onExitState(Tamagotchi* tamagotchi) override;
    std::string getStateName() const override;
};

// 피곤한 상태
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
*/ 