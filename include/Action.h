//
// Created by gorjalts@wincs.cs.bgu.ac.il on 11/9/18.
//

#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};

//Forward declaration
class Restaurant;

class BaseAction{
public:
    BaseAction();
    ActionStatus getStatus() const;
    void setStatus(ActionStatus otherstatus);//ours
    virtual void act(Restaurant& restaurant)=0;
    virtual std::string toString() const=0;
    void setRest(std::string str); //ours
    std::string getREst() const; //ours
    virtual BaseAction* clone()=0;//ours
    virtual ~BaseAction(); //ours
    void setError (std::string errormsg);//ours
protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
    std::string rest; //ours

};


class OpenTable : public BaseAction {
public:
    OpenTable(int id, std::vector<Customer *> &customersList);
    void act(Restaurant &restaurant);
    std::string toString() const;
    ~OpenTable();
    OpenTable(const OpenTable &other); //copy constractor
    std::vector<Customer *> operator=(OpenTable &&other); //move
    void copy (const OpenTable& other);//ours
    BaseAction* clone() override;//ours
private:
    const int tableId;
    std::vector<Customer *> customers;
};


class Order : public BaseAction {
public:
    Order(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;
    ~Order();//destructor
    BaseAction* clone() override;//ours
private:
    const int tableId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    void act(Restaurant &restaurant);
    std::string toString() const;
    ~MoveCustomer();
    BaseAction* clone()override;//ours
private:
    const int srcTable;
    const int dstTable;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    void act(Restaurant &restaurant);
    ~Close();
    std::string toString() const;
    BaseAction* clone() override;//ours
private:
    const int tableId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    void act(Restaurant &restaurant);
    ~CloseAll();
    std::string toString() const;
    BaseAction* clone() override;//ours
private:
};


class PrintMenu : public BaseAction {
public:
    PrintMenu();
    void act(Restaurant &restaurant);
    ~PrintMenu();
    std::string toString() const;
    BaseAction* clone() override;//ours
private:
};


class PrintTableStatus : public BaseAction {
public:
    PrintTableStatus(int id);
    void act(Restaurant &restaurant);
    ~PrintTableStatus();
    std::string toString() const;
    BaseAction* clone() override;//ours
private:
    const int tableId;
};



class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    void act(Restaurant &restaurant);
    ~PrintActionsLog();
    std::string toString() const;
    BaseAction* clone() override;//ours
private:
};


class BackupRestaurant : public BaseAction {
public:
    BackupRestaurant();
    void act(Restaurant &restaurant);
    ~BackupRestaurant();
    std::string toString() const;
    BaseAction* clone() override;//ours
private:
};


class RestoreResturant : public BaseAction {
public:
    RestoreResturant();
    void act(Restaurant &restaurant);
    std::string toString() const;
    BaseAction* clone() override;//ours
    ~RestoreResturant();
private://we added
};


#endif
