#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <cmath>


using namespace std;

struct Unit {
    int id;
    int x;
    int y;
    int health;
};

struct Base {
    int player;
    int x;
    int y;
    int health;
};

int distance(int x1, int y1, int x2, int y2) {
    return abs(x2 - x1) + abs(y2 - y1);
}

void playerLogic(const vector<string>& map, const vector<Unit>& units, const vector<Base>& bases) {
    for (const Unit& unit : units) {
        int targetX = -1;
        int targetY = -1;
        int minDistance = std::numeric_limits<int>::max();

        for (const Unit& enemyUnit : units) {
            if (enemyUnit.id != unit.id && enemyUnit.health > 0 && enemyUnit.id != unit.id) {
                int dist = distance(unit.x, unit.y, enemyUnit.x, enemyUnit.y);
                if (dist < minDistance) {
                    minDistance = dist;
                    targetX = enemyUnit.x;
                    targetY = enemyUnit.y;
                }
            }
        }

        if (targetX != -1 && targetY != -1) {
            cout << unit.id << " A " << targetX << " " << targetY << endl;
        }
    }
}

int main(int argc, char** argv) {
    if (argc < 4 || argc > 5) {
        std::cerr << "Usage: " << argv[0] << " map.txt status.txt orders.txt [time_limit]" << std::endl;
        return 1;
    }

    string mapFile = argv[1];
    string statusFile = argv[2];
    string ordersFile = argv[3];
    int timeLimit = (argc == 5) ? atoi(argv[4]) : 5;

    ifstream mapInput(mapFile.c_str());
    ifstream statusInput(statusFile.c_str());
    ofstream ordersOutput(ordersFile.c_str());

    vector<std::string> map;
    vector<Unit> units;
    vector<Base> bases;

    string line;
    while (getline(mapInput, line)) {
        map.push_back(line);
    }

    int id, x, y, health;

    while (statusInput >> id >> x >> y >> health) {
        units.push_back({id, x, y, health});
    }

    while (statusInput >> id >> x >> y >> health) {
        bases.push_back({id, x, y, health});
    }

    playerLogic(map, units, bases);

    return 0;
}

