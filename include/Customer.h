//
// Created by gorjalts@wincs.cs.bgu.ac.il on 11/9/18.
//

#ifndef CUSTOMER_H_
#define CUSTOMER_H_


#include <vector>
#include <string>
#include "Dish.h"

class Customer{

public:
    Customer();//ours
    Customer(std::string c_name, int c_id);
    virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    virtual ~Customer(); //ours
    int getId() const;
    virtual Customer* clone()=0;//ours
private:
    const std::string name;
    const int id;
};


class VegetarianCustomer : public Customer {
public:
    VegetarianCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer* clone() override;//ours
private:
};


class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer* clone() override;//ours
    void setordered(bool status);//ours
    bool getordered();//ours
private:
    bool ordered;//ours

};


class SpicyCustomer : public Customer {
public:
    SpicyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer* clone() override;//ours
    void setSPcbool(bool status);//ours
    bool getSPCbool();//ours
private:
    bool SPCbool;//ours
};


class AlchoholicCustomer : public Customer {
public:
    AlchoholicCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    void erase(int place, std::vector<Dish> &strvec);///ours
    Customer* clone() override;//ours
    void setALCcounter(int num);//ours
    int getALCcounter();//ours
private:
    int ALCcounter;//ours
};


#endif
