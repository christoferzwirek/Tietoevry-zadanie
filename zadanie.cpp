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


int F(char Type);
struct Unit {
    string player;
    char type;
    int id;
    int x;
    int y;
    int health;
    int ruch;
	bool atak=true;
};
struct Command {
    int id;
    int id2;
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

struct AttackTable {
    char unitType;
    vector<int> damageValues;
};
vector<AttackTable> attackTable = {
        {'K', {35, 35, 5, 35, 35, 50, 35, 35}},
        {'S', {30, 30, 30, 20, 20, 30, 30, 30}},
        {'A', {15, 15,15, 15, 10, 10, 15, 15}},
        {'P', {35, 15, 15, 15, 15, 10, 15, 10}},
        {'C', {40, 40, 40, 40, 40, 40, 40, 50}},
        {'R', {10, 10, 10, 10, 10, 10, 10, 50}},
        {'W', {5, 5, 5, 5, 5, 5, 5, 1}}
    };


int distance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}
int build(vector<Command> &commands,vector<Base>& bases,int i,int j,long &gold){
if(bases[i].producingUnit!='0'){
cout<<bases[i].producingUnit;
cout<<"\nBaza już buduje\n";
return 1;
}
bases[i].producingUnit=commands[j].parameters;
int index=F(commands[j].parameters);
gold-=unitTypes[index].cost;
return 0;
}
int F(char Type){
//cout<<unitTypes.size();

	for(int i=0;i<unitTypes.size();i++){
	if(unitTypes[i].symbol==Type){
		return i;}
	}
}
// Funkcja do pobierania indeksu jednostki w tabeli ataku na podstawie jej typu
int getUnitIndex(const vector<AttackTable>& attackTable, char unitType) {
    for (int i = 0; i < attackTable.size(); i++) {
        if (attackTable[i].unitType == unitType) {
            return i;
        }
    }
    return -1;  // Jeżeli nie znaleziono jednostki, zwracamy -1
}
int findUnitIndexByID(const vector<Unit>& units, int id) {
    for (int i = 0; i < units.size(); i++) {
        if (units[i].id == id) {
            return i;  // Zwracamy indeks jednostki
        }
    }
    // Jeśli nie znaleziono jednostki o podanym ID, zwracamy -1 lub inny wartość sygnalizującą brak znalezienia
    return -1;
}
int getDamage( char attackerType, char defenderType) {
    int attackerIndex = getUnitIndex(attackTable, attackerType);
    int defenderIndex = getUnitIndex(attackTable, defenderType);

    if (attackerIndex != -1 && defenderIndex != -1) {
        return attackTable[attackerIndex].damageValues[defenderIndex];
    }

    return 0;  // Jeżeli nie znaleziono jednostek, zwracamy 0
}
string getPlayerByID(const vector<Unit>& units, int id) {
    for (const auto& unit : units) {
        if (unit.id == id) {
            return unit.player;
        }
    }
    // Jeśli nie znaleziono jednostki o podanym ID, zwracamy pusty znak
    return '\0';
}

