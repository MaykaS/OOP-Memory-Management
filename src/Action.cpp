//
// Created by gorjalts@wincs.cs.bgu.ac.il on 11/9/18.
//

#include "../include/Action.h"
#include <string>

#include <vector>
#include <iostream>
#include "../include/Customer.h"
#include "../include/Table.h"
#include "../include/Restaurant.h"
using namespace std;

extern Restaurant* backup;


BaseAction::BaseAction(): errorMsg(""), status(PENDING)  {}

ActionStatus BaseAction::getStatus() const {
    return status;
}


void BaseAction::setStatus(ActionStatus otherstatus) {
    this->status=otherstatus;
}


void BaseAction::complete() {
    status=COMPLETED;
}


void BaseAction::error(std::string errorMsg) {
   this->errorMsg=errorMsg;
    status=ERROR;
    cout<<"Error: "+this->errorMsg<<endl;
}


std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}


void BaseAction::setError(std::string errormsg) {
    this->errorMsg=errormsg;
}


std::string BaseAction::getREst() const {
    return rest;
}


void BaseAction::setRest(std::string str) {
    rest=str;
}


BaseAction::~BaseAction() = default;


/// OpenTable
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList):BaseAction(), tableId(id), customers(customersList) {}

void OpenTable::act(Restaurant &restaurant) {
    if (tableId < restaurant.getNumOfTables() && tableId >= 0 && (!(restaurant.getTable(tableId))->isOpen())) {
        if (restaurant.getTable(tableId)->getCapacity() >= (int)customers.size()) {
            restaurant.getTable(tableId)->openTable();
            for (auto &customer : customers) {
                restaurant.getTable(tableId)->addCustomer(customer);
            }
        }
        complete();
    } else {
        error("Table does not exist or is already open");
        for(Customer* customer : customers){
            delete customer;
        }
    }
    customers.clear();
}


std::string OpenTable::toString() const {
    if(this->getStatus()==COMPLETED) {
        return "open " + this->getREst()+" Completed";
    }else{
        return "open "+ this->getREst()+" Error: Table does not exist or is already open";
    }
}


//destructor
 OpenTable::~OpenTable() {
     for (auto &customer : customers) {
         delete customer;
     }
     customers.clear();
}

//copy constructor
OpenTable::OpenTable(const OpenTable &other):tableId() {
    copy(other);
}

//copy
void OpenTable::copy(const OpenTable &other) {
    for (auto customer : other.customers) {
        this->customers.push_back(customer->clone());
    }
}

//move constructor
vector<Customer *> OpenTable::operator=(OpenTable &&other) {
    for (auto customer : other.customers) {
        customers.push_back(customer);
    }
    return customers;
}


BaseAction* OpenTable::clone() {
    std::vector<Customer *> temp;
    for (auto customer : customers) {
        temp.push_back(customer);
    }
    std::vector<Customer *> &ref=temp;
    OpenTable *act = new OpenTable(tableId,ref);
    act->setStatus(this->getStatus());
    act->setError(this->getErrorMsg());
    act->setRest(this->getREst());
    return act;
}


//// Order
Order::Order(int id): BaseAction(), tableId(id) {}

void Order::act(Restaurant &restaurant) {
    Table *t = restaurant.getTable(tableId);
    if (!t->isOpen() || tableId > restaurant.getNumOfTables()) {
        error("Table does not exist or is not open");
    } else {
       t->order(restaurant.getMenu());
       complete();
    }
}


std::string Order::toString() const {
    if(getStatus()==COMPLETED){
        return "order "+to_string(tableId)+" Completed";
    }
    else{
        return "order "+to_string(tableId)+" Error: Table does not exist or is not open";
    }
}


BaseAction* Order::clone() {
    Order *act= new Order(tableId);
    act->setStatus(this->getStatus());
    act->setError(this->getErrorMsg());
    act->setRest(this->getREst());
    return act;
}

Order::~Order() {}



//// MoveCustomer
MoveCustomer::MoveCustomer(int src, int dst, int customerId):BaseAction(), srcTable(src), dstTable(dst),id(customerId){}

