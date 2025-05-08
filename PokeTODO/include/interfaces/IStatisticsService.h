#pragma once
#include <string>
#include <map>
#include "../models/User.h"

class IStatisticsService {
public:
    virtual ~IStatisticsService() = default;
    virtual std::map<std::string, double> calculateStats(const User& user) = 0;
    virtual std::string generateReport(const User& user) = 0;
    virtual bool checkLowPerformance(const std::map<std::string, double>& stats) = 0;
}; 

//오우오우오우