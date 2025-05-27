#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "commands.h"

using namespace std;

vector<string> splitString(const string& input) {
    vector<string> tokens;
    istringstream iss(input);
    string token;
    
    while (iss >> token) {
        tokens.push_back(token);
    }
    
    return tokens;
}

void processCommand(const string& input) {
    vector<string> tokens = splitString(input);
    if (tokens.empty()) return;
    
    string commandStr = tokens[0];
    CommandType command = parseCommand(commandStr);
    
    switch (command) {
        case CommandType::CREATE_TRAIN: {
            if (tokens.size() < 3) {
                cout << "Ошибка: недостаточно параметров для команды CREATE_TRAIN" << endl;
                break;
            }
            string trainName = tokens[1];
            vector<string> towns(tokens.begin() + 2, tokens.end());
            createTrain(trainName, towns);
            cout << "Поезд " << trainName << " создан" << endl;
            break;
        }
        
        case CommandType::TRAINS_FOR_TOWN: {
            if (tokens.size() != 2) {
                cout << "Ошибка: неверное количество параметров для команды TRAINS_FOR_TOWN" << endl;
                break;
            }
            string town = tokens[1];
            vector<string> trains = getTrainsForTown(town);
            cout << "Поезда, проходящие через город " << town << ":" << endl;
            for (const auto& train : trains) {
                cout << "- " << train << endl;
            }
            break;
        }
        
        case CommandType::TOWNS_FOR_TRAIN: {
            if (tokens.size() != 2) {
                cout << "Ошибка: неверное количество параметров для команды TOWNS_FOR_TRAIN" << endl;
                break;
            }
            string trainName = tokens[1];
            auto townsInfo = getTownsForTrain(trainName);
            cout << "Города, которые проезжает поезд " << trainName << ":" << endl;
            for (const auto& [town, trains] : townsInfo) {
                cout << "- " << town;
                if (!trains.empty()) {
                    cout << " (другие поезда: ";
                    for (size_t i = 0; i < trains.size(); ++i) {
                        if (i != 0) cout << ", ";
                        cout << trains[i];
                    }
                    cout << ")";
                }
                cout << endl;
            }
            break;
        }
        
        case CommandType::TRAINS: {
            auto allTrains = getAllTrains();
            cout << "Все поезда:" << endl;
            for (const auto& [train, towns] : allTrains) {
                cout << "- " << train << ": ";
                for (size_t i = 0; i < towns.size(); ++i) {
                    if (i != 0) cout << " -> ";
                    cout << towns[i];
                }
                cout << endl;
            }
            break;
        }
        
        case CommandType::UNKNOWN:
            cout << "Ошибка: неизвестная команда" << endl;
            break;
    }
}

int main() {
    cout << "Система управления поездами" << endl;
    cout << "Доступные команды:" << endl;
    cout << "CREATE_TRAIN <имя_поезда> <город1> <город2> ..." << endl;
    cout << "TRAINS_FOR_TOWN <город>" << endl;
    cout << "TOWNS_FOR_TRAIN <поезд>" << endl;
    cout << "TRAINS" << endl;
    cout << "Для выхода введите 'exit'" << endl;
    
    string input;
    while (true) {
        cout << "> ";
        getline(cin, input);
        
        if (input == "exit") {
            break;
        }
        
        processCommand(input);
    }
    
    return 0;
}