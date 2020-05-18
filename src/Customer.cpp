#include <utility>

//
// Created by gorjalts@wincs.cs.bgu.ac.il on 11/9/18.
//
#include "../include/Customer.h"
#include <iostream>
#include <vector>
#include <string>
#include "../include/Restaurant.h"


Customer::Customer():name(),id() {}
Customer::Customer(std::string c_name, int c_id) : name(std::move(c_name)), id(c_id){}


std::string Customer::getName() const {
    return name;
}


int Customer::getId() const {
    return id;
}


Customer::~Customer()=default;


////VegetarianCustomer
VegetarianCustomer::VegetarianCustomer(std::string name, int id):Customer(name,id) {}

std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu) {
    std::vector <int> vegOrd;
    int  foodIndex =(int)menu.size()+1;
    int drinkIndex= -1;
    int priceCount=-1;
    for (int i = 0; i <(int)menu.size() ; ++i) {
        if(menu.at(static_cast<unsigned long>(i)).getId() <= foodIndex && menu.at(static_cast<unsigned long>(i)).getType() == VEG)
            foodIndex=i;
        if(menu.at(static_cast<unsigned long>(i)).getPrice() > priceCount && menu.at(static_cast<unsigned long>(i)).getType() == BVG){
            drinkIndex=i;
            priceCount=menu.at(static_cast<unsigned long>(i)).getPrice();
        }
    }
    if(foodIndex<(int)menu.size()+1 && drinkIndex>-1){

        vegOrd.push_back(foodIndex);
        vegOrd.push_back(drinkIndex);
    }
    return vegOrd; //orders forever!
}


std::string VegetarianCustomer::toString() const {
    return std::to_string(getId()) + " " + getName();
}


Customer* VegetarianCustomer::clone() {
VegetarianCustomer *veg = new VegetarianCustomer(this->getName(),this->getId());
    return veg;
}


////CheapCustomer
CheapCustomer::CheapCustomer(std::string name, int id):Customer(name,id), ordered(false) {}

std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu) {
    std::vector <int> chpOrd;
    int dishIndex = (int)menu.size()+1;
    int priceCount=menu.at(0).getPrice();
    for (int i = 1; i <(int)menu.size() ; ++i) {
        if(menu.at(static_cast<unsigned long>(i)).getPrice() < priceCount){
            dishIndex=i;
            priceCount=menu.at(static_cast<unsigned long>(i)).getPrice();
        }
    }
    if(dishIndex!=(int)menu.size()+1 && !ordered) {
        chpOrd.push_back(dishIndex); //orderes only the cheapest one AND once!
        ordered= true;
    }
    return chpOrd;
}


std::string CheapCustomer::toString() const {
    return std::to_string(getId())+" "+getName();
}


Customer* CheapCustomer::clone() {
    CheapCustomer *chip=new CheapCustomer(this->getName(),this->getId());
    chip->setordered(this->getordered());
    return chip;
}


void CheapCustomer::setordered(bool status) {
    this->ordered=status;
}


bool CheapCustomer::getordered() {
    return this->ordered;
}


//// SpicyCustomer
SpicyCustomer::SpicyCustomer(std::string name, int id):Customer(name,id), SPCbool(false) {}

std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> spyStr;
    std::vector<int> spyOrd;
    int foodIndex = (int) menu.size() + 1;
    int drinkIndex = -1;
    int foodPriceCount = -1;
    int drinkPriceCount=-1;
    int a = 0;
    while (menu[a].getType() != BVG) {// finding first BVG
        a = a + 1;
    }
    if (a < static_cast<int>(menu.size())) {
       drinkPriceCount = menu[a].getPrice();
        drinkIndex = a;
    }
    for (int i = 0; i <(int) menu.size(); ++i) {
        if (menu[i].getPrice() > foodPriceCount && menu[i].getType() == SPC) {
            foodIndex = i;
            foodPriceCount = menu.at(static_cast<unsigned long>(i)).getPrice();
        }
        if (menu[i].getPrice() < drinkPriceCount && menu.at(static_cast<unsigned long>(i)).getType() == BVG) {
            drinkIndex = i;
            drinkPriceCount = menu.at(static_cast<unsigned long>(i)).getPrice();
        }
    }
    if(foodIndex!=(int)menu.size()+1 && drinkIndex!=-1) {
        spyStr.push_back(foodIndex); //orders one time spicy and forever cheap drink!
        spyStr.push_back(drinkIndex);

        if (!SPCbool) {
            spyOrd.push_back(spyStr.at(0));
            SPCbool = true;
        } else spyOrd.push_back(spyStr.at(1));
    }
    return spyOrd;
}


std::string SpicyCustomer::toString() const {
    return std::to_string(getId()) +" "+getName();
}


Customer* SpicyCustomer::clone() {
    SpicyCustomer *spicy= new SpicyCustomer(this->getName(),this->getId());
    spicy->setSPcbool(this->getSPCbool());
    return  spicy;
}


void SpicyCustomer::setSPcbool(bool status) {
    this->SPCbool=status;
}


bool SpicyCustomer::getSPCbool() {
    return SPCbool;
}


//// AlchoholicCustomer:
AlchoholicCustomer::AlchoholicCustomer(std::string name, int id):Customer(name,id), ALCcounter(0){}

std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) {
    std::vector<Dish> alcStr;
    std::vector<int> alcOrd;
    for (const auto &i : menu) {
        if(i.getType() == ALC)
            alcStr.push_back(i);
    }
// sorting the strategy vector
    std::vector<int> SortedId;
    int min=0;
    int priceIndex= alcStr[0].getPrice();
    while(!alcStr.empty()) {
        for (int j = 0; j < (int)alcStr.size(); ++j) {
            if (alcStr[j].getPrice() < priceIndex) {
                min = j;
                priceIndex = alcStr[j].getPrice();
            }
        }
        SortedId.push_back(alcStr[min].getId());
        erase(min, alcStr);
        priceIndex= alcStr[0].getPrice();
    }
// end of the sorting
    if(ALCcounter<(int)SortedId.size()){
        alcOrd.push_back(SortedId[ALCcounter]);
        ALCcounter=ALCcounter+1;
    }
    return alcOrd;
}


void AlchoholicCustomer::erase(int place, std::vector<Dish> &strvec) {
    std::vector<Dish> temp;
    for (int i = 0; i <(int)strvec.size() ; ++i) {
        if(i!=place)
            temp.push_back(strvec[i]);
    }
    strvec.clear();
    for (int j = 0; j <(int)temp.size() ; ++j) {
        strvec.push_back(temp[j]);
    }
}


std::string AlchoholicCustomer::toString() const {
    return std::to_string(getId()) +" "+getName();
}


Customer* AlchoholicCustomer::clone() {
    AlchoholicCustomer *alcho=new AlchoholicCustomer(this->getName(),this->getId());
    alcho->setALCcounter(this->getALCcounter());
    return alcho;
}


void AlchoholicCustomer::setALCcounter(int num) {
    this->ALCcounter=num;
}


int AlchoholicCustomer::getALCcounter() {
    return this->ALCcounter;
}