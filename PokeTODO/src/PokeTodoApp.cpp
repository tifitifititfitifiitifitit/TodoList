#include "PokeTodoApp.h"
#include "ui/UIManager.h"
#include "ui/PanelContentGenerator.h"
#include "utils/PlanUtils.h"
#include "models/TaskCreateRequest.h"
#include <iostream>
#include <ctime>
#include <conio.h>

PokeTodoApp::PokeTodoApp()
    : currentAppState(UiState::SHOWING_MAIN_MENU)
    , selectedItem(0)
    , running(true)
    , selectedCalendarDay(1)
    , selectedPlanIndex(0)
    , selectedTaskIndex(0)
    , selectedTamagotchiIndex(0)
{
    initializeServices();
    initializeUI();
    createSampleData();
}

PokeTodoApp::~PokeTodoApp() {
    // 다마고치 메모리 해제는 tamagotchiService 소멸자에서 처리됨
}

void PokeTodoApp::initializeServices() {
    tamagotchiItems = {
        u8"먹이주기",
        u8"놀아주기",
        u8"종료"
    };
    selectedTamagotchiIndex = 0;

    // 다마고치 상태 불러오기 시도
    loadTamagotchiFromFile();

    // 만약 불러오기에 실패했다면 새 다마고치 생성
    if (!myPet) {
        myPet = new Tamagotchi::Tamagotchi(1, "피카츄");
        myPet->setName();
        std::cout << u8"새로운 다마고치를 생성했습니다!" << std::endl;
    }

    tamagotchiService.assignTamagotchi(myPet);
}

void PokeTodoApp::initializeUI() {
    menuItems = {
        u8"캘린더 보기",
        u8"작업 목록 보기",
        u8"작업 추가하기",
        u8"작업 제거하기",
        u8"작업 완료하기",
        u8"다마고치 모드",
        u8"종료"
    };

    rightPanelDynamicContent = PanelContentGenerator::getDefaultRightPanelContent();
}

void PokeTodoApp::createSampleData() {
    // 요일별 7개 Plan 생성
    std::vector<std::string> dayNames = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };
    std::vector<Plan> weeklyPlans;

    std::time_t baseTime = std::time(0);
    for (int i = 0; i < 7; ++i) {
        Plan dayPlan = planService.createPlan(baseTime + (i * 86400), dayNames[i]); // 하루씩 증가
        weeklyPlans.push_back(dayPlan);
    }

    // 월요일(Mon)에 샘플 작업 추가
    if (!weeklyPlans.empty() && weeklyPlans[0].getPlanId() != "error-plan") {
        planService.addTaskToPlan(weeklyPlans[0].getPlanId(),
            TaskCreateRequest(u8"C++ UI 개선", u8"아스키 아트와 메뉴 시스템 통합", Priority::HIGH, baseTime + 3600, { u8"개발", u8"UI" }));
        planService.addTaskToPlan(weeklyPlans[0].getPlanId(),
            TaskCreateRequest(u8"저녁 식사 준비", u8"된장찌개 만들기", Priority::MEDIUM, baseTime + 7200, { u8"요리" }));
    }

    // 화요일(Tue)에 샘플 작업 추가
    if (weeklyPlans.size() > 1 && weeklyPlans[1].getPlanId() != "error-plan") {
        planService.addTaskToPlan(weeklyPlans[1].getPlanId(),
            TaskCreateRequest(u8"발표 자료 준비", u8"최종 검토 및 수정", Priority::HIGH, baseTime + 86400, { u8"업무" }));
    }
}

