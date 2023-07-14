#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
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
	vector <string> commands;
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
	// Zapisz rozkazy do pliku
	    ofstream ordersOutput("rozkazy.txt");
	    if (ordersOutput.is_open()) {
		for (const string& command : commands) {
		    ordersOutput << command << endl;
		}
		ordersOutput.close();
	    }
}

int main(int argc, char** argv) {
	//sprawdzamy czy mamy włąściwą liczbę argumętów 
    if (argc < 4 || argc > 5) {
        cerr << "Usage: " << argv[0] << " map.txt status.txt orders.txt [time_limit]" << endl;
        return 1;
    }

    string mapFile = argv[1];//mapa
    string statusFile = argv[2];//status
    string ordersFile = argv[3];//rozkazy
    int timeLimit = (argc == 5) ? atoi(argv[4]) : 5;

    ifstream mapInput(mapFile.c_str());

    ifstream statusInput(statusFile.c_str());
    ofstream ordersOutput(ordersFile.c_str());

    vector<string> map;
    vector<Unit> units;
    vector<Base> bases;

    string line;
    while (getline(mapInput, line)) {
	
        map.push_back(line);
    }

    // Odczytaj status jednostek i baz z pliku
    int goldAmount;//ilość golda 
    statusInput >> goldAmount;

    string unitLine;
    while (getline(statusInput, unitLine)) {
        if (unitLine.empty()) {
            continue;
        }
	//cout<<unitLine<<endl;
	string player;
        char type;
        int id, x, y, health;
        char producingUnit;

        istringstream iss(unitLine);
        iss >> player >> type >> id >> x >> y >> health;
	//cout<<iss<<endl;
        if (player == "P") {
            iss >> producingUnit;
	cout<<"BASA"<<producingUnit;
            //bases.push_back(Base( const & player, x, y, health, producingUnit));
        } else if(player=="E"){
	//przeciwnik
           // units.push_back(Unit(const string& player, type, id, x, y, health));
        }
    }

    playerLogic(map, units, bases);

    return 0;
}

