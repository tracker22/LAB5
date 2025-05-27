#include <iostream>
#include <string>
#include <map>
#include <set>

using namespace std;

void addFriends(const string$ name1, const string$ name2){
    friends[name1].insert(name2);
    friends[name2].insert(name1);
}
int main() {
    int N;
    cin >> N;

    map<string, set<string>> friends;

    for (int i = 0; i < N; ++i) {
        string command;
        cin >> command;

        if (command == "FRIENDS") {
            string name1, name2;
            cin >> name1 >> name2;
            addFriends(name1, name2);
        } else if (command == "COUNT") {
            string name;
            cin >> name;
            cout << friends[name].size() << endl;
        } else if (command == "QUESTION") {
            string name1, name2;
            cin >> name1 >> name2;
            if (friends[name1].count(name2) > 0) {
                cout << "YES" << endl;
            } else {
                cout << "NO" << endl;
            }
        }
    }

    return 0;
}