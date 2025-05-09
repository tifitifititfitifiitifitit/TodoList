#include "TaskCreateRequest.h"

TaskCreateRequest::TaskCreateRequest(
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

std::string TaskCreateRequest::getTitle() const {
    return title;
}

std::string TaskCreateRequest::getDescription() const {
    return description;
}

Priority TaskCreateRequest::getPriority() const {
    return priority;
}

std::time_t TaskCreateRequest::getDeadline() const {
    return deadline;
}

const std::vector<std::string>& TaskCreateRequest::getTags() const {
    return tags;
} 