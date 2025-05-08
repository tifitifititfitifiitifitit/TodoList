#pragma once
#include <string>
#include <ctime>

class INotificationService {
public:
    virtual ~INotificationService() = default;
    virtual void sendNotification(const std::string& userId, const std::string& message, const std::time_t& time) = 0;
}; 