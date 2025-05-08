#pragma once
#include <string>
#include "../services/UserPlanService.h"
#include "../services/UserPointService.h"
#include "../services/UserTamagotchiService.h"

class User {
private:
    std::string userId;
    std::string username;
    UserPlanService userPlanService;
    UserPointService userPointService;
    UserTamagotchiService userTamagotchiService;

public:
    User(const std::string& id, const std::string& name);
    
    // Getters
    std::string getUserId() const;
    std::string getUsername() const;
    UserPlanService& getPlanService();
    UserPointService& getPointService();
    UserTamagotchiService& getTamagotchiService();
    
    // Setters
    void setUsername(const std::string& name);
}; 