void PokeTodoApp::run() {
    while (running) {
        UIManager::displayMenu(menuItems, selectedItem, rightPanelDynamicContent, inputPrompt, myPet->getCurrentArt());
        inputPrompt = ""; // 사용 후 초기화

        UIManager::clearKeyboardBuffer();
        int key = _getch();

        if (key == 224) {
            key = _getch();
            handleArrowKeys(key);
        }
        else if (key == 13) { // Enter 키
            handleEnterKey();
        }
        else if (key == 27) { // ESC 키
            handleEscapeKey();
        }
        else if (key == 'l' || key == 'L') {    // 개발자 전용 레벨업 단축키
            myPet->levelUp();
            rightPanelDynamicContent = { u8"  " + myPet->getName() + u8"은(는) 이제 " + std::to_string(myPet->getLevel()) + u8"레벨입니다"};
            if (myPet->tryEvolve()) {
                rightPanelDynamicContent.push_back(u8"  " + myPet->getName() + u8"이(가) 진화했습니다!");
                myPet->setName();
            }
            currentAppState = UiState::PROMPT_ENTER_TAMAGOTCHI_MODE;
        }
        else if (key == 'p' || key == 'P') {    // 개발자 전용 포인트 단축키
            pointService.addPoints(10);
        }
    }
}

void PokeTodoApp::handleArrowKeys(int key) {
    switch (currentAppState) {
    case UiState::SHOWING_MAIN_MENU:
        switch (key) {
        case 72: // 위쪽 화살표
            selectedItem = (selectedItem - 1 + static_cast<int>(menuItems.size())) % static_cast<int>(menuItems.size());
            break;
        case 80: // 아래쪽 화살표
            selectedItem = (selectedItem + 1) % static_cast<int>(menuItems.size());
            break;
        }
        break;

    case UiState::VIEWING_CALENDAR:
        switch (key) {
        case 72: // 위쪽 화살표 (일주일 전)
            selectedCalendarDay = (selectedCalendarDay - 7 <= 0) ? selectedCalendarDay + 24 : selectedCalendarDay - 7;
            if (selectedCalendarDay > 31) selectedCalendarDay = 31;
            rightPanelDynamicContent = PanelContentGenerator::getCalendarStringsForPanel(selectedCalendarDay);
            break;
        case 80: // 아래쪽 화살표 (일주일 후)
            selectedCalendarDay = (selectedCalendarDay + 7 > 31) ? selectedCalendarDay - 24 : selectedCalendarDay + 7;
            if (selectedCalendarDay <= 0) selectedCalendarDay = 1;
            rightPanelDynamicContent = PanelContentGenerator::getCalendarStringsForPanel(selectedCalendarDay);
            break;
        case 75: // 왼쪽 화살표 (하루 전)
            selectedCalendarDay = (selectedCalendarDay - 1 <= 0) ? 31 : selectedCalendarDay - 1;
            rightPanelDynamicContent = PanelContentGenerator::getCalendarStringsForPanel(selectedCalendarDay);
            break;
        case 77: // 오른쪽 화살표 (하루 후)
            selectedCalendarDay = (selectedCalendarDay + 1 > 31) ? 1 : selectedCalendarDay + 1;
            rightPanelDynamicContent = PanelContentGenerator::getCalendarStringsForPanel(selectedCalendarDay);
            break;
        }
        break;

    case UiState::SELECTING_PLAN_FOR_TASKS:
    case UiState::SELECTING_PLAN_FOR_ADD_TASK:
    case UiState::SELECTING_PLAN_FOR_REMOVE_TASK:
    case UiState::SELECTING_PLAN_FOR_COMPLETE_TASK:
    {
        int planCount = PlanUtils::getPlanCount(planService);
        if (planCount > 0) {
            switch (key) {
            case 72: // 위쪽 화살표
                selectedPlanIndex = (selectedPlanIndex - 1 + planCount) % planCount;
                rightPanelDynamicContent = PanelContentGenerator::getPlanListStringsForPanel(planService, u8"  방향키로 선택, 엔터로 확인, ESC로 취소", selectedPlanIndex);
                break;
            case 80: // 아래쪽 화살표
                selectedPlanIndex = (selectedPlanIndex + 1) % planCount;
                rightPanelDynamicContent = PanelContentGenerator::getPlanListStringsForPanel(planService, u8"  방향키로 선택, 엔터로 확인, ESC로 취소", selectedPlanIndex);
                break;
            }
        }
    }
    break;

    case UiState::SELECTING_TASK_FOR_REMOVE:
    case UiState::SELECTING_TASK_FOR_COMPLETE:
    {
        Plan* currentPlan = planService.getPlan(currentPlanIdContext);
        if (currentPlan && !currentPlan->getTasks().empty()) {
            int taskCount = static_cast<int>(currentPlan->getTasks().size());

            switch (key) {
            case 72: // 위쪽 화살표
                selectedTaskIndex = (selectedTaskIndex - 1 + taskCount) % taskCount;
                rightPanelDynamicContent = PanelContentGenerator::getTaskListStringsForPanel(currentPlan,
                    (currentAppState == UiState::SELECTING_TASK_FOR_REMOVE) ?
                    u8"  방향키로 작업 선택, 엔터로 제거, ESC로 취소" :
                    u8"  방향키로 작업 선택, 엔터로 완료, ESC로 취소", selectedTaskIndex);
                break;
            case 80: // 아래쪽 화살표
                selectedTaskIndex = (selectedTaskIndex + 1) % taskCount;
                rightPanelDynamicContent = PanelContentGenerator::getTaskListStringsForPanel(currentPlan,
                    (currentAppState == UiState::SELECTING_TASK_FOR_REMOVE) ?
                    u8"  방향키로 작업 선택, 엔터로 제거, ESC로 취소" :
                    u8"  방향키로 작업 선택, 엔터로 완료, ESC로 취소", selectedTaskIndex);
                break;
            }
        }
    }
    break;
    case UiState::SELECTING_TAMAGOTCHI_ACTION:
        Tamagotchi::Tamagotchi* pet = tamagotchiService.getTamagotchi();
        switch (key) {
        case 72: // 위쪽 화살표
            selectedTamagotchiIndex = (selectedTamagotchiIndex - 1 + 3) % 3;
            rightPanelDynamicContent = PanelContentGenerator::getTamagotchiMenu(pet, pointService, selectedTamagotchiIndex);
            break;
        case 80: // 아래쪽 화살표
            selectedTamagotchiIndex = (selectedTamagotchiIndex + 1) % 3;
            rightPanelDynamicContent = PanelContentGenerator::getTamagotchiMenu(pet, pointService, selectedTamagotchiIndex);
            break;
        }
        break;
    }
}

