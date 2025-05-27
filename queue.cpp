#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Генерация номера талона
string generateTicket() {
    return "T" + to_string(rand() % 1000);
}

// Функция распределения очереди по окнам
void distribute(const vector<pair<int, string>>& visitors, vector<int>& windowTimes, vector<vector<string>>& windowTickets) {
    // Распределяем посетителей по окнам
    for (const auto& visitor : visitors) {
        // Находим окно с минимальным временем
        int minWindow = min_element(windowTimes.begin(), windowTimes.end()) - windowTimes.begin();
        windowTimes[minWindow] += visitor.first;
        windowTickets[minWindow].push_back(visitor.second);
    }
}

int main() {
    cout << ">>> Введите кол-во окон\n<<< ";
    int windows;
    cin >> windows;
    
    vector<pair<int, string>> visitors;
    vector<int> windowTimes(windows, 0);
    vector<vector<string>> windowTickets(windows);
    
    string cmd;
    while (true) {
        cout << "<<< ";
        cin >> cmd;
        
        if (cmd == "ENQUEUE") {
            int time;
            cin >> time;
            string ticket = generateTicket();
            visitors.push_back({time, ticket});
            cout << ">>> " << ticket << endl;
        }
        else if (cmd == "DISTRIBUTE") {
            // Вызываем функцию распределения
            distribute(visitors, windowTimes, windowTickets);
            
            // Выводим результаты
            for (int i = 0; i < windows; ++i) {
                if (!windowTickets[i].empty()) {
                    cout << ">>> Окно " << i+1 << " (" << windowTimes[i] << " минут): ";
                    for (size_t j = 0; j < windowTickets[i].size(); ++j) {
                        if (j != 0) cout << ", ";
                        cout << windowTickets[i][j];
                    }
                    cout << endl;
                }
            }
            break;
        }
        else {
            cout << ">>> Неизвестная команда\n";
        }
    }
    
    return 0;
}