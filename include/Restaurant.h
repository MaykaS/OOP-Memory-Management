//
// Created by gorjalts@wincs.cs.bgu.ac.il on 11/9/18.
//

#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"


class Restaurant{
public:
    Restaurant();
    Restaurant(const std::string &configFilePath);
    void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu();
    void CreateTables(std::string &capacity);//ours
    void CreateMenu(std::string &line);//ours
    void setOpen(bool status); //our
    DishType Change(std::string &str);//ours
    void update(std::string &line, BaseAction *baseA, int actindex, std::string &actoinOrder);//ours
   //rule of 5
    virtual ~Restaurant();
    Restaurant(const Restaurant &other);
    Restaurant (Restaurant &&other);
    Restaurant& operator=(const Restaurant &other);
    Restaurant& operator=(Restaurant &&other);
    void steal (Restaurant &other);
    void copy (const Restaurant& other);
    void distract();
private:
    bool open;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
};

#endif