void MoveCustomer::act(Restaurant &restaurant) {
    if (srcTable > restaurant.getNumOfTables() || srcTable < 0 || dstTable > restaurant.getNumOfTables() || dstTable < 0) {
        return error("Cannot move customer");
    }
    Table *srcT = restaurant.getTable(srcTable);
    Table *dstT = restaurant.getTable(dstTable);

    if ((!srcT->isOpen()) || (!dstT->isOpen()) || (int)dstT->getCustomers().size() == dstT->getCapacity() || srcT->getCustomer(id) == nullptr) {
        return error("Cannot move customer");
    } else {
        dstT->addCustomer(srcT->getCustomer(id)->clone());
        std::vector<OrderPair> ToMove = srcT->removeOrders(id);/// take only his orders
        dstT->setorder(ToMove); /// add his orders to the dst table

        restaurant.getTable(srcTable)->removeCustomer(id);
        srcT->eraseOrder(id);
        if(srcT->getCustomers().empty()){
            srcT->closeTable();
        }
        complete();
    }
}


std::string MoveCustomer::toString() const {
    if(getStatus()==COMPLETED)
    return "move "+to_string(srcTable)+" "+to_string(dstTable)+" "+to_string(id)+" Completed";
    else return "move "+to_string(srcTable)+" "+to_string(dstTable)+" "+to_string(id)+" Error: "+getErrorMsg();
}


BaseAction* MoveCustomer::clone() {
    MoveCustomer *act=new MoveCustomer(srcTable, dstTable, id);
    act->setStatus(this->getStatus());
    act->setError(this->getErrorMsg());
    act->setRest(this->getREst());
    return act;
}

MoveCustomer::~MoveCustomer(){}


//// Close
Close::Close(int id): BaseAction(), tableId(id) {}

void Close::act(Restaurant &restaurant) {
    Table *t = restaurant.getTable(tableId);
    if (!t->isOpen() || tableId > restaurant.getNumOfTables() || tableId < 0) {
        error("Table does not exist or is not open");
    } else {
        cout << "Table " + to_string(tableId) + " was closed. Bill " + to_string(t->getBill()) + "NIS" << endl;
        t->closeTable();
        complete();
    }
}


std::string Close::toString() const {
    if (getStatus() == ERROR)
        return "close "+to_string(tableId)+ "Error: "+getErrorMsg();
    else return "close "+to_string(tableId)+" Completed";
}


BaseAction* Close::clone() {
    Close *act= new Close(tableId);
    act->setStatus(this->getStatus());
    act->setError(this->getErrorMsg());
    act->setRest(this->getREst());
    return act;
}

Close::~Close(){}


////CloseAll
CloseAll::CloseAll(): BaseAction() {}

void CloseAll::act(Restaurant &restaurant) {
    for(int i=0; i<restaurant.getNumOfTables(); i++){
        if(restaurant.getTable(i)->isOpen()){
            Close(i).act(restaurant); //close and print
        }
    }
}


std::string CloseAll::toString() const {
    return "";
}


BaseAction* CloseAll::clone() {
    CloseAll *act= new CloseAll();
    act->setStatus(this->getStatus());
    act->setError(this->getErrorMsg());
    act->setRest(this->getREst());
    return act;
}

CloseAll::~CloseAll() {}


////PrintMenu
PrintMenu::PrintMenu():BaseAction() {}

void PrintMenu::act(Restaurant &restaurant) {
    std::vector<Dish> menutoprint;
    for (const auto &j : restaurant.getMenu()) {
        menutoprint.push_back(j);
    }
    for (auto &i : menutoprint) {
        if (i.getType() == 0) {
            cout << i.getName() + " VEG " + to_string(i.getPrice()) + "NIS" << endl;
        }
        if (i.getType() == 1) {
            cout << i.getName() + " SPC "+ to_string(i.getPrice()) + "NIS" << endl;
        }
        if (i.getType() == 2) {
            cout << i.getName() + " BVG "+ to_string(i.getPrice()) + "NIS" << endl;
        }
        if (i.getType() == 3) {
            cout << i.getName() + " ALC " + to_string(i.getPrice()) + "NIS" << endl;
        }
    }
    complete();
}


