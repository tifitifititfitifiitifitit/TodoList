#pragma once
#include <string>
#include "../services/UserPlanService.h"
#include "../services/UserPointService.h"
// #include "../services/UserTamagotchiService.h" // 주석 처리

class UserPlanService; // 전방 선언
class UserPointService; // 전방 선언
// class UserTamagotchiService; // 주석 처리

class User {
private:
    std::string userId;
    std::string username;
    UserPlanService userPlanService; // 실제 객체 또는 포인터/참조 형태에 따라 UserPlanService.h include 필요
    UserPointService userPointService; // 실제 객체 또는 포인터/참조 형태에 따라 UserPointService.h include 필요
    // UserTamagotchiService userTamagotchiService; // 주석 처리

public:
    User(const std::string& id, const std::string& name);
    
    // Getters
    std::string getUserId() const;
    std::string getUsername() const;
    UserPlanService& getPlanService();
    UserPointService& getPointService();
    // UserTamagotchiService& getTamagotchiService(); // 주석 처리
    
    // Setters
    void setUsername(const std::string& name);
}; 