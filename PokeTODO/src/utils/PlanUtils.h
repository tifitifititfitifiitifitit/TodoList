#pragma once
#include "../services/UserPlanService.h"
#include "../models/Plan.h"

class PlanUtils {
public:
    // 플랜 개수를 반환하는 함수
    static int getPlanCount(UserPlanService& planService);
    
    // 인덱스로 플랜을 가져오는 함수
    static Plan* getPlanByIndex(UserPlanService& planService, int index);
}; 