#pragma once
#include <string>
#include <vector>
#include "../services/UserPlanService.h"
#include "../models/Plan.h"
#include "../services/UserTamagotchiService.h"
#include "../models/Tamagotchi.h"
#include "../services/UserPointService.h"


class PanelContentGenerator {
public:
    // 오른쪽 패널에 표시될 기본 내용을 반환하는 함수
    static std::vector<std::string> getDefaultRightPanelContent();
    
    // 플랜 목록을 위한 패널 컨텐츠 생성
    static std::vector<std::string> getPlanListStringsForPanel(
        UserPlanService& planService, 
        const std::string& instruction = u8"  엔터 키를 누르면 메뉴로 돌아갑니다.", 
        int selectedPlanIndex = -1);
    
    // 작업 목록을 위한 패널 컨텐츠 생성
    static std::vector<std::string> getTaskListStringsForPanel(
        Plan* plan, 
        const std::string& instruction = u8"  엔터 키를 누르면 메뉴로 돌아갑니다.", 
        int selectedTaskIndex = -1);
    
    // 2025년 5월 캘린더 생성 함수
    static std::vector<std::string> getCalendarStringsForPanel(int selectedDay = 1);

    static std::vector<std::string> getTamagotchiMenu(
        Tamagotchi::Tamagotchi* myPet,
        UserPointService& pointService,
        int selectedTamagotchiItem
    );
}; 
