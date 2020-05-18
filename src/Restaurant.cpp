//
// Created by gorjalts@wincs.cs.bgu.ac.il on 11/9/18.
//
#include "../include/Restaurant.h"
#include <iostream>
#include <vector>
#include <string>
#include "../include/Dish.h"
#include "../include/Table.h"
#include "../include/Action.h"
#include <fstream>
#include <sstream>
using namespace std;


Restaurant::Restaurant() : open(false),tables(), menu(), actionsLog() {}


Restaurant::Restaurant(const std::string &configFilePath) {
    ifstream inFile; //declare
    inFile.open(configFilePath);//open
    string line; //the line that we are reading
    int numOfTables=0;
    int index = 0;// index of line

    if (!inFile.is_open()) {
        exit(1);   // call system to stop
    } else
        { //if we could open
        while (!inFile.eof()) {//reading lines until the file ends
            getline(inFile, line);//transfer the line to var "line"
            if (line.empty() || line[0] == '\r')//skip empty lines
                continue;
            else if (line.at(0) != '#') {//not a command
                if (index == 0) {//num Of tables
                    stringstream num(line);
                    num >> numOfTables;
                } else if (index == 1) { //create tables
                    CreateTables(line);
                } else { //create menu
                    CreateMenu(line);
                }
                index = index + 1;
            }
        }
        inFile.close();
    }
}

//aid function
void Restaurant::CreateTables(std::string &capacitystr) { //connect each table with his capacity
    while (capacitystr.size()) {
        if (static_cast<int>(capacitystr.find(',')) != -1) {
            string tablecapacityWord = capacitystr.substr(0, capacitystr.find(','));//extract the capacity
            capacitystr = capacitystr.substr(capacitystr.find(',') + 1);//cutting the line
            stringstream num(tablecapacityWord);//char to int
            int tcapacity = 0;
            num >> tcapacity;//char to int
            this->tables.push_back(new Table(tcapacity));
        } else {
            stringstream num(capacitystr);//char to int
            int tcapacity = 0;
            num >> tcapacity;//char to int
            this->tables.push_back(new Table(tcapacity));
            capacitystr = "";
        }
    }
}


//aid function
void Restaurant::CreateMenu(std::string &line) {
    string name, DishType, priceStr;
    __u_long charCounter = 0;
    name = line.substr(charCounter, line.find_first_of(',', charCounter)); //first substring name
    charCounter = line.find_first_of(',', charCounter + 1);
    DishType = line.substr(charCounter + 1, 3); //second substring Dish type
    charCounter = line.find_first_of(',', charCounter + 1);
    priceStr = line.substr(charCounter + 1); //last substring price
    stringstream priceS(priceStr);//char to int
    int price=0;
    priceS >> price;//char to int
    menu.emplace_back(Dish((int) menu.size(), name, price, Change(DishType)));
}

//aid function
DishType Restaurant::Change(std::string &str) {
    DishType d;
    if (str == "VEG") {
        d= VEG;
    } else if (str == "SPC") {
        d= SPC;
    } else if (str == "ALC") {
        d= ALC;
    } else if (str == "BVG") {
        d= BVG;
    }
    return d;
}



