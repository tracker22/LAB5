#include "commands.h"
#include <algorithm>
#include <sstream>

using namespace std;

// Хранилище данных о поездах
static map<string, vector<string>> trainsData;
// Обратный индекс: город -> поезда
static map<string, vector<string>> townsIndex;

CommandType parseCommand(const string& commandStr) {
    if (commandStr.find("CREATE_TRAIN") == 0) return CommandType::CREATE_TRAIN;
    if (commandStr.find("TRAINS_FOR_TOWN") == 0) return CommandType::TRAINS_FOR_TOWN;
    if (commandStr.find("TOWNS_FOR_TRAIN") == 0) return CommandType::TOWNS_FOR_TRAIN;
    if (commandStr.find("TRAINS") == 0) return CommandType::TRAINS;
    return CommandType::UNKNOWN;
}

void createTrain(const string& trainName, const vector<string>& towns) {
    // Добавляем поезд в основное хранилище
    trainsData[trainName] = towns;
    
    // Обновляем обратный индекс
    for (const auto& town : towns) {
        townsIndex[town].push_back(trainName);
    }
}

vector<string> getTrainsForTown(const string& town) {
    if (townsIndex.find(town) != townsIndex.end()) {
        return townsIndex.at(town);
    }
    return {};
}

map<string, vector<string>> getTownsForTrain(const string& trainName) {
    map<string, vector<string>> result;
    
    if (trainsData.find(trainName) != trainsData.end()) {
        for (const auto& town : trainsData.at(trainName)) {
            // Копируем все поезда для этого города, кроме текущего
            vector<string> trains;
            for (const auto& t : townsIndex.at(town)) {
                if (t != trainName) {
                    trains.push_back(t);
                }
            }
            result[town] = trains;
        }
    }
    
    return result;
}

map<string, vector<string>> getAllTrains() {
    return trainsData;
}