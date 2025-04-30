#pragma once
#include <map>
#include <vector>
#include <ctime>
#include "../models/Plan.h"
#include "../models/Task.h"
#include "../models/TaskCreateRequest.h"

class UserPlanService {
private:
    std::map<std::time_t, std::vector<Plan>> plans;

public:
    UserPlanService();

    // Plan management
    Plan createPlan(const std::time_t& date, const std::string& title);
    Plan* getPlan(const std::string& planId);
    Task addTaskToPlan(const std::string& planId, const TaskCreateRequest& request);
    void completeTask(const std::string& taskId);

    // Getters
    const std::map<std::time_t, std::vector<Plan>>& getPlans() const;
}; 