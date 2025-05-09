#pragma once
#include <string>
#include <vector>
#include <ctime>

// Enum 정의를 클래스 선언 위로 이동
enum class Priority { LOW, MEDIUM, HIGH };
enum class TaskStatus { PENDING, IN_PROGRESS, COMPLETED };

// TaskCreateRequest 와 TaskUpdateRequest 에 대한 전방 선언을 추가하여
// Task 클래스 선언 시 해당 타입들을 알 수 있도록 합니다.
class TaskCreateRequest;
class TaskUpdateRequest;

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
    // 생성자의 매개변수 타입이 완전한 정의를 요구하므로, 
    // 생성자 선언은 TaskCreateRequest/TaskUpdateRequest 정의 이후로 옮기거나
    // .cpp 파일에만 정의를 남기는 것이 좋습니다.
    // 여기서는 헤더에 선언을 유지하되, TaskCreateRequest.h 등이 먼저 include 되도록 합니다.
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