#include "Tamagotchi.h"
#include "TamagotchiState.h"
#include <algorithm>

namespace Tamagotchi {

    // — HappyState —
    void HappyState::handleFeed(Tamagotchi* t) {
        t->setHappiness(std::min(t->getHappiness() + 10, 100));
        t->setHunger(std::max(t->getHunger() - 5, 0));
    }

    void HappyState::handlePlay(Tamagotchi* t) {
        t->setHappiness(std::min(t->getHappiness() + 20, 100));
        t->setHunger(std::min(t->getHunger() + 10, 100));
    }

    void HappyState::checkCondition(Tamagotchi* t) {
        if (t->getHunger() > 80)      t->setState(new HungryState());
        else if (t->getHappiness() < 50) t->setState(new SadState());
    }

    void HappyState::onEnterState(Tamagotchi* ) { /* 진입 시 이펙트 */ }
    void HappyState::onExitState(Tamagotchi* ) { /* 종료 시 정리 */ }

    std::string HappyState::getStateName() const {
        return "Happy";
    }


    // — NormalState —
    void NormalState::handleFeed(Tamagotchi* t) {
        t->setHunger(std::max(t->getHunger() - 10, 0));
    }

    void NormalState::handlePlay(Tamagotchi* t) {
        t->setHappiness(std::min(t->getHappiness() + 10, 100));
        t->setHunger(std::min(t->getHunger() + 5, 100));
    }

    void NormalState::checkCondition(Tamagotchi* t) {
        if (t->getHunger() > 60) t->setState(new HungryState());
        else if (t->getHappiness() > 80) t->setState(new HappyState());
        else if (t->getHappiness() < 40) t->setState(new SadState());
    }

    void NormalState::onEnterState(Tamagotchi* /*t*/) {}
    void NormalState::onExitState(Tamagotchi* /*t*/) {}
    std::string NormalState::getStateName() const { return "Normal"; }


    // — SadState —
    void SadState::handleFeed(Tamagotchi* t) {
        t->setHunger(std::max(t->getHunger() - 5, 0));
        t->setHappiness(std::min(t->getHappiness() + 5, 100));
    }

    void SadState::handlePlay(Tamagotchi* t) {
        t->setHappiness(std::min(t->getHappiness() + 30, 100));
        t->setHunger(std::min(t->getHunger() + 10, 100));
    }

    void SadState::checkCondition(Tamagotchi* t) {
        if (t->getHappiness() > 60)          t->setState(new HappyState());
        else if (t->getHunger() > 70)     t->setState(new HungryState());
    }

    void SadState::onEnterState(Tamagotchi* /*t*/) {}
    void SadState::onExitState(Tamagotchi* /*t*/) {}
    std::string SadState::getStateName() const { return "Sad"; }


    // — HungryState —
    void HungryState::handleFeed(Tamagotchi* t) {
        t->setHunger(std::max(t->getHunger() - 30, 0));
    }

    void HungryState::handlePlay(Tamagotchi* t) {
        t->setHappiness(std::max(t->getHappiness() - 5, 0));
    }

    void HungryState::checkCondition(Tamagotchi* t) {
        if (t->getHunger() < 20)                     t->setState(new NormalState());
        else if (t->getHunger() < 10 && t->getHappiness() > 80)
            t->setState(new HappyState());
    }

    void HungryState::onEnterState(Tamagotchi* /*t*/) {}
    void HungryState::onExitState(Tamagotchi* /*t*/) {}
    std::string HungryState::getStateName() const { return "Hungry"; }

} // namespace Tamagotchi
