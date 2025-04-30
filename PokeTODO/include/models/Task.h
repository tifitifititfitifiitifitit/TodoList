#pragma once
#include <string>
#include <vector>
#include <ctime>
#include "TaskCreateRequest.h"
#include "TaskUpdateRequest.h"

enum class Priority { LOW, MEDIUM, HIGH };
enum class TaskStatus { PENDING, IN_PROGRESS, COMPLETED };

class Task {
private:
    std::string taskId;
    std::string title;
    std::string description;
    Priority priority;
    std::time_t deadline;
    TaskStatus status;
    std::vector<std::string> tags;

public:
    Task(const std::string& id, const TaskCreateRequest& request);

    // Status management
    void markComplete();
    void updateFromRequest(const TaskUpdateRequest& request);

    // Getters
    std::string getTaskId() const;
    std::string getTitle() const;
    std::string getDescription() const;
    Priority getPriority() const;
    std::time_t getDeadline() const;
    TaskStatus getStatus() const;
    const std::vector<std::string>& getTags() const;

    // Setters
    void setTitle(const std::string& newTitle);
    void setDescription(const std::string& newDescription);
    void setPriority(Priority newPriority);
    void setDeadline(const std::time_t& newDeadline);
    void setStatus(TaskStatus newStatus);
    void setTags(const std::vector<std::string>& newTags);
}; 