void  Restaurant::start() {

    int ID = 0;
    setOpen(true);
    cout << "Restaurant is now open!" << endl;
    string line;
    getline(cin, line); //the line that we are reading
    string actionOrder;
    int tID = -1; //table number
    string args; //customers+type
    int actindex = -1;
    actindex=(int)line.find_first_of(' ');
    actionOrder = line.substr(0, static_cast<unsigned long>(actindex));
    args=line.substr(static_cast<unsigned long>(actindex + 1));
    while (actionOrder != "closeall") {
        if (actindex+1== 0) {
            if (actionOrder == "log") {
                BaseAction *log=new PrintActionsLog();
                update(line,log,actindex,actionOrder);
                actindex = (int)line.find_first_of(' ');
                actionOrder=line.substr(0,line.find_first_of(' '));
                continue;
            } else if (actionOrder == "menu") {
                BaseAction *printM=new PrintMenu();
                update(line,printM,actindex,actionOrder);
                actindex = (int)line.find_first_of(' ');
                actionOrder=line.substr(0,line.find_first_of(' '));
                continue;
            }
            else if (actionOrder == "backup") {
                BaseAction *action= new BackupRestaurant();
                update(line,action,actindex,actionOrder);
                actindex = (int)line.find_first_of(' ');
                actionOrder=line.substr(0,line.find_first_of(' '));
                continue;
            }
           else if (actionOrder == "restore") {
                BaseAction *restore= new RestoreResturant();
                update(line,restore,actindex,actionOrder);
                actindex = (int)line.find_first_of(' ');
                actionOrder=line.substr(0,line.find_first_of(' '));
                continue;
            }
        } else {
            tID = line[actindex + 1]-'0';//update the table id
            if(static_cast<int>(line.size())> actindex + 3) {
                args = line.substr(static_cast<unsigned long>(actindex + 3)); //the rest
            }if (actionOrder == "close") {
                BaseAction *close = new Close(tID);
                update(line, close, actindex, actionOrder);
                actindex = (int)line.find_first_of(' ');
                actionOrder=line.substr(0,line.find_first_of(' '));
                continue;
            } else if (actionOrder == "order") {
                BaseAction *order = new Order(tID);
                update(line, order, actindex, actionOrder);
                actindex = (int)line.find_first_of(' ');
                actionOrder=line.substr(0,line.find_first_of(' '));
                continue;
            } else if (actionOrder == "open") {
                    int nameindex = 0;
                    std::vector<Customer *> Tcustomers; //the tables customers
                    string nametype;
                if (tID< static_cast<int>(tables.size())&& tID>0 && !tables[tID]->isOpen()) {//check if the input us valid
                    while (args[nameindex] != '\n' && args[nameindex] != '\0') {
                        while (args[nameindex] != ' ' && args[nameindex] != '\0' && args[nameindex] != '\n') {
                            nametype.push_back(args[nameindex]);
                            nameindex = nameindex + 1;
                        }
                        string Ctype = nametype.substr(nametype.length() - 3);
                        if (Ctype == "veg") {
                            Tcustomers.push_back(new VegetarianCustomer(nametype.substr(0, nametype.length() - 4), ID));
                            ID = ID + 1;
                        }
                        if (Ctype == "spc") {
                            Tcustomers.push_back(new SpicyCustomer(nametype.substr(0, nametype.length() - 4), ID));
                            ID = ID + 1;
                        }
                        if (Ctype == "alc") {
                            Tcustomers.push_back(new AlchoholicCustomer(nametype.substr(0, nametype.length() - 4), ID));
                            ID = ID + 1;
                        }
                        if (Ctype == "chp") {
                            Tcustomers.push_back(new CheapCustomer(nametype.substr(0, nametype.length() - 4), ID));
                            ID = ID + 1;
                        }
                        if (args[nameindex] != '\n' && args[nameindex] != '\0') {
                            nameindex++;
                        }
                        nametype = "";
                    }
                }

                    BaseAction *openT = new OpenTable(tID, Tcustomers);
                    update(line, openT, actindex,actionOrder);
                    actindex = (int)line.find_first_of(' ');
                    actionOrder=line.substr(0,line.find_first_of(' '));
                    continue;
            }
            else if (actionOrder == "move") {
                BaseAction *moveC=new MoveCustomer(tID, args.at(0)-'0', args.at(2)-'0');
                update(line,moveC,actindex,actionOrder);
                actindex = (int)line.find_first_of(' ');
                actionOrder=line.substr(0,line.find_first_of(' '));
                continue;
            }
            else if (actionOrder == "status") {
                BaseAction *stat=new PrintTableStatus(tID);
                update(line,stat,actindex,actionOrder);
                actindex = (int)line.find_first_of(' ');
                actionOrder=line.substr(0,line.find_first_of(' '));
                continue;
            }
        }
    }

    if (actionOrder == "closeall") {
        BaseAction *clsall=new CloseAll();
        clsall->act(*this);
        delete clsall;
    }
}

//aid function
void Restaurant::update(std::string &line, BaseAction *baseA, int actindex, std::string &actionOrder) {
    Restaurant &rest=*this;
    baseA->act(rest);
    actionsLog.push_back(baseA);
    baseA->setRest(line.substr(static_cast<unsigned long>(actindex + 1)));
    getline(cin,line);
}

//aid function
void Restaurant::setOpen(bool status) {
    open=status;
}


int Restaurant::getNumOfTables() const {
    return (int)tables.size();
}


Table* Restaurant::getTable(int ind) {
    if(ind<0 || ind>static_cast<int>(tables.size())) {
        return nullptr;
    } else{
        return tables[ind];
    }
}


const std::vector<BaseAction*>& Restaurant::getActionsLog() const {
    const std::vector<BaseAction*>& ans = actionsLog;
    return ans;
}


std::vector<Dish>& Restaurant::getMenu() {
    vector<Dish> &dishes = menu;
    return dishes;
}


////rule of 5
//Copy Constractor
Restaurant::Restaurant(const Restaurant &other) {
    copy(other);
}

//Copy Assignment
Restaurant& Restaurant::operator=(const Restaurant &other) {
    if(this!=&other){
        this->distract();
        copy(other);
    }
    return *this;
}

//Destructor
Restaurant::~Restaurant() {
    for (auto &i : actionsLog) {
        delete i;
    }
    actionsLog.clear();
    for (auto &table : tables) {
        delete table;
    }
    tables.clear();
    menu.clear();
    setOpen(false);
}

//Move Constractor
Restaurant::Restaurant(Restaurant &&other):open(), tables(), menu(), actionsLog() {
    this->steal(other);
}

//Move Assignment
Restaurant& Restaurant::operator=(Restaurant &&other) {
    if (this != &other) {
        this->distract();
        steal(other);
    }
    return *this;
}

//aid function
//steal
void Restaurant::steal(Restaurant &other) {
    open=other.open;
    tables=other.tables;
    actionsLog=other.actionsLog;
   for (auto &i : other.menu) {
      menu.emplace_back(i.getId(), i.getName(), i.getPrice(), i.getType());
   }
    other.distract();
}


//copy
void Restaurant::copy(const Restaurant& other) { //tables,menu,actionlog
    setOpen(other.open);
    for (auto table : other.tables) {
        tables.push_back(new Table(*table));
    }
    for (const auto &j : other.menu) {
        menu.emplace_back(Dish(j.getId(), j.getName(), j.getPrice(), j.getType()));
    }
    for (auto k : other.actionsLog) {
        actionsLog.push_back(k->clone());
    }
}


//clean
void Restaurant::distract() {
    for (int i = 0; i < static_cast<int>(actionsLog.size()) ; ++i) {
        delete this->actionsLog[i];
    }
    actionsLog.clear();
    for (auto &table : tables) {
        delete table;
    }
    tables.clear();
    menu.clear();
    setOpen(false);
}
