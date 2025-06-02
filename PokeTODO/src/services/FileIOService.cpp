#include "FileIOService.h"
#include <iostream>
#include <cstdio>  // remove 함수를 위한 헤더

// 다마고치 상태 저장 함수
bool FileIOService::saveTamagotchiState(const Tamagotchi::Tamagotchi* pet, const std::string& filename) {
    if (!pet) {
        std::cerr << u8"다마고치 포인터가 null입니다." << std::endl;
        return false;
    }

    std::string fullPath = getFullPath(filename);
    std::ofstream outFile(fullPath);
    
    if (!outFile.is_open()) {
        std::cerr << u8"파일 저장 실패: " << fullPath << std::endl;
        return false;
    }

    try {
        outFile << pet->getName() << std::endl;
        outFile << pet->getHappiness() << std::endl;
        outFile << pet->getHunger() << std::endl;
        outFile << pet->getLevel() << std::endl;
        outFile << pet->getCurrentStateName() << std::endl;
        outFile.close();
        
        std::cout << u8"다마고치 상태가 저장되었습니다: " << fullPath << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << u8"파일 저장 중 오류 발생: " << e.what() << std::endl;
        outFile.close();
        return false;
    }
}

// 다마고치 상태 불러오기 함수
Tamagotchi::Tamagotchi* FileIOService::loadTamagotchiState(const std::string& filename) {
    std::string fullPath = getFullPath(filename);
    
    if (!fileExists(fullPath)) {
        std::cout << u8"저장된 다마고치 파일이 없습니다: " << fullPath << std::endl;
        return nullptr;
    }

    std::ifstream inFile(fullPath);
    if (!inFile.is_open()) {
        std::cerr << u8"파일 읽기 실패: " << fullPath << std::endl;
        return nullptr;
    }

    try {
        std::string name;
        int happiness, hunger, level;
        std::string stateName;

        std::getline(inFile, name);
        inFile >> happiness >> hunger >> level;
        inFile.ignore(); // 개행문자 무시
        std::getline(inFile, stateName);

        Tamagotchi::Tamagotchi* pet = new Tamagotchi::Tamagotchi(level, name);
        pet->setHappiness(happiness);
        pet->setHunger(hunger);

        inFile.close();
        
        std::cout << u8"다마고치 상태가 불러와졌습니다: " << name << std::endl;
        return pet;
    }
    catch (const std::exception& e) {
        std::cerr << u8"파일 읽기 중 오류 발생: " << e.what() << std::endl;
        inFile.close();
        return nullptr;
    }
}

// 파일 존재 여부 확인
bool FileIOService::fileExists(const std::string& filename) {
    std::string fullPath = getFullPath(filename);
    std::ifstream file(fullPath);
    return file.good();
}

// 파일 삭제
bool FileIOService::deleteFile(const std::string& filename) {
    std::string fullPath = getFullPath(filename);
    try {
        // C 표준 라이브러리의 remove 함수 사용
        int result = std::remove(fullPath.c_str());
        return result == 0;
    }
    catch (const std::exception& e) {
        std::cerr << u8"파일 삭제 중 오류 발생: " << e.what() << std::endl;
        return false;
    }
}

// 데이터 디렉토리 경로 반환
std::string FileIOService::getDataDirectory() {
    // 현재는 실행 파일과 같은 디렉토리에 저장
    // 향후 필요시 Documents 폴더나 다른 경로로 변경 가능
    return "./";
}

// 전체 파일 경로 반환
std::string FileIOService::getFullPath(const std::string& filename) {
    return getDataDirectory() + filename;
} 