void PokeTodoApp::handleEnterKey() {
    Tamagotchi::Tamagotchi* pet = tamagotchiService.getTamagotchi();
    switch (currentAppState) {
    case UiState::SHOWING_MAIN_MENU:
        processMainMenuSelection();
        break;
    case UiState::VIEWING_CALENDAR:
        processCalendarView();
        break;
    case UiState::SELECTING_PLAN_FOR_TASKS:
    case UiState::SELECTING_PLAN_FOR_ADD_TASK:
    case UiState::SELECTING_PLAN_FOR_REMOVE_TASK:
    case UiState::SELECTING_PLAN_FOR_COMPLETE_TASK:
        processPlanSelection();
        break;
    case UiState::SELECTING_TASK_FOR_REMOVE:
    case UiState::SELECTING_TASK_FOR_COMPLETE:
        processTaskSelection();
        break;
    case UiState::DOUBLE_CHECK_FOR_REMOVE_TASK:
        processDoubleCheck();
        break;
    case UiState::ADDING_TASK_DETAILS_TITLE:
        processTaskTitleInput();
        break;
    case UiState::ADDING_TASK_DETAILS_PRIORITY:
        processTaskPriorityInput();
        break;
    case UiState::PROMPT_ENTER_TAMAGOTCHI_MODE:
        processTamagotchiMode();
        break;
    case UiState::SELECTING_TAMAGOTCHI_ACTION:
        processTamagotchiAction();
        break;
    case UiState::TAMAGOTCHI_ONE_MORE_ENTER:
        processTamagotchiAction();
        break;
    case UiState::QUIT_TAMIGOTCHI_MODE:
        rightPanelDynamicContent = PanelContentGenerator::getDefaultRightPanelContent();;
        currentAppState = UiState::SHOWING_MAIN_MENU;
    case UiState::VIEWING_TASKS:
        currentAppState = UiState::SHOWING_MAIN_MENU;
        rightPanelDynamicContent = PanelContentGenerator::getDefaultRightPanelContent();
        break;
    default:
        currentAppState = UiState::SHOWING_MAIN_MENU;
        rightPanelDynamicContent = PanelContentGenerator::getDefaultRightPanelContent();
        break;
    }
}

