#include "TaskUpdateRequest.h"

TaskUpdateRequest::TaskUpdateRequest(
    const std::string& taskTitle,
    const std::string& taskDescription,
    Priority taskPriority,
    const std::time_t& taskDeadline,
    const std::vector<std::string>& taskTags
) : title(taskTitle),
    description(taskDescription),
    priority(taskPriority),
    deadline(taskDeadline),
    tags(taskTags) {}

std::string TaskUpdateRequest::getTitle() const {
    return title;
}

std::string TaskUpdateRequest::getDescription() const {
    return description;
}

Priority TaskUpdateRequest::getPriority() const {
    return priority;
}

std::time_t TaskUpdateRequest::getDeadline() const {
    return deadline;
}

const std::vector<std::string>& TaskUpdateRequest::getTags() const {
    return tags;
} 