std::string PrintMenu::toString() const {
    return "menu Completed";
}


BaseAction* PrintMenu::clone() {
    PrintMenu *act= new PrintMenu();
    act->setStatus(this->getStatus());
    act->setError(this->getErrorMsg());
    act->setRest(this->getREst());
    return act;
}

PrintMenu::~PrintMenu() {}


////PrintTableStatus
PrintTableStatus::PrintTableStatus(int id):BaseAction(), tableId(id){}

void PrintTableStatus::act(Restaurant &restaurant) {
    Table *t = restaurant.getTable(tableId);
    if(t->isOpen()){
        cout<<"Table "+to_string(tableId)+" status: "+t->Tstatus()<<endl;
        if(!t->getCustomers().empty()) {
            cout << "Customers:" << endl;
        }
        std::vector<Customer*> Tcustomer=t->getCustomers();
        std::vector<int> Cid;
        for (auto &i : Tcustomer) {
            Cid.push_back(i->getId());
        }
        for (int j : Cid) {
            cout<<to_string(j)+" "+t->getCustomer(j)->getName()<<endl;
        }
        cout << "Orders:" << endl;
        for (auto &k : t->getOrders()) {
            cout<< k.second.getName()+" "+ to_string(k.second.getPrice())+"NIS "+ to_string(k.first)<<endl;
        }
        cout<<"Current Bill: "+to_string(t->getBill())+"NIS"<<endl;
    }
    else{
        cout<<"Table "+to_string(tableId)+" status: closed"<<endl;
    }
    complete();
}


std::string PrintTableStatus::toString() const {
    return "status "+to_string(tableId)+" Completed";
}


BaseAction* PrintTableStatus::clone() {
    PrintTableStatus *act= new PrintTableStatus(tableId);
    act->setStatus(this->getStatus());
    act->setError(this->getErrorMsg());
    act->setRest(this->getREst());
    return act;
}

PrintTableStatus::~PrintTableStatus() {}


////PrintActionLog
PrintActionsLog::PrintActionsLog():BaseAction() {}

void PrintActionsLog::act(Restaurant &restaurant) {
    for (auto i : restaurant.getActionsLog()) {
        cout<< i->toString()<<endl;
    }
    complete();
}


std::string PrintActionsLog::toString() const {
    return "log Completed";
}


BaseAction* PrintActionsLog::clone() {
    PrintActionsLog *act= new PrintActionsLog();
    act->setStatus(this->getStatus());
    act->setError(this->getErrorMsg());
    act->setRest(this->getREst());
    return act;
}

PrintActionsLog::~PrintActionsLog() {}


////BackupRestaurant
BackupRestaurant::BackupRestaurant():BaseAction() {}

void BackupRestaurant::act(Restaurant &restaurant) {
    if (backup == nullptr) {
        backup = new Restaurant(restaurant);
    } else {
        delete backup;
        *backup = (restaurant);
    }
    complete();
}


std::string BackupRestaurant::toString() const {
    return "backup Completed";
}


BaseAction* BackupRestaurant::clone() {
    BackupRestaurant *act= new BackupRestaurant();
    act->setStatus(this->getStatus());
    act->setError(this->getErrorMsg());
    act->setRest(this->getREst());
    return act;
}

BackupRestaurant::~BackupRestaurant() {}


////RestoreRestaurant
RestoreResturant::RestoreResturant():BaseAction() {}

void RestoreResturant::act(Restaurant &restaurant) {
    if (backup == nullptr) error("No backup available");
    else {
        restaurant.distract();
        restaurant = *backup;
        complete();
    }
}


std::string RestoreResturant::toString() const {
    if(getStatus()==ERROR)
        return "restore Error: "+getErrorMsg();
    else return "restore Completed";
}


BaseAction* RestoreResturant::clone() {
    RestoreResturant *act= new RestoreResturant();
    act->setStatus(this->getStatus());
    act->setError(this->getErrorMsg());
    act->setRest(this->getREst());
    return act;
}

RestoreResturant::~RestoreResturant() {}