void PokeTodoApp::handleEscapeKey() {
    if (currentAppState != UiState::SHOWING_MAIN_MENU) {
        currentAppState = UiState::SHOWING_MAIN_MENU;
        rightPanelDynamicContent = PanelContentGenerator::getDefaultRightPanelContent();
        inputPrompt = "";
    }
    else {
        // 프로그램 종료 시 다마고치 상태 저장
        saveTamagotchiToFile();
        running = false;
        system("cls");
        std::cout << u8"PokeTODO를 종료합니다. 다음에 또 만나요!" << std::endl;
    }
}

void PokeTodoApp::processMainMenuSelection() {
    if (menuItems[selectedItem] == u8"캘린더 보기") {
        currentAppState = UiState::VIEWING_CALENDAR;
        selectedCalendarDay = 1;
        rightPanelDynamicContent = PanelContentGenerator::getCalendarStringsForPanel(selectedCalendarDay);
    }
    else if (menuItems[selectedItem] == u8"작업 목록 보기") {
        selectedPlanIndex = 0;
        rightPanelDynamicContent = PanelContentGenerator::getPlanListStringsForPanel(planService, u8"  방향키로 선택, 엔터로 확인, ESC로 취소", selectedPlanIndex);
        currentAppState = UiState::SELECTING_PLAN_FOR_TASKS;
    }
    else if (menuItems[selectedItem] == u8"작업 추가하기") {
        tempTaskData = {};
        selectedPlanIndex = 0;
        rightPanelDynamicContent = PanelContentGenerator::getPlanListStringsForPanel(planService, u8"  방향키로 선택, 엔터로 확인, ESC로 취소", selectedPlanIndex);
        currentAppState = UiState::SELECTING_PLAN_FOR_ADD_TASK;
    }
    else if (menuItems[selectedItem] == u8"작업 제거하기") {
        selectedPlanIndex = 0;
        rightPanelDynamicContent = PanelContentGenerator::getPlanListStringsForPanel(planService, u8"  방향키로 선택, 엔터로 확인, ESC로 취소", selectedPlanIndex);
        currentAppState = UiState::SELECTING_PLAN_FOR_REMOVE_TASK;
    }
    else if (menuItems[selectedItem] == u8"작업 완료하기") {
        selectedPlanIndex = 0;
        rightPanelDynamicContent = PanelContentGenerator::getPlanListStringsForPanel(planService, u8"  방향키로 선택, 엔터로 확인, ESC로 취소", selectedPlanIndex);
        currentAppState = UiState::SELECTING_PLAN_FOR_COMPLETE_TASK;
    }
    else if (menuItems[selectedItem] == u8"다마고치 모드") {
        rightPanelDynamicContent = {
            u8"  포인트를 사용해 다마고치에게 먹이를 주거나 놀아주세요",
            u8"  포인트는 작업을 완료하여 얻을 수 있습니다"
        };
        currentAppState = UiState::PROMPT_ENTER_TAMAGOTCHI_MODE;
    }
    else if (menuItems[selectedItem] == u8"종료") {
        // 프로그램 종료 시 다마고치 상태 저장
        saveTamagotchiToFile();
        running = false;
        system("cls");
        std::cout << u8"  PokeTODO를 종료합니다. 다음에 또 만나요!" << std::endl;
    }
}

