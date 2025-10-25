#pragma once
#include <string>
#include <vector>
#include <cctype>
#include "services/UserPlanService.h"
#include "services/UserPointService.h"
#include "services/UserTamagotchiService.h"
#include "services/FileIOService.h"
#include "models/Tamagotchi.h"
#include "ui/UIState.h"

class PokeTodoApp {
private:
    // 서비스들
    UserPlanService planService;
    UserPointService pointService;
    UserTamagotchiService tamagotchiService;
    Tamagotchi::Tamagotchi* myPet;

    // UI 상태 관련
    UiState currentAppState;
    std::vector<std::string> menuItems;
    std::vector<std::string> tamagotchiItems;
    std::vector<std::string> rightPanelDynamicContent;
    std::string inputPrompt;
    int selectedItem;
    bool running;

    // 컨텍스트 변수들
    std::string currentPlanIdContext;
    std::string currentTaskIdContext;
    TempTaskData tempTaskData;
    int selectedCalendarDay;
    int selectedPlanIndex;
    int selectedTaskIndex;
    int selectedTamagotchiIndex;

    // 초기화 메소드들
    void initializeServices();
    void initializeUI();
    void createSampleData();

    // 파일 저장/불러오기 메소드들
    void loadTamagotchiFromFile();
    void saveTamagotchiToFile();

    // 입력 처리 메소드들
    void handleArrowKeys(int key);
    void handleEnterKey();
    void handleEscapeKey();

    // 메뉴 액션 처리 메소드들
    void processMainMenuSelection();
    void processCalendarView();
    void processPlanSelection();
    void processTaskSelection();
    void processTamagotchiMode();
    void processTamagotchiAction();

    // 작업 추가를 위한 입력 처리 메소드들
    void processTaskTitleInput();
    void processTaskPriorityInput();

    // 작업 삭제 더블 체크 메소드
    void processDoubleCheck();

public:
    PokeTodoApp();
    ~PokeTodoApp();

    void run();
};
