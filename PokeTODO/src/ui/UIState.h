#pragma once
#include <string>
#include <vector>
#include <ctime>
#include "../models/Task.h"

// UI 상태를 정의합니다.
enum class UiState {
    SHOWING_MAIN_MENU,       // 기본 메뉴 표시 상태 (오른쪽 패널은 기본 정보)
    VIEWING_CALENDAR,        // 캘린더 보기 상태
    SELECTING_PLAN_FOR_TASKS, // 작업 목록 보기를 위해 플랜 선택
    VIEWING_TASKS,           // 특정 플랜의 작업 목록을 오른쪽 패널에 표시
    SELECTING_PLAN_FOR_ADD_TASK, // 작업 추가를 위해 플랜 선택
    ADDING_TASK_DETAILS_TITLE,
    ADDING_TASK_DETAILS_DESC,
    ADDING_TASK_DETAILS_PRIORITY,
    ADDING_TASK_DETAILS_DEADLINE,
    ADDING_TASK_DETAILS_TAGS,
    SELECTING_PLAN_FOR_REMOVE_TASK, // 작업 제거를 위해 플랜 선택
    SELECTING_TASK_FOR_REMOVE, // 작업 제거를 위해 작업 선택
    SELECTING_PLAN_FOR_COMPLETE_TASK, // 작업 완료를 위해 플랜 선택
    SELECTING_TASK_FOR_COMPLETE, // 작업 완료를 위해 작업 선택
    PROMPT_ENTER_TAMAGOTCHI_MODE,
};

// 임시 작업 데이터 저장을 위한 구조체
struct TempTaskData {
    std::string planId;
    std::string title;
    std::string description;
    Priority priority = Priority::MEDIUM;
    std::time_t deadline = 0;
    std::vector<std::string> tags;
}; 