void PokeTodoApp::processCalendarView() {
    rightPanelDynamicContent = PanelContentGenerator::getCalendarStringsForPanel(selectedCalendarDay);
    rightPanelDynamicContent.push_back("");
    rightPanelDynamicContent.push_back(u8"  선택된 날짜: 2025년 5월 " + std::to_string(selectedCalendarDay) + u8"일");
    rightPanelDynamicContent.push_back(u8"  이 날짜의 할 일을 확인하려면");
    rightPanelDynamicContent.push_back(u8"  '작업 목록 보기'를 이용하세요.");
}

void PokeTodoApp::processPlanSelection() {
    Plan* selectedPlan = PlanUtils::getPlanByIndex(planService, selectedPlanIndex);
    if (!selectedPlan) return;

    switch (currentAppState) {
    case UiState::SELECTING_PLAN_FOR_TASKS:
        currentPlanIdContext = selectedPlan->getPlanId();
        rightPanelDynamicContent = PanelContentGenerator::getTaskListStringsForPanel(selectedPlan);
        currentAppState = UiState::VIEWING_TASKS;
        break;

    case UiState::SELECTING_PLAN_FOR_ADD_TASK:
        tempTaskData.planId = selectedPlan->getPlanId();
        rightPanelDynamicContent = { u8"  플랜  [" + tempTaskData.planId + u8"]에 작업 추가 중..." };
        inputPrompt = u8"  작업 제목: ";
        currentAppState = UiState::ADDING_TASK_DETAILS_TITLE;
        break;

    case UiState::SELECTING_PLAN_FOR_REMOVE_TASK:
        currentPlanIdContext = selectedPlan->getPlanId();
        selectedTaskIndex = 0;
        rightPanelDynamicContent = PanelContentGenerator::getTaskListStringsForPanel(selectedPlan, u8"  방향키로 작업 선택, 엔터로 제거, ESC로 취소", selectedTaskIndex);
        currentAppState = UiState::SELECTING_TASK_FOR_REMOVE;
        break;

    case UiState::SELECTING_PLAN_FOR_COMPLETE_TASK:
        currentPlanIdContext = selectedPlan->getPlanId();
        selectedTaskIndex = 0;
        rightPanelDynamicContent = PanelContentGenerator::getTaskListStringsForPanel(selectedPlan, u8"  방향키로 작업 선택, 엔터로 완료, ESC로 취소", selectedTaskIndex);
        currentAppState = UiState::SELECTING_TASK_FOR_COMPLETE;
        break;
    }
}

void PokeTodoApp::processTaskSelection() {
    Plan* currentPlan = planService.getPlan(currentPlanIdContext);
    if (!currentPlan || selectedTaskIndex >= static_cast<int>(currentPlan->getTasks().size())) return;

    const auto& tasks = currentPlan->getTasks();
    const std::string& taskId = tasks[selectedTaskIndex].getTaskId();

    if (currentAppState == UiState::SELECTING_TASK_FOR_REMOVE) {
        rightPanelDynamicContent = { u8"  [" + (currentPlan->getTask(taskId))->getTitle() + u8"] 작업을 정말 제거하시겠습니까?" };
        rightPanelDynamicContent.push_back(u8"");
        rightPanelDynamicContent.push_back(u8"  [ENTER] 제거하기");
        rightPanelDynamicContent.push_back(u8"  [ESC] 나가기");
        currentAppState = UiState::DOUBLE_CHECK_FOR_REMOVE_TASK;
    }
    else if (currentAppState == UiState::SELECTING_TASK_FOR_COMPLETE) {
        if (((planService.getPlan(currentPlanIdContext))->getTask(taskId))->getStatus() == TaskStatus::COMPLETED) {
            rightPanelDynamicContent = { u8"  이미 완료된 작업입니다!" };
            rightPanelDynamicContent.push_back(u8"  엔터를 누르면 메뉴로 돌아갑니다.");
            currentAppState = UiState::VIEWING_TASKS;
        }
        else if (((planService.getPlan(currentPlanIdContext))->getTask(taskId))->getStatus() != TaskStatus::COMPLETED) {
            planService.completeTask(taskId);
            pointService.addPoints(3);
            rightPanelDynamicContent = { u8"  ["+tasks[selectedTaskIndex].getTitle()+u8"]작업을(를) 완료했습니다!" };
            rightPanelDynamicContent.push_back(
                u8"  보유 포인트 : " + std::to_string(pointService.getPoints() - 3) +
                u8" →  " + std::to_string(pointService.getPoints()));
            rightPanelDynamicContent.push_back(u8"  엔터를 누르면 메뉴로 돌아갑니다.");
            currentAppState = UiState::VIEWING_TASKS;
        }
    }
}

