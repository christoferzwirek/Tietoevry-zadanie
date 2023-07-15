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
	string player;
char type;
    int id;
    int x;
    int y;
    int health;
};
struct Command {
    int unitId;
    char action;
    std::string parameters;
    int mx,my;
};

struct Base {
	int id;
	char player;
	int x;
	int y;
	int health;
	char producingUnit;
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
    }/*
	// Zapisz rozkazy do pliku
	    ofstream ordersOutput("rozkazy.txt");
	    if (ordersOutput.is_open()) {
		for (const string& command : commands) {
		    ordersOutput << command << endl;
		}
		ordersOutput.close();
	    }*/
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
    ifstream ordersOutput(ordersFile.c_str());

    vector<string> map;
    vector<Unit> units;
    vector<Base> bases;

    string line;
    while (getline(mapInput, line)) {
	
        map.push_back(line);
    }
    string playerLine, enemyLine;
    getline(statusInput, playerLine); // Wczytaj linijkę gracza
    getline(statusInput, enemyLine);  // Wczytaj linijkę przeciwnika

    long playerGold,enemyGold;
// Sprawdź pierwszą literę w linii i przypisz wartość do odpowiedniej zmiennej
    if (playerLine[0] == 'P') {
        playerGold = stoi(playerLine.substr(2));
    } else if (playerLine[0] == 'E') {
        enemyGold = stoi(playerLine.substr(2));
    }
    // Sprawdź pierwszą literę w linii i przypisz wartość do odpowiedniej zmiennej
    if (enemyLine[0] == 'P') {
        playerGold = stoi(enemyLine.substr(2));
    } else if (enemyLine[0] == 'E') {
        enemyGold = stoi(enemyLine.substr(2));
    }
    cout << "Player gold: " << playerGold << endl;
    cout << "Enemy gold: " << enemyGold << endl;

    while(getline(statusInput, line)) {
        istringstream iss(line);
        char player;
        char type;
        int x, y, health,id;
        iss >> player >> type >>id>> x >> y >> health;

        if (player == 'P' || player == 'E') {
	   if (type == 'B') {
                Base base;
                char producingUnit;
                iss >> producingUnit;
		base.producingUnit=producingUnit;	
		base.id=id;
		base.player=player;
		base.x=x;
		base.y=y;
		base.health=health;
		bases.push_back(base);
          }//koniec if B
	   else {
		Unit unit;
		unit.player=player;
		unit.type=type;
		unit.id=id;
		unit.x=x;
		unit.y=y;
		unit.health=health;
		units.push_back(unit);
            }//koniec else
        }//koniec pierwszy if
    }//koniec while

    

    cout << "Bases:" << std::endl;
    for (const auto& base : bases) {
        //cout << base.player << " " <<base.id<<" "<< base.x << " " << base.y << " " << base.health << " " << base.producingUnit << std::endl;
    }

    cout << "Units:" << endl;
    for (const auto& unit : units) {
//        cout << unit.player << " " <<unit.type<<" "<<unit.id  << " " << unit.x << " " << unit.y << " " << unit.health << endl;
    }
    //ordersFile  ordersOutput
    //vector<Command> commands = parseCommands(ordersOutput&);
	//vector<Command> parseCommands(ifstream& filename) {

    vector<Command> commands;

    

    //string line;
    while (getline(ordersOutput, line)) {
        Command command;
        istringstream iss(line);

        if (iss >> command.unitId >> command.action) {
		if(command.action=='M'){
			iss>>command.mx>>command.my;
		}else{
		iss>>command.parameters;
		}
		
	
            commands.push_back(command);
        } else {
            cout << "Invalid format in line: " << line << std::endl;
        }
    }



	for (const Command& command : commands){
		cout << "Unit ID: " << command.unitId << ", Action: " << command.action << ", Parameters: " << command.parameters << endl;}
    playerLogic(map, units, bases);

    return 0;
}

