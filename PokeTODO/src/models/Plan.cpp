#include "../models/Plan.h"
#include <algorithm> // std::remove_if

Plan::Plan(const std::string& id, const std::time_t& planDate, const std::string& planTitle)
    : planId(id), date(planDate), title(planTitle) {}

void Plan::addTask(const Task& task) {
    this->tasks.push_back(task);
}

void Plan::removeTask(const std::string& taskIdToRemove) {
    this->tasks.erase(
        std::remove_if(this->tasks.begin(), this->tasks.end(), 
                       [&taskIdToRemove](const Task& task) {
                           return task.getTaskId() == taskIdToRemove;
                       }),
        this->tasks.end()
    );
}

Task* Plan::getTask(const std::string& taskIdToFind) {
    for (auto& task : this->tasks) {
        if (task.getTaskId() == taskIdToFind) {
            return &task;
        }
    }
    return nullptr; // 찾지 못한 경우
}

// 나머지 Getter 및 Setter는 헤더 파일에 정의된 대로 사용하거나 필요시 여기에 구현합니다.
std::vector<Task>& Plan::getTasks() {
    return this->tasks;
}

void Plan::setTitle(const std::string& newTitle) {
    this->title = newTitle;
}

// Additional Getters
std::string Plan::getPlanId() const {
    return planId;
}

std::time_t Plan::getDate() const {
    return date;
}

std::string Plan::getTitle() const {
    return title;
}