void PokeTodoApp::processDoubleCheck() {
    Plan* currentPlan = planService.getPlan(currentPlanIdContext);
    if (!currentPlan || selectedTaskIndex >= static_cast<int>(currentPlan->getTasks().size())) return;

    const auto& tasks = currentPlan->getTasks();
    const std::string& taskId = tasks[selectedTaskIndex].getTaskId();

    rightPanelDynamicContent = { u8"  [" + (currentPlan->getTask(taskId))->getTitle() + u8"] 작업을 제거했습니다." };
    rightPanelDynamicContent.push_back(u8"  엔터를 누르면 메뉴로 돌아갑니다.");
    planService.removeTask(taskId);
    currentAppState = UiState::VIEWING_TASKS;
}


void PokeTodoApp::processTamagotchiMode() {
    Tamagotchi::Tamagotchi* pet = tamagotchiService.getTamagotchi();
    rightPanelDynamicContent = PanelContentGenerator::getTamagotchiMenu(pet, pointService, selectedTamagotchiIndex);
    currentAppState = UiState::SELECTING_TAMAGOTCHI_ACTION;
    selectedTamagotchiIndex = 0;
    /*
    while (true) {
        Tamagotchi::Tamagotchi* pet = tamagotchiService.getTamagotchi();

        rightPanelDynamicContent = {
            " Lv." + std::to_string(pet->getLevel()) + "  " + pet->getName(),
            u8" 상태: " + pet->getCurrentStateName(),
            u8" 행복도: " + std::to_string(pet->getHappiness()),
            u8" 배고픔: " + std::to_string(pet->getHunger()),
            u8"",
            u8" 포인트를 사용하여 다마고치와 상호작용하세요! (남은 포인트: " + std::to_string(pointService.getPoints()) + ")",
            u8"    먹이주기",
            u8"    놀아주기",
            u8"    종료"
        };

        UIManager::displayMenu(menuItems, selectedItem, rightPanelDynamicContent, inputPrompt, myPet->getCurrentArt());
        UIManager::clearKeyboardBuffer();

        int tamagotchiInput = _getch();

        if (tamagotchiInput == 'F' || tamagotchiInput == 'f') {
            if (pointService.getPoints() > 0) {
                tamagotchiService.interactWithTamagotchi(TamagotchiAction::FEED);
                pointService.addPoints(-1);
            }
            else {
                rightPanelDynamicContent = { u8" 포인트가 부족해요!" };
                break;
            }
        }
        else if (tamagotchiInput == 'P' || tamagotchiInput == 'p') {
            if (pointService.getPoints() > 0) {
                tamagotchiService.interactWithTamagotchi(TamagotchiAction::PLAY);
                pointService.addPoints(-1);
            }
            else {
                rightPanelDynamicContent = { u8" 포인트가 부족해요!" };
                break;
            }
        }
        else if (tamagotchiInput == 'L' || tamagotchiInput == 'l') {
            myPet->levelUp();
            rightPanelDynamicContent = { myPet->getName() + u8"은(는) 이제 " + std::to_string(myPet->getLevel()) + u8"레벨입니다" };
            if (myPet->tryEvolve()) {
                rightPanelDynamicContent.push_back(myPet->getName() + u8"이(가) 진화했습니다!");
                myPet->setName();
                break;
            }
            break;
        }
        else if (tamagotchiInput == 27) { // ESC
            // 다마고치 모드 종료 시 상태 저장
            saveTamagotchiToFile();
            currentAppState = UiState::SHOWING_MAIN_MENU;
            rightPanelDynamicContent = PanelContentGenerator::getDefaultRightPanelContent();
            break;
        }

        if (pet->getHappiness() >= 99) {
            myPet->levelUp();
            rightPanelDynamicContent = { myPet->getName() + u8"은(는) 이제 " + std::to_string(myPet->getLevel()) + u8"레벨입니다" };
            if (myPet->tryEvolve()) {
                rightPanelDynamicContent.push_back(myPet->getName() + u8"이(가) 진화했습니다!");
                myPet->setName();
                break;
            }
            break;
        }
    }
    */
}

