#include "PlanUtils.h"

int PlanUtils::getPlanCount(UserPlanService& planService) {
    int count = 0;
    const auto& plans = planService.getPlans();
    for (const auto& pair : plans) {
        count += static_cast<int>(pair.second.size());
    }
    return count;
}

Plan* PlanUtils::getPlanByIndex(UserPlanService& planService, int index) {
    int currentIndex = 0;
    const auto& plans = planService.getPlans();
    for (const auto& pair : plans) {
        const std::vector<Plan>& planlist = pair.second;
        for (const auto& plan : planlist) {
            if (currentIndex == index) {
                return const_cast<Plan*>(&plan);
            }
            currentIndex++;
        }
    }
    return nullptr;
} 