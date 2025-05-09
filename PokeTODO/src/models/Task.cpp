#include "Task.h"
#include "TaskCreateRequest.h"
#include "TaskUpdateRequest.h"

Task::Task(const std::string& id, const TaskCreateRequest& request)
    : taskId(id), 
      title(request.getTitle()), 
      description(request.getDescription()), 
      priority(request.getPriority()), 
      deadline(request.getDeadline()), 
      status(TaskStatus::PENDING), // 기본 상태는 PENDING
      tags(request.getTags()) {}

void Task::markComplete() {
    this->status = TaskStatus::COMPLETED;
}

void Task::updateFromRequest(const TaskUpdateRequest& request) {
    // 요청 객체의 각 필드가 실제로 변경 사항을 가지고 있는지 확인하는 로직이 필요할 수 있으나,
    // 여기서는 모든 필드를 요청 값으로 업데이트한다고 가정합니다.
    // 실제 애플리케이션에서는 선택적 업데이트를 고려해야 합니다.
    this->title = request.getTitle();
    this->description = request.getDescription();
    this->priority = request.getPriority();
    this->deadline = request.getDeadline();
    this->tags = request.getTags();
}

// Getters
std::string Task::getTaskId() const {
    return taskId;
}

std::string Task::getTitle() const {
    return title;
}

std::string Task::getDescription() const {
    return description;
}

Priority Task::getPriority() const {
    return priority;
}

std::time_t Task::getDeadline() const {
    return deadline;
}

TaskStatus Task::getStatus() const {
    return status;
}

const std::vector<std::string>& Task::getTags() const {
    return tags;
}

// Setter 및 Setter 구현 (헤더에 이미 선언된 것 외 필요한 경우)
// 예: std::string Task::getTaskId() const { return taskId; }
// 헤더 파일에 이미 const 멤버 함수로 정의되어 있으므로 중복 구현 필요 없음.
// 나머지 getter들도 마찬가지입니다.
// Setter들은 헤더에 선언된 대로 구현하면 됩니다. (예시)
void Task::setTitle(const std::string& newTitle) {
    this->title = newTitle;
}

void Task::setDescription(const std::string& newDescription) {
    this->description = newDescription;
}

void Task::setPriority(Priority newPriority) {
    this->priority = newPriority;
}

void Task::setDeadline(const std::time_t& newDeadline) {
    this->deadline = newDeadline;
}

void Task::setStatus(TaskStatus newStatus) {
    this->status = newStatus;
}

void Task::setTags(const std::vector<std::string>& newTags) {
    this->tags = newTags;
} 