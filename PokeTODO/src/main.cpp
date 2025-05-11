#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "services/UserPlanService.h" // UserPlanService.h 경로
#include "models/TaskCreateRequest.h" // TaskCreateRequest.h 경로
#include "models/Task.h"              // Task.h 경로 (TaskStatus, Priority enum 사용)

// Helper functions to convert TaskStatus and Priority to string
std::string statusToString(TaskStatus status) {
    switch (status) {
        case TaskStatus::PENDING: return "PENDING";
        case TaskStatus::IN_PROGRESS: return "IN_PROGRESS";
        case TaskStatus::COMPLETED: return "COMPLETED";
        default: return "UNKNOWN";
    }
}

std::string priorityToString(Priority priority) {
    switch (priority) {
        case Priority::LOW: return "LOW";
        case Priority::MEDIUM: return "MEDIUM";
        case Priority::HIGH: return "HIGH";
        default: return "UNKNOWN";
    }
}

void printPlanList(UserPlanService planService) {
    const auto& plans = planService.getPlans();
    int planNumber = 1;
    for (const auto& pair : plans) {
        const std::vector<Plan>& planlist = pair.second;
        for (const auto& plan : planlist) {
            std::cout << "  [" << planNumber << "] Plan ID: " << plan.getPlanId()
                        << ", Title: " << plan.getTitle() << std::endl;
        }
        planNumber += 1;
    }
    std::cout << std::endl;
}

