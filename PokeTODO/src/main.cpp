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

int main() {

    // Create UserPlanService instance
    UserPlanService planService;

    // Create Plan with current time
    std::time_t currentTime = std::time(0);
    std::string planTitle = "Today\'s Tasks"; // "오늘의 할 일"
    Plan myPlan = planService.createPlan(currentTime, planTitle);
    std::cout << "Plan created: " << myPlan.getPlanId() << " - " << myPlan.getTitle() << std::endl;

    // Create TaskCreateRequest objects
    TaskCreateRequest taskReq1(
        "Study C++", // "C++ 공부하기"
        "Implement and test UserPlanService", // "UserPlanService 구현 및 테스트"
        Priority::HIGH, 
        currentTime + 3600, // Deadline: 1 hour later
        {"programming", "cpp", "todo"}
    );

    TaskCreateRequest taskReq2(
        "Grocery Shopping", // "저녁 장보기"
        "Buy milk, eggs, bread", // "우유, 계란, 빵 구입"
        Priority::MEDIUM, 
        currentTime + 7200, // Deadline: 2 hours later
        {"shopping", "food"}
    );

    // Add Tasks to Plan
    Task task1 = planService.addTaskToPlan(myPlan.getPlanId(), taskReq1);
    if (task1.getTaskId() != "error-task") { // Check if addTaskToPlan had an error
        std::cout << "  Task added: " << task1.getTaskId() << " - " << task1.getTitle() 
                  << " (Status: " << statusToString(task1.getStatus()) << ")" << std::endl;
    }

    Task task2 = planService.addTaskToPlan(myPlan.getPlanId(), taskReq2);
    if (task2.getTaskId() != "error-task") {
        std::cout << "  Task added: " << task2.getTaskId() << " - " << task2.getTitle() 
                  << " (Status: " << statusToString(task2.getStatus()) << ")" << std::endl;
    }
    std::cout << std::endl;

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

    std::string input;
    system("cls");
    while (1) {
        std::getline(std::cin, input);
        
        if (input == "help") {
            std::cout << "  new plan\t";
            std::cout << "라라라라" << std::endl;
            std::cout << "  plan list\tshow all plans" << std::endl;
            std::cout << "  add task\tadd task to plan" << std::endl;
            std::cout << "  task list\tshow task list" << std::endl;
            std::cout << "  complete task\tcomplete the task" << std::endl;
            std::cout << std::endl;
        }
        else if (input == "new plan") { //완성
            std::string title;
            std::cout << "plz input title: ";
            std::cin >> title;
            planService.createPlan(currentTime, title);
            std::cout << "Plan created! (plan name : " << title << ")\n\n";
            std::cin.ignore();
        }
        else if (input == "plan list") { //완성
            const auto& plans = planService.getPlans();
            for (const auto& pair : plans) {
                const std::vector<Plan>& planlist = pair.second;
                for (const auto& plan : planlist) {
                    std::cout << "  Plan ID: " << plan.getPlanId()
                        << ", Title: " << plan.getTitle() << std::endl;
                }
                std::cout << std::endl;
            }
        }
        else if (input == "task list") { //미완
            // Print all Tasks in the Plan
            Plan* retrievedPlan = planService.getPlan(myPlan.getPlanId());
            if (retrievedPlan) {
                std::cout << "Current tasks in '" << retrievedPlan->getTitle() << "':" << std::endl;
                for (const auto& task : retrievedPlan->getTasks()) {
                    std::cout << "  - " << task.getTaskId() << ": " << task.getTitle()
                        << " (Priority: " << priorityToString(task.getPriority())
                        << ", Status: " << statusToString(task.getStatus()) << ")" << std::endl;
                }
                std::cout << std::endl;
            }
        }
        else if (input == "complete task") { //오류
            std::string taskID;
            std::cout << "plz input taskID: ";
            std::cin >> taskID;

            Task* retrievedTask = myPlan.getTask(task1.getTaskId());

            //아래 두 줄 수정해야함
            //std::cout << "Completing Task '" << retrievedTask->getTaskId() << "'..." << std::endl << std::endl;
            //planService.completeTask(task1.getTaskId()); 
            std::cin.ignore();
        }
        else if (input == "break") break;
        else {
            std::cout << "error\n\n";
        }
    }
    return 0;
} 