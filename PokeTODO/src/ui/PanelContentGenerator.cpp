#include "PanelContentGenerator.h"
#include "../utils/UIUtils.h"

std::vector<std::string> PanelContentGenerator::getDefaultRightPanelContent() {
    return {
        u8"   [PokeTODO에 오신 것을 환영합니다!]",
        u8"",
        u8"   무엇을 도와드릴까요?",
        u8"   왼쪽 메뉴에서 작업을 선택하세요.",
        u8"",
    };
}

std::vector<std::string> PanelContentGenerator::getPlanListStringsForPanel(UserPlanService& planService, const std::string& instruction, int selectedPlanIndex) {
    std::vector<std::string> content;
    content.push_back(u8"--- 플랜 목록 ---");
    const auto& plans = planService.getPlans();
    if (plans.empty()) {
        content.push_back(u8"  표시할 플랜이 없습니다.");
    }
    else {
        int planIndex = 0;
        for (const auto& pair : plans) {
            const std::vector<Plan>& planlist = pair.second;
            for (const auto& plan : planlist) {
                std::string prefix = (planIndex == selectedPlanIndex) ? u8" > " : u8"   ";
                content.push_back(prefix + plan.getTitle());
                planIndex++;
            }
        }
    }
    content.push_back(u8"");
    if (!instruction.empty()) {
        content.push_back(instruction);
    }
    return content;
}

std::vector<std::string> PanelContentGenerator::getTaskListStringsForPanel(Plan* plan, const std::string& instruction, int selectedTaskIndex) {
    std::vector<std::string> content;
    if (!plan) {
        content.push_back(u8"  플랜이 유효하지 않습니다.");
        if (!instruction.empty()) content.push_back(instruction);
        return content;
    }
    content.push_back(u8"--- '" + plan->getTitle() + u8"'의 작업 목록 ---");
    const std::vector<Task>& tasks = plan->getTasks();
    if (tasks.empty()) {
        content.push_back(u8"  표시할 작업이 없습니다.");
    }
    else {
        for (int i = 0; i < static_cast<int>(tasks.size()); ++i) {
            const auto& task = tasks[i];
            std::string prefix = (i == selectedTaskIndex) ? u8" > " : u8"   ";
            content.push_back(prefix + u8"[" + task.getTaskId() + u8"] " + task.getTitle() +
                u8" (우선순위: " + priorityToString(task.getPriority()) +
                u8", 상태: " + statusToString(task.getStatus()) + u8")");
        }
    }
    content.push_back(u8"");
    if (!instruction.empty()) {
        content.push_back(instruction);
    }
    return content;
}

std::vector<std::string> PanelContentGenerator::getCalendarStringsForPanel(int selectedDay) {
    std::vector<std::string> content;
    content.push_back(u8"        2025년 6월");
    content.push_back(u8"");
    content.push_back(u8" 일  월  화  수  목  금  토");
    
    // 각 주별로 날짜 표시 (선택된 날짜는 > < 로 강조)
    // 6월 1일은 일요일
    std::string week1 = u8" ";
    for (int day = 1; day <= 7; ++day) {
        if (day == selectedDay) {
            week1 += u8">" + std::to_string(day) + u8"<";
        } else {
            week1 += (day < 10 ? u8" " : u8"") + std::to_string(day) + u8" ";
        }
        if (day < 7) week1 += u8" ";
    }
    content.push_back(week1);
    
    std::string week2 = u8" ";
    for (int day = 8; day <= 14; ++day) {
        if (day == selectedDay) {
            week2 += u8">" + std::to_string(day) + u8"<";
        } else {
            week2 += std::to_string(day) + u8" ";
        }
        if (day < 14) week2 += u8" ";
    }
    content.push_back(week2);
    
    std::string week3 = u8" ";
    for (int day = 15; day <= 21; ++day) {
        if (day == selectedDay) {
            week3 += u8">" + std::to_string(day) + u8"<";
        } else {
            week3 += std::to_string(day) + u8" ";
        }
        if (day < 21) week3 += u8" ";
    }
    content.push_back(week3);
    
    std::string week4 = u8" ";
    for (int day = 22; day <= 28; ++day) {
        if (day == selectedDay) {
            week4 += u8">" + std::to_string(day) + u8"<";
        } else {
            week4 += std::to_string(day) + u8" ";
        }
        if (day < 28) week4 += u8" ";
    }
    content.push_back(week4);
    
    std::string week5 = u8" ";
    for (int day = 29; day <= 30; ++day) {
        if (day == selectedDay) {
            week5 += u8">" + std::to_string(day) + u8"<";
        } else {
            week5 += std::to_string(day) + u8" ";
        }
        if (day < 30) week5 += u8" ";
    }
    content.push_back(week5);
    
    content.push_back(u8"");
    content.push_back(u8"선택된 날짜: " + std::to_string(selectedDay) + u8"일");
    content.push_back(u8"방향키로 날짜 선택, 엔터로 확인");
    content.push_back(u8"ESC로 메뉴로 돌아가기");
    return content;
} 