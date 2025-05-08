#include "../models/Plan.h"
#include "../models/Task.h"
#include "../models/TaskCreateRequest.h"
#include "../services/UserPlanService.h"

// UserPlanService.h

#ifndef USERPLANSERVICE_H
#define USERPLANSERVICE_H

class UserPlan {
public:
    UserPlan();
    // 생성자
    UserPlan(int id, const std::string& name, const std::string& date);

    // 계획에 Task 추가
    void addTask(const Task& task);

    // Task 완료 처리
    bool completeTask(int taskId);

    // getter
    int getId() const;
    std::string getName() const;
    std::string getDate() const;

    // 계획 내의 모든 Task 조회
    std::vector<Task> getTasks() const;

private:
    int planId;  // 계획 ID
    std::string planName;  // 계획 이름
    std::string planDate;  // 계획 날짜
    std::vector<Task> tasks;  // 계획에 포함된 Task들
};

class UserPlanService {
public:
    UserPlanService();  // 생성자
    ~UserPlanService(); // 소멸자

    // 계획 생성
    bool createPlan(const UserPlan& plan);

    // 계획 조회
    UserPlan getPlan(int planId) const;

    // 계획에 Task 추가
    bool addTaskToPlan(int planId, const Task& task);

    // Task 완료 처리
    bool completeTask(int planId, int taskId);

private:
    std::vector<UserPlan> plans;  // 계획들을 저장할 벡터
    int planCounter;  // 계획 ID를 관리하기 위한 카운터
};

#endif // USERPLANSERVICE_H
// UserPlanService.cpp

#include "UserPlanService.h"

// 생성자
UserPlanService::UserPlanService() : planCounter(0) {}

// 소멸자
UserPlanService::~UserPlanService() {}

// 계획 생성
bool UserPlanService::createPlan(const UserPlan& plan) {
    plans.push_back(plan);
    planCounter++; // 계획 ID 증가
    return true; // 계획 생성 성공
}

// 계획 조회
UserPlan UserPlanService::getPlan(int planId) const {
    for (const auto& plan : plans) {
        if (plan.getId() == planId) {
            return plan; // 계획 ID에 해당하는 계획 반환
        }
    }
    // 계획이 없으면 예외 처리 (빈 계획 객체 반환)
    return UserPlan();
}

// 계획에 Task 추가
bool UserPlanService::addTaskToPlan(int planId, const Task& task) {
    for (auto& plan : plans) {
        if (plan.getId() == planId) {
            plan.addTask(task); // 계획에 Task 추가
            return true;
        }
    }
    return false; // 계획을 찾을 수 없으면 실패
}

// Task 완료 처리
bool UserPlanService::completeTask(int planId, int taskId) {
    for (auto& plan : plans) {
        if (plan.getId() == planId) {
            return plan.completeTask(taskId); // 계획 내에서 Task 완료 처리
        }
    }
    return false; // 계획을 찾을 수 없으면 실패
}