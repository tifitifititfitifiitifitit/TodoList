#pragma once
#include <string>
#include <vector>
#include <ctime>
#include "Task.h"

class Plan {
private:
    std::string planId;
    std::time_t date;
    std::string title;
    std::vector<Task> tasks;

public:
    Plan(const std::string& id, const std::time_t& planDate, const std::string& planTitle);

    // Task management
    void addTask(const Task& task);
    void removeTask(const std::string& taskId);
    Task* getTask(const std::string& taskId);
    std::vector<Task>& getTasks();

    // Getters
    std::string getPlanId() const;
    std::time_t getDate() const;
    std::string getTitle() const;

    // Setters
    void setTitle(const std::string& newTitle);
}; 