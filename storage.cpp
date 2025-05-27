#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

// Глобальные переменные для хранения данных склада
map<string, map<string, int>> storage;
const vector<char> ZONES = {'A', 'B', 'C'};
const int SHELVES = 20;
const int VERTICAL = 5;
const int HORIZONTAL = 2;
const int capacity_per_cell = 10;

// Функция для проверки корректности адреса
bool isAdressValid(const string& address) {
    if (address.length() != 5) return false;
    
    // Проверяем зону
    if (find(ZONES.begin(), ZONES.end(), address[0]) == ZONES.end()) 
        return false;
    
    // Проверяем номер стеллажа (2 цифры)
    if (!isdigit(address[1]) || !isdigit(address[2])) 
        return false;
    int shelf = stoi(address.substr(1, 2));
    if (shelf < 1 || shelf > SHELVES) 
        return false;
    
    // Проверяем номер секции
    if (!isdigit(address[3])) 
        return false;
    int section = address[3] - '0';
    if (section < 1 || section > VERTICAL) 
        return false;
    
    // Проверяем номер полки
    if (!isdigit(address[4])) 
        return false;
    int shelf_num = address[4] - '0';
    if (shelf_num < 1 || shelf_num > HORIZONTAL) 
        return false;
    
    return true;
}

// Функция для подсчета общего количества товаров в ячейке
int itemsInCell(const string& address) {
    int total = 0;
    for (const auto& item : storage[address]) {
        total += item.second;
    }
    return total;
}

// Функция для добавления товара
void add(const string& name, int value, const string& address) {
    if (!isAdressValid(address)) {
        cout << "Ошибка: некорректный адрес " << address << endl;
        return;
    }
    
    int current_quantity = itemsInCell(address);
    if (current_quantity + value > capacity_per_cell) {
        cout << "Ошибка: в ячейке " << address << " недостаточно места для " 
             << value << " единиц товара" << endl;
        return;
    }
    
    storage[address][name] += value;
    cout << "Добавлено " << value << " единиц товара '" << name 
         << "' в ячейку " << address << endl;
}

// Функция для удаления товара
void remove(const string& name, int value, const string& address) {
    if (!isAdressValid(address)) {
        cout << "Ошибка: некорректный адрес " << address << endl;
        return;
    }
    
    if (storage[address].count(name) == 0 || storage[address][name] < value) {
        cout << "Ошибка: в ячейке " << address << " недостаточно товара '" 
             << name << "' для списания" << endl;
        return;
    }
    
    storage[address][name] -= value;
    if (storage[address][name] == 0) {
        storage[address].erase(name);
    }
    cout << "Списано " << value << " единиц товара '" << name 
         << "' из ячейки " << address << endl;
}

// Функция для вывода информации о складе
void info() {
    int cellsTotal = ZONES.size() * SHELVES * VERTICAL * HORIZONTAL;
    int used_cells = 0;
    int total_used = 0;
    int total_capacity = cellsTotal * capacity_per_cell;
    
    // Подсчет использованных ячеек и общего количества товаров
    for (const auto& cell : storage) {
        int cell_items = 0;
        for (const auto& item : cell.second) {
            cell_items += item.second;
        }
        if (cell_items > 0) {
            used_cells++;
            total_used += cell_items;
        }
    }
    
    // Общая загруженность склада
    double total_percent = (static_cast<double>(total_used) / total_capacity) * 100;
    cout << fixed << setprecision(1);
    cout << "\nОбщая загруженность склада: " << total_percent << "%" << endl;
    
    // Загруженность по зонам
    for (char zone : ZONES) {
        int zone_used = 0;
        int zone_cells = SHELVES * VERTICAL * HORIZONTAL;
        
        for (const auto& cell : storage) {
            if (cell.first[0] == zone) {
                for (const auto& item : cell.second) {
                    zone_used += item.second;
                }
            }
        }
        
        double zone_percent = (static_cast<double>(zone_used) / (zone_cells * capacity_per_cell)) * 100;
        cout << "Зона " << zone << " загружена на " << zone_percent << "%" << endl;
    }
    
    // Вывод содержимого непустых ячеек
    cout << "\nСодержимое ячеек:" << endl;
    for (const auto& cell : storage) {
        if (!cell.second.empty()) {
            cout << "Ячейка " << cell.first << ": ";
            for (const auto& item : cell.second) {
                cout << item.first << " - " << item.second << " шт.; ";
            }
            cout << endl;
        }
    }
    
    // Вывод пустых ячеек
    cout << "\nПустые ячейки:" << endl;
    int empty_count = 0;
    for (char zone : ZONES) {
        for (int shelf = 1; shelf <= SHELVES; shelf++) {
            for (int section = 1; section <= VERTICAL; section++) {
                for (int shelf_num = 1; shelf_num <= HORIZONTAL; shelf_num++) {
                    string shelf_str = (shelf < 10) ? "0" + to_string(shelf) : to_string(shelf);
                    string address = string(1, zone) + shelf_str + to_string(section) + to_string(shelf_num);
                    if (storage[address].empty()) {
                        if (empty_count % 10 == 0 && empty_count != 0) cout << endl;
                        cout << address << " ";
                        empty_count++;
                    }
                }
            }
        }
    }
    cout << endl;
}

// Функция для инициализации склада
void initStorage() {
    for (char zone : ZONES) {
        for (int shelf = 1; shelf <= SHELVES; shelf++) {
            for (int section = 1; section <= VERTICAL; section++) {
                for (int shelf_num = 1; shelf_num <= HORIZONTAL; shelf_num++) {
                    string shelf_str = (shelf < 10) ? "0" + to_string(shelf) : to_string(shelf);
                    string address = string(1, zone) + shelf_str + to_string(section) + to_string(shelf_num);
                    storage[address] = map<string, int>();
                }
            }
        }
    }
}

int main() {
    initStorage();
    cout << "Программа учета товаров на складе" << endl;
    cout << "Доступные команды:" << endl;
    cout << "ADD <наименование> <количество> <адрес> - добавить товар" << endl;
    cout << "REMOVE <наименование> <количество> <адрес> - удалить товар" << endl;
    cout << "INFO - показать информацию о складе" << endl;
    cout << "EXIT - выход из программы" << endl;
    
    string command;
    while (true) {
        cout << "\n> ";
        getline(cin, command);
        
        if (command.empty()) continue;
        
        if (command == "INFO") {
            info();
        }
        else if (command == "EXIT") {
            break;
        }
        else {
            vector<string> comParts;
            size_t start = 0;
            size_t end = command.find(' ');
            
            // Разбиваем команду на части
            while (end != string::npos) {
                comParts.push_back(command.substr(start, end - start));
                start = end + 1;
                end = command.find(' ', start);
            }
            comParts.push_back(command.substr(start));
            
            if (comParts.size() == 4 && (comParts[0] == "ADD" or comParts[0] == "REMOVE")) {
                try {
                    string name = comParts[1];
                    int value = stoi(comParts[2]);
                    string address = comParts[3];
                    
                    if (comParts[0] == "ADD") {
                        add(name, value, address);
                    } else {
                        remove(name, value, address);
                    }
                } catch (const invalid_argument&) {
                    cout << "Ошибка: некорректное количество товара" << endl;
                }
            } else {
                cout << "Ошибка: неизвестная команда или неверный формат" << endl;
            }
        }
    }
    
    return 0;
}