//fukcja do sprawdzanie czy pole jest zajęte
bool unitmap(vector<Unit> &units,Command& commands){
	string a=getPlayerByID(units,commands.id );
	//cout<<commands.id;
	//string a;
	for (const auto& unit : units){
		if((unit.x==commands.mx &&unit.y==commands.my) && (unit.player != a))return true;
				}
	return false;
}
void action(vector<Command>& commands,vector<Unit> &units,int i,int j,vector<string> &map){

if(commands[j].action=='M'){
//poruszanie się

int maxy=map[0].size();
int maxx=map.size();


	//sprawdzamy czy nie wychodzimy poza mapę 
	if(commands[j].mx>=maxx ||commands[j].my>=maxy){
		cout<<"nieprawidłowe współżędne";
	//sprawdzamy czy pole na mapie jest wolne lub czy Worker może wejść do kopalni
	}else if(map[commands[j].mx][commands[j].my]=='0' || 
(map[commands[j].mx][commands[j].my]=='6' && units[i].type=='W')) {
		int a=F(units[i].type);//sprawdzenie czy można tak dalego ruszać się

		if(distance(units[i].x,units[i].y,commands[j].mx,commands[j].my)>unitTypes[a].speed){
		cout<<"za duża odległość do pokonania";
		}else{
			if(unitmap(units,commands[j])){
				cout<<"miejsce zajęte";
			}else{
				units[i].ruch-=distance(units[i].x,units[i].y,commands[j].mx,commands[j].my);
				units[i].x=commands[j].mx;		
				units[i].y=commands[j].my;
			     }
			}
	}else{
	cout<<"nie prawidłowy ruch";
}
}
else if(commands[j].action=='A'){
//atakowanie 
	int a=F(units[i].type);//sprawdzenie czy można tak dalego atakować
	string P= getPlayerByID (units,commands[j].id);
	string P2= getPlayerByID (units,commands[j].id2);
	int enemyID=findUnitIndexByID(units, commands[j].id2);
	//cout<<distance(units[enemyID].x,units[enemyID].y,units[i].x,units[i].y);
	//cout<<units[i].type;
		if(distance(units[enemyID].x,units[enemyID].y,units[i].x,units[i].y)>unitTypes[a].attackRange){
		cout<<"za duża odległość do atakowania";
		}else if(units[i].ruch<=1){
		cout<<"brak punktów ruchu";
		}else if(units[i].player==P2){
		cout<<"nie można atakować własnych jednostek";
		}else{
		units[i].ruch-=1;
		units[i].atak=false;
		int DMG=getDamage(units[i].type,units[enemyID].type) ;
		units[enemyID].health-=DMG;
		if(units[enemyID].health<=0)units.erase(units.begin()+a);
		}
}
else{cout<<"nie poprawna akcja";}


}//koniec funkcji
int com(vector<Command> &commands,vector<Unit> &units,vector<Base>& bases, vector<string> &map,long &playerGold,long &enemygold){

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
		
				if(bases[i].player=='P'){
				build(commands,bases,i,j,playerGold);}
				else{ build(commands,bases,i,j,enemygold);}
				goto a;	
		}
	}
	
	cout<<"brak ID"<<commands[i].id;
	
	a:
	j++;
	if(j>=commands.size()){return 0;}
	else{goto start;}

}

void saveStatus(const vector<Unit>& units, const vector<Base>& bases, long &playerGold, long &enemyGold,string statusFile) {
    ofstream statusOutput(statusFile);
    if (!statusOutput) {
        cout << "Błąd podczas otwierania pliku status.txt" << endl;
        return;
    }

    // Zapisz stan złota gracza i przeciwnika
    statusOutput << "P " << playerGold << endl;
    statusOutput << "E " << enemyGold << endl;

    // Zapisz dane jednostek
    for (const auto& unit : units) {
        statusOutput << unit.player << " " << unit.type << " " << unit.id << " " << unit.x << " " << unit.y << " " << unit.health << endl;
    }

    // Zapisz dane baz
    for (const auto& base : bases) {
        statusOutput << base.player <<" B"<< " " << base.id << " " << base.x << " " << base.y << " " << base.health << " " << base.producingUnit << endl;
    }

    statusOutput.close();
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
    long playerGold,enemyGold;
    while (getline(mapInput, line)) {
	
        map.push_back(line);
    }
    string playerLine, enemyLine;
    getline(statusInput, playerLine); // Wczytaj linijkę gracza
    getline(statusInput, enemyLine);  // Wczytaj linijkę przeciwnika

    
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
		int a =F(unit.type);
		
		unit.ruch=unitTypes[a].speed;
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
	for ( auto& unit : units) {
		}
    

    //string line;
    while (getline(ordersOutput, line)) {
        Command command;
        istringstream iss(line);

        if (iss >> command.id >> command.action) {
		if(command.action=='M'){
			iss>>command.mx>>command.my;
		}else if(command.action=='B') {
		iss>>command.parameters;
		//command.mx=NULL;
		}
		else{
		iss>>command.id2;
}
	
            commands.push_back(command);
        } else {
            cout << "Invalid format in line: " << line << std::endl;
        }
    }



	for (const Command& command : commands){
		
		cout << "Unit ID: " << command.id << ", Action: " << command.action << ", Parameters: " << command.parameters << endl;}


	com(commands,units,bases,map, playerGold, enemyGold);
	cout << "Bases:" << std::endl;
    for (const auto& base : units) {
        cout << base.player << " " <<base.id<<" "<< base.x << " " << base.y << " " << base.health ;//<< " " << base.producingUnit << std::endl;
    }
    saveStatus(units,bases,playerGold,enemyGold,statusFile);
    return 0;
}

