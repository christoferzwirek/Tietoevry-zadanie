#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
#include <cmath>
#include <bits/stdc++.h>

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
    int id;
    char action;
    char parameters;
    int mx,my;
};

struct Base {
	int id;
	char player;
	int x;
	int y;
	int health;
	char producingUnit;
	int time;
	
};
struct UnitType {
    char symbol;      // Typ jednostki (K, S, A, P, R, C, W, B)
    int health;       // Wytrzymałość
    int speed;        // Prędkość
    int cost;         // Koszt zakupu
    int attackRange;  // Zasięg ataku
    int buildTime;    // Czas budowania
};

//lista jednostek
vector<UnitType> unitTypes = {
    {'K', 70, 5, 400, 1, 5},
    {'S', 60, 2, 250, 1, 3},
    {'A', 40, 2, 250, 5, 3},
    {'P', 50, 2, 200, 2, 3},
    {'R', 90, 2, 500, 1, 4},
    {'C', 50, 2, 800, 7, 6},
    {'W', 20, 2, 100, 1, 2},
    {'B', 200, 0, 0, 0, 0}  // Użyj 0 jako zastępczą wartość dla kosztu zakupu, zasięgu ataku i czasu budowania dla bazy
};


int distance(int x1, int y1, int x2, int y2) {
    return abs(x2 - x1) + abs(y2 - y1);
}
int build(vector<Command> &commands,vector<Base>& bases,int i,int j){
if(bases[i].producingUnit!='0'){
cout<<bases[i].producingUnit;
cout<<"\nBaza już buduje\n";
return 1;
}
bases[i].producingUnit=commands[j].parameters;
return 0;
}
int F(char Type){
//cout<<unitTypes.size();

	for(int i=0;i<unitTypes.size();i++){
	if(unitTypes[i]==Type){
		return i;}
	}
}
int ruch(Unit unit){
	F(unit.type);
}
//fukcja do sprawdzanie czy pole jest zajęte
bool unitmap(vector<Unit> &units,Command& commands){
	for (const auto& unit : units){
		if(unit.x==commands.mx &&unit.y==commands.my){
		return true;}
	}
	return false;
}
void action(vector<Command>& commands,vector<Unit> &units,int i,int j,vector<string> &map){
if(commands[j].action=='M'){
//cout<<commands[j].mx.type;
int maxy=map[0].size();
int maxx=map.size();
//cout<<map[commands[j].mx][commands[j].my];
	if(commands[j].mx>maxx ||commands[j].my>maxy){
		cout<<"nieprawidłowe współżędne";
	}else if(map[commands[j].mx][commands[j].my]=='0' || 
(map[commands[j].mx][commands[j].my]=='6' && units[i].type=='W')) {
		ruch(units[i]);
		if(unitmap(units,commands[j])){
			cout<<"miejsce zajęte";
		}else{
			units[i].x=commands[j].mx;		
			units[i].y=commands[j].my;
			//int index=find(unitTypes.begin(),unitTypes.end(),'Z')
			//cout<<"index: "<<index<<endl;
			
		}
	
	}else{
	cout<<"nie prawidłowy ruch";
}
}
else if(commands[j].action=='A'){}
else{cout<<"nie poprawna akcja";}
}
int com(vector<Command> &commands,vector<Unit> &units,vector<Base>& bases, vector<string> &map){

	int i=0;
	int j=0;
	start:
		
	for (const auto& unit : units){
		if(units[i].id==commands[j].id){
			//cout<<units[i].id;
			action(commands,units,i,j,map);
			goto a;	
		}
	}
	for(i=0;i<bases.size();i++){
		if(bases[i].id==commands[j].id){
				build(commands,bases,i,j);
				goto a;	
		}
	}
	
	cout<<"brak ID"<<commands[i].id;
	
	a:
	j++;
	if(j>=commands.size()){return 0;}
	else{goto start;}

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
        cout << base.player << " " <<base.id<<" "<< base.x << " " << base.y << " " << base.health << " " << base.producingUnit << std::endl;
    }

    cout << "Units:" << endl;
    for (const auto& unit : units) {
        cout << unit.player << " " <<unit.type<<" "<<unit.id  << " " << unit.x << " " << unit.y << " " << unit.health << endl;
    }
    //ordersFile  ordersOutput
    //vector<Command> commands = parseCommands(ordersOutput&);
	//vector<Command> parseCommands(ifstream& filename) {

    vector<Command> commands;

    

    //string line;
    while (getline(ordersOutput, line)) {
        Command command;
        istringstream iss(line);

        if (iss >> command.id >> command.action) {
		if(command.action=='M'){
			iss>>command.mx>>command.my;
		}else{
		iss>>command.parameters;
		//command.mx=NULL;
		}
		
	
            commands.push_back(command);
        } else {
            cout << "Invalid format in line: " << line << std::endl;
        }
    }



	for (const Command& command : commands){
		
		cout << "Unit ID: " << command.id << ", Action: " << command.action << ", Parameters: " << command.parameters << endl;}
	com(commands,units,bases,map);
	cout << "Bases:" << std::endl;
    for (const auto& base : units) {
        cout << base.player << " " <<base.id<<" "<< base.x << " " << base.y << " " << base.health ;//<< " " << base.producingUnit << std::endl;
    }
//zapis(commands,units,bases);
    return 0;
}

