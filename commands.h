#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <vector>
#include <map>

enum class CommandType {
    CREATE_TRAIN,
    TRAINS_FOR_TOWN,
    TOWNS_FOR_TRAIN,
    TRAINS,
    UNKNOWN
};

// Структура для хранения данных о поезде
struct Train {
    std::string name;
    std::vector<std::string> towns;
};

// Функции для обработки команд
CommandType parseCommand(const std::string& commandStr);
void createTrain(const std::string& trainName, const std::vector<std::string>& towns);
std::vector<std::string> getTrainsForTown(const std::string& town);
std::map<std::string, std::vector<std::string>> getTownsForTrain(const std::string& trainName);
std::map<std::string, std::vector<std::string>> getAllTrains();

#endif // COMMANDS_H