void PokeTodoApp::processTamagotchiAction() {
    Tamagotchi::Tamagotchi* pet = tamagotchiService.getTamagotchi();
    if (currentAppState == UiState::SELECTING_TAMAGOTCHI_ACTION) {
        if (tamagotchiItems[selectedTamagotchiIndex] == u8"먹이주기") {
            if (pointService.getPoints() > 0) {
                tamagotchiService.interactWithTamagotchi(TamagotchiAction::FEED);
                pointService.addPoints(-1);
                rightPanelDynamicContent = PanelContentGenerator::getTamagotchiMenu(pet, pointService, selectedTamagotchiIndex);
                if (pet->getHappiness() >= 99) {
                    myPet->levelUp();
                    rightPanelDynamicContent = { u8"  " + myPet->getName() + u8"은(는) 이제 " + std::to_string(myPet->getLevel()) + u8"레벨입니다" };
                    if (myPet->tryEvolve()) {
                        rightPanelDynamicContent.push_back(u8"  " + myPet->getName() + u8"이(가) 진화했습니다!");
                        myPet->setName();
                    }
                    currentAppState = UiState::TAMAGOTCHI_ONE_MORE_ENTER;
                }
            }
            else {
                rightPanelDynamicContent = { u8" 포인트가 부족해요!" };
                currentAppState = UiState::TAMAGOTCHI_ONE_MORE_ENTER;
            }
        }
        else if (tamagotchiItems[selectedTamagotchiIndex] == u8"놀아주기") {
            if (pointService.getPoints() > 0) {
                tamagotchiService.interactWithTamagotchi(TamagotchiAction::PLAY);
                pointService.addPoints(-1);
                rightPanelDynamicContent = PanelContentGenerator::getTamagotchiMenu(pet, pointService, selectedTamagotchiIndex);
                if (pet->getHappiness() >= 99) {
                    myPet->levelUp();
                    rightPanelDynamicContent = { u8"  " + myPet->getName() + u8"은(는) 이제 " + std::to_string(myPet->getLevel()) + u8"레벨입니다" };
                    if (myPet->tryEvolve()) {
                        rightPanelDynamicContent.push_back(u8"  " + myPet->getName() + u8"이(가) 진화했습니다!");
                        myPet->setName();
                    }
                    currentAppState = UiState::TAMAGOTCHI_ONE_MORE_ENTER;
                }
            }
            else {
                rightPanelDynamicContent = { u8"  포인트가 부족해요!" };
                currentAppState = UiState::TAMAGOTCHI_ONE_MORE_ENTER;
            }
        }
        else if (tamagotchiItems[selectedTamagotchiIndex] == u8"종료") {
            // 다마고치 모드 종료 시 상태 저장
            saveTamagotchiToFile();
            rightPanelDynamicContent = { u8"  다마고치 모드를 종료합니다." };
            selectedTamagotchiIndex = 0;
            currentAppState = UiState::QUIT_TAMIGOTCHI_MODE;
        }
    }
    else if (currentAppState == UiState::TAMAGOTCHI_ONE_MORE_ENTER) {
        rightPanelDynamicContent = PanelContentGenerator::getTamagotchiMenu(pet, pointService, selectedTamagotchiIndex);
        currentAppState = UiState::SELECTING_TAMAGOTCHI_ACTION;
    }
}