void printTaskList(Plan* plan) {
    const std::vector<Task> tasks = plan->getTasks();
    int taskNumber = 1;
    for (const auto& task : tasks) {
        std::cout << "  [" << taskNumber << "] Task ID: " << task.getTaskId()
            << ", Title: " << task.getTitle() << " (Priority: " << priorityToString(task.getPriority())
            << ", Status: " << statusToString(task.getStatus()) << ")" << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    // Create UserPlanService instance
    UserPlanService planService;

    // Create Plan with current time
    std::time_t currentTime = std::time(0);
    std::string planTitle = "Today\'s Tasks"; // "오늘의 할 일"
    Plan myPlan = planService.createPlan(currentTime, planTitle);
    //std::cout << "Plan created: " << myPlan.getPlanId() << " - " << myPlan.getTitle() << std::endl;

    // Create TaskCreateRequest objects
    TaskCreateRequest taskReq1(
        "Study C++", // "C++ 공부하기"
        "Implement and test UserPlanService", // "UserPlanService 구현 및 테스트"
        Priority::HIGH,
        currentTime + 3600, // Deadline: 1 hour later
        { "programming", "cpp", "todo" }
    );

    TaskCreateRequest taskReq2(
        "Grocery Shopping", // "저녁 장보기"
        "Buy milk, eggs, bread", // "우유, 계란, 빵 구입"
        Priority::MEDIUM,
        currentTime + 7200, // Deadline: 2 hours later
        { "shopping", "food" }
    );

    // Add Tasks to Plan
    Task task1 = planService.addTaskToPlan(myPlan.getPlanId(), taskReq1);
    Task task2 = planService.addTaskToPlan(myPlan.getPlanId(), taskReq2);
    /*
    if (task1.getTaskId() != "error-task") { // Check if addTaskToPlan had an error
        std::cout << "  Task added: " << task1.getTaskId() << " - " << task1.getTitle()
                  << " (Status: " << statusToString(task1.getStatus()) << ")" << std::endl;
    }
    if (task2.getTaskId() != "error-task") {
        std::cout << "  Task added: " << task2.getTaskId() << " - " << task2.getTitle()
                  << " (Status: " << statusToString(task2.getStatus()) << ")" << std::endl;
    }

    // Print all Tasks in the Plan
    Plan* retrievedPlan = planService.getPlan(myPlan.getPlanId());
    if (retrievedPlan) {
        std::cout << "Current tasks in '" << retrievedPlan->getTitle() << "':" << std::endl;
        for (const auto& task : retrievedPlan->getTasks()) {
            std::cout << "  - " << task.getTaskId() << ": " << task.getTitle()
                      << " (Priority: " << priorityToString(task.getPriority())
                      << ", Status: " << statusToString(task.getStatus()) << ")" << std::endl;
        }
    }
    std::cout << std::endl;

    // Complete a Task
    std::cout << "Completing Task '" << task1.getTaskId() << "'..." << std::endl;
    planService.completeTask(task1.getTaskId());

    // Check updated Task status
    Task* completedTaskCheck = retrievedPlan->getTask(task1.getTaskId());
    if (completedTaskCheck) {
        std::cout << "Final status of Task '" << completedTaskCheck->getTaskId() << "': "
                  << statusToString(completedTaskCheck->getStatus()) << std::endl;
    }
    std::cout << std::endl;

    // Print all Tasks in the Plan again (to see status change)
    if (retrievedPlan) {
        std::cout << "Final task list for '" << retrievedPlan->getTitle() << "':" << std::endl;
        for (const auto& task : retrievedPlan->getTasks()) {
            std::cout << "  - " << task.getTaskId() << ": " << task.getTitle()
                      << " (Priority: " << priorityToString(task.getPriority())
                      << ", Status: " << statusToString(task.getStatus()) << ")" << std::endl;
        }
    }
    */

    std::string input;
    while (1) {
        std::getline(std::cin, input);

        if (input == "help") {
            std::cout << "  plan list\t\tshow all plans" << std::endl;
            std::cout << "  plan add\t\tadd new plan" << std::endl;
            std::cout << "  plan set title\t\tdelete the plan" << std::endl;
            std::cout << "  task list\t\tshow task list" << std::endl;
            std::cout << "  task add\t\tadd task to plan" << std::endl;
            std::cout << "  task remove\t\tremove task from plan" << std::endl;
            std::cout << "  task complete\t\tcomplete the task" << std::endl;
            std::cout << "  break\t\t\tsystem shutdown" << std::endl;
            std::cout << std::endl;
        }
        else if (input == "plan list") {
            printPlanList(planService);
        }
        else if (input == "plan add") {
            std::string title;
            std::time_t newTime = std::time(0);
            std::cout << "plz input title: " << std::endl;
            std::getline(std::cin, title);
            planService.createPlan(newTime, title);
            std::cout << "Plan created! (plan name : " << title << ")\n\n";
        }
        else if (input == "plan set title") {
            std::string planID, newTitle;
            std::cout << "Type a PlanID to change plan title" << std::endl;
            printPlanList(planService);
            std::cin >> planID;
            
            if (!planService.getPlan(planID)) {
                std::cout << "The selected plan does not exist" << std::endl << std::endl;
                std::cin.ignore();
                continue;
            }

            Plan* selectedPlan = planService.getPlan(planID);
            std::cout << "Type new title: " << std::endl << std::endl;
            std::cin.ignore();
            std::getline(std::cin, newTitle);
            std::cout << "Set title \"" << selectedPlan->getTitle() << "\" to \"" 
                << newTitle <<"\"" << std::endl <<std::endl;
            selectedPlan->setTitle(newTitle);
        }
        else if (input == "task list") {
            std::string planID;
            std::cout << "Type a PlanID to view your task" << std::endl;
            printPlanList(planService);
            std::cin >> planID;

            if (!planService.getPlan(planID)) {
                std::cout << "The selected plan does not exist" << std::endl;
                std::cin.ignore();
                continue;
            }

            Plan* selectedPlan = planService.getPlan(planID);
            std::cout << "Current tasks in '" << selectedPlan->getTitle() << "':" << std::endl;
            printTaskList(selectedPlan);
            std::cin.ignore();
        }
        else if (input == "task add") { // 미완
            std::string planID;
            std::cout << "Type a PlanID to add new task to plan" << std::endl;
            printPlanList(planService);
            std::cin >> planID;

            if (!planService.getPlan(planID)) {
                std::cout << "The selected plan does not exist" << std::endl;
                std::cin.ignore();
                continue;
            }
            Plan* selectedPlan = planService.getPlan(planID);

            // task 정보들 입력받아서 추가하기
            std::cin.ignore();
        }
        else if (input == "task remove") {
            std::string planId, taskId;

            std::cout << "Type a PlanID to remove task from plan" << std::endl;
            printPlanList(planService);
            std::cin >> planId;
            if (!planService.getPlan(planId)) {
                std::cout << "Selected plan does not exist" << std::endl;
                std::cin.ignore();
                continue;
            }
            Plan* selectedPlan = planService.getPlan(planId);

            std::cout << "Type a TaskID to remove your task" << std::endl;
            printTaskList(selectedPlan);
            std::cin >> taskId;
            if (!selectedPlan->getTask(taskId)) {
                std::cout << "Selected plan does not exist" << std::endl;
                std::cin.ignore();
                continue;
            }
            Task* selectedTask = selectedPlan->getTask(taskId);

            std::cout << "Remove Task '" << selectedTask->getTaskId() << "'..." << std::endl << std::endl;
            selectedPlan->removeTask(selectedTask->getTaskId());
            std::cin.ignore();
        }
        else if (input == "task complete") {
            std::string planId, taskId;

            std::cout << "Type a PlanID to complete your task" << std::endl;
            printPlanList(planService);
            std::cin >> planId;
            if (!planService.getPlan(planId)) {
                std::cout << "Selected plan does not exist" << std::endl;
                std::cin.ignore();
                continue;
            }
            Plan* selectedPlan = planService.getPlan(planId);

            std::cout << "Type a TaskID to complete your task" << std::endl;
            printTaskList(selectedPlan);
            std::cin >> taskId;
            if (!selectedPlan->getTask(taskId)) {
                std::cout << "Selected plan does not exist" << std::endl;
                std::cin.ignore();
                continue;
            }
            Task* selectedTask = selectedPlan->getTask(taskId);

            std::cout << "Completing Task '" << selectedTask->getTaskId() << "'..." << std::endl << std::endl;
            planService.completeTask(selectedTask->getTaskId());
            std::cin.ignore();
        }
        else if (input == "break") {
            break;
        }
        else {
            std::cout << "error\n\n";
        }
     }
    return 0;
}