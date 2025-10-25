#include "PanelContentGenerator.h"
#include "../utils/UIUtils.h"

// ----- cpp 내부에서만 사용하는 달력 공백 맞춤 함수 -----
namespace {
    std::string formatDay(int day, int selectedDay) {
        std::string dayStr;
        if (day == selectedDay) {
            dayStr = (day < 10 ? " >" : ">") + std::to_string(day) + "<";
            return dayStr;
        }
        else dayStr = (day < 10 ? "  " : " ") + std::to_string(day); // 폭 맞춤
        return dayStr + " "; // 숫자 칸 사이 공백 1칸
    }

    int getDisplayWidth(const std::string& str) {
        int width = 0;
        for (size_t i = 0; i < str.size();) {
            unsigned char c = (unsigned char)str[i];
            int charWidth = 0;

            if (c < 128) {
                charWidth = 1;
                i += 1;
            }
            else if ((c & 0xE0) == 0xC0) {
                // UTF-8 2바이트 문자
                charWidth = 2;
                i += 2;
            }
            else if ((c & 0xF0) == 0xE0) {
                // UTF-8 3바이트 문자, 보통 한글
                charWidth = 2;
                i += 3;
            }
            else if ((c & 0xF8) == 0xF0) {
                // UTF-8 4바이트 문자
                charWidth = 2;
                i += 4;
            }
            else {
                charWidth = 1;
                i += 1;
            }

            width += charWidth;
        }
        return width;
    }

    // 가운데 맞춤 함수 (실제 출력 폭 기준)
    std::string padCenter(const std::string& str, int totalWidth, char paddingChar = ' ') {
        int strWidth = getDisplayWidth(str);
        if (strWidth >= totalWidth) return str;

        int totalPadding = totalWidth - strWidth;
        int padLeft = totalPadding / 2;
        int padRight = totalPadding - padLeft;

        return std::string(padLeft, paddingChar) + str + std::string(padRight, paddingChar);
    }
}

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
    content.push_back(u8"  --- 플랜 목록 ---");
    const auto& plans = planService.getPlans();
    if (plans.empty()) {
        content.push_back(u8"  표시할 플랜이 없습니다.");
    }
    else {
        int planIndex = 0;
        for (const auto& pair : plans) {
            const std::vector<Plan>& planlist = pair.second;
            for (const auto& plan : planlist) {
                std::string prefix = (planIndex == selectedPlanIndex) ? u8"  > " : u8"    ";
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
    content.push_back(u8"  --- '" + plan->getTitle() + u8"'의 작업 목록 ---");
    const std::vector<Task>& tasks = plan->getTasks();
    if (tasks.empty()) {
        content.push_back(u8"  표시할 작업이 없습니다.");
    }
    else {
        int taskListNum = 1;
        for (int i = 0; i < static_cast<int>(tasks.size()); ++i) {
            const auto& task = tasks[i];
            std::string prefix = (i == selectedTaskIndex) ? u8"  > " : u8"    ";
            content.push_back(prefix + u8"[" + std::to_string(taskListNum) + u8"] " + task.getTitle() +
                u8" (우선순위: " + priorityToString(task.getPriority()) +
                u8", 상태: " + statusToString(task.getStatus()) + u8")");
            taskListNum += 1;
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
    content.push_back(u8"          2025년 6월");
    content.push_back(u8"");
    content.push_back(u8"   일  월  화  수  목  금  토");

    // 각 주별 날짜
    for (int weekStart = 1; weekStart <= 31; weekStart += 7) {
        std::string week = "  ";
        for (int day = weekStart; day < weekStart + 7 && day <= 31; ++day) {
            week += formatDay(day, selectedDay);    // 공백 처리를 위해 해당 cpp파일에 새로운 함수를 만들었습니다
        }
        content.push_back(week);
    }

    /*
    // 기존에 사용하던 코드
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
    */
    
    content.push_back(u8"");
    content.push_back(u8"  선택된 날짜: " + std::to_string(selectedDay) + u8"일");
    content.push_back(u8"  방향키로 날짜 선택, 엔터로 확인");
    content.push_back(u8"  ESC로 메뉴로 돌아가기");
    return content;
}
std::vector<std::string> PanelContentGenerator::getTamagotchiMenu(Tamagotchi::Tamagotchi* myPet, UserPointService& pointService, int selectedTamagotchiItem)
{
    std::vector<std::string> content;
    content.push_back("  Lv." + std::to_string(myPet->getLevel()) + "  " + myPet->getName());
    content.push_back(u8"");
    content.push_back(
        padCenter(u8"행복도", 10) +
        padCenter(u8"배고픔", 10) +
        padCenter(u8"포인트", 10) +
        padCenter(u8"상태", 10));
    content.push_back(
        padCenter(std::to_string(myPet->getHappiness()), 10) +
        padCenter(std::to_string(myPet->getHunger()), 10) +
        padCenter(std::to_string(pointService.getPoints()), 10) +
        padCenter(myPet->getCurrentStateName(), 10));
    content.push_back(u8"");
    //content.push_back(u8"");
    //content.push_back(u8"--------------------------------------------------");
    
    std::vector<std::string> items = { u8"먹이주기", u8"놀아주기", u8"돌아가기" };
    for (int i = 0; i < items.size(); i++) {
        if (i == selectedTamagotchiItem) {
            content.push_back(u8"  > [" + items[i] + "]");
        }
        else {
            content.push_back(u8"    [" + items[i] + "]");
        }
    }
    


    return content;
}