void PokeTodoApp::processTaskTitleInput() {
    std::string title;
    std::cout << u8"\n제목을 입력하세요: ";
    std::getline(std::cin, title);

    if (!title.empty()) {
        tempTaskData.title = title;
        rightPanelDynamicContent = { u8"플랜 [" + tempTaskData.planId + u8"]에 작업 추가 중..." };
        rightPanelDynamicContent.push_back(u8"제목: " + tempTaskData.title);
        inputPrompt = u8"우선순위 (1:높음, 2:중간, 3:낮음): ";
        currentAppState = UiState::ADDING_TASK_DETAILS_PRIORITY;
    }
    else {
        inputPrompt = u8"제목을 다시 입력하세요: ";
    }
}

void PokeTodoApp::processTaskPriorityInput() {
    std::string priorityStr;
    std::cout << u8"\n우선순위를 입력하세요 (1:높음, 2:중간, 3:낮음): ";
    std::getline(std::cin, priorityStr);

    if (priorityStr == "1") {
        tempTaskData.priority = Priority::HIGH;
    }
    else if (priorityStr == "2") {
        tempTaskData.priority = Priority::MEDIUM;
    }
    else if (priorityStr == "3") {
        tempTaskData.priority = Priority::LOW;
    }
    else {
        tempTaskData.priority = Priority::MEDIUM; // 기본값
    }

    // 기본값들 설정
    tempTaskData.description = ""; // 빈 설명
    tempTaskData.deadline = std::time(0) + (7 * 86400); // 7일 후 기본 마감일
    tempTaskData.tags.clear(); // 빈 태그 목록

    // 작업 생성
    TaskCreateRequest request(tempTaskData.title, tempTaskData.description,
        tempTaskData.priority, tempTaskData.deadline, tempTaskData.tags);
    planService.addTaskToPlan(tempTaskData.planId, request);

    rightPanelDynamicContent = { u8"작업이 성공적으로 추가되었습니다!" };
    rightPanelDynamicContent.push_back(u8"제목: " + tempTaskData.title);

    std::string priorityText = priorityStr.empty() ? u8"중간" :
        (priorityStr == "1" ? u8"높음" :
            priorityStr == "2" ? u8"중간" : u8"낮음");
    rightPanelDynamicContent.push_back(u8"우선순위: " + priorityText);
    rightPanelDynamicContent.push_back(u8"플랜: " + tempTaskData.planId);
    rightPanelDynamicContent.push_back(u8"엔터를 누르면 메뉴로 돌아갑니다.");

    // 상태 초기화
    tempTaskData = {};
    inputPrompt = "";
    currentAppState = UiState::SHOWING_MAIN_MENU;
}

void PokeTodoApp::loadTamagotchiFromFile() {
    myPet = FileIOService::loadTamagotchiState();
    if (myPet) {
        std::cout << u8"저장된 다마고치를 불러왔습니다: " << myPet->getName() << std::endl;
    }
}

void PokeTodoApp::saveTamagotchiToFile() {
    if (myPet) {
        bool success = FileIOService::saveTamagotchiState(myPet);
        if (success) {
            std::cout << u8"다마고치 상태가 저장되었습니다." << std::endl;
        }
        else {
            std::cout << u8"다마고치 상태 저장에 실패했습니다." << std::endl;
        }
    }
}
