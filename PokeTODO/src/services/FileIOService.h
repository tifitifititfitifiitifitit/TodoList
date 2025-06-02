#pragma once

#include <string>
#include <fstream>
#include "../models/Tamagotchi.h"

class FileIOService {
public:
    // 다마고치 상태 저장/불러오기
    static bool saveTamagotchiState(const Tamagotchi::Tamagotchi* pet, const std::string& filename = "tamagotchi_save.txt");
    static Tamagotchi::Tamagotchi* loadTamagotchiState(const std::string& filename = "tamagotchi_save.txt");
    
    // 향후 확장을 위한 메서드들 (현재는 선언만)
    // static bool saveTaskData(const std::vector<Task>& tasks, const std::string& filename = "tasks_save.txt");
    // static std::vector<Task> loadTaskData(const std::string& filename = "tasks_save.txt");
    
    // 파일 존재 여부 확인
    static bool fileExists(const std::string& filename);
    
    // 파일 삭제
    static bool deleteFile(const std::string& filename);

private:
    // 파일 경로 관련 유틸리티
    static std::string getDataDirectory();
    static std::string getFullPath(const std::string& filename);
}; 