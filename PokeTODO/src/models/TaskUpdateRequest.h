#pragma once
#include <string>
#include <vector>
#include <ctime>
#include "Task.h"

class TaskUpdateRequest {
private:
    std::string title;
    std::string description;
    Priority priority;
    std::time_t deadline;
    std::vector<std::string> tags;

public:
    TaskUpdateRequest(
        const std::string& taskTitle,
        const std::string& taskDescription,
        Priority taskPriority,
        const std::time_t& taskDeadline,
        const std::vector<std::string>& taskTags
    );

    // Getters
    std::string getTitle() const;
    std::string getDescription() const;
    Priority getPriority() const;
    std::time_t getDeadline() const;
    const std::vector<std::string>& getTags() const;
}; 