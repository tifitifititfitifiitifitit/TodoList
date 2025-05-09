#include "../services/UserPlanService.h" // UserPlanService.h를 가장 먼저 인클루드 (또는 관련된 모델 헤더들 이후)
#include "../models/Plan.h"
#include "../models/Task.h"
#include "../models/TaskCreateRequest.h"
#include <iostream> // For std::cerr, std::endl
#include <string>   // For std::to_string
#include <vector>   // Included by Plan.h etc., but good practice to be explicit if used directly
#include <map>      // For std::map

// Static counters for ID generation
static int planIdCounter = 0;
static int taskIdCounter = 0;

UserPlanService::UserPlanService() {
    // Constructor logic (if needed)
}

// UserPlanService::~UserPlanService() {} // Uncomment or remove if destructor is not declared in header

Plan UserPlanService::createPlan(const std::time_t& date, const std::string& title) {
    std::string newPlanId = "plan-" + std::to_string(++planIdCounter);
    Plan newPlan(newPlanId, date, title);
    this->plans[date].push_back(newPlan); // Add to the list of Plans for that date
    return newPlan;
}

Plan* UserPlanService::getPlan(const std::string& planIdToFind) {
    for (auto& mapEntry : this->plans) { // 올바른 map 순회 방식
        std::vector<Plan>& planList = mapEntry.second;
        for (auto& plan : planList) {
            if (plan.getPlanId() == planIdToFind) {
                return &plan;
            }
        }
    }
    return nullptr;
}

Task UserPlanService::addTaskToPlan(const std::string& planId, const TaskCreateRequest& request) {
    Plan* plan = getPlan(planId); // getPlan(const std::string&) 호출
    if (plan) {
        std::string newTaskId = planId + "-task-" + std::to_string(++taskIdCounter);
        Task newTask(newTaskId, request);
        plan->addTask(newTask);
        return newTask;
    } else {
        // Plan not found - exception handling or returning an empty/error Task needed
        // For now, simply return a default-constructed Task (to clearly indicate an error)
        // Consider throwing std::runtime_error in a real application
        std::cerr << "Error: Plan with ID " << planId << " not found. Cannot add task." << std::endl;
        // Since Task requires TaskCreateRequest, create an empty request for the error task.
        TaskCreateRequest emptyRequest("ERROR", "Plan not found", Priority::LOW, time(0), {});
        return Task("error-task", emptyRequest); 
    }
}

void UserPlanService::completeTask(const std::string& taskIdToComplete) {
    for (auto& mapEntry : this->plans) { // 올바른 map 순회 방식
        std::vector<Plan>& planList = mapEntry.second;
        for (auto& plan : planList) {
            Task* task = plan.getTask(taskIdToComplete);
            if (task) {
                task->markComplete();
                return; // Task found and completed, exit function
            }
        }
    }
    std::cerr << "Error: Task with ID " << taskIdToComplete << " not found in any plan." << std::endl;
}

const std::map<std::time_t, std::vector<Plan>>& UserPlanService::getPlans() const {
    return this->plans;
}