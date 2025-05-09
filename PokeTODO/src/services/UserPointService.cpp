#include "UserPointService.h"

UserPointService::UserPointService()
    : points(0) // points 멤버 변수를 0으로 초기화
{
    // 생성자에서 수행할 추가 로직이 있다면 여기에 작성
}

void UserPointService::addPoints(int amount) {
    this->points += amount;
}

int UserPointService::getPoints() const {
    return this->points;
} 