//
// Created by gorjalts@wincs.cs.bgu.ac.il on 11/9/18.
//
#include "../include/Table.h"
#include "../../../../../../../../usr/include/c++/7/bits/stl_pair.h"
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fcntl.h>


Table::Table(): capacity(-1), open(false), customersList(), orderList() {}

Table::Table(int t_capacity) : capacity(t_capacity), open(false), customersList(), orderList() {}


int Table::getCapacity() const {
    return capacity;
}


void Table::addCustomer(Customer *customer) {
    customersList.push_back(customer);
}


void Table::removeCustomer(int id) {
    int index = -1;
    for (int i = 0;i <static_cast<int>(customersList.size()); ++i) {
        if (customersList[i]->getId() == id)
            index = i;
    }
    if (index != -1) {
        eraseCustomer(id);//remove from customer list
        eraseOrder(id);//remove from order list
    }
}


//aid function
std::vector<OrderPair> Table::removeOrders(int id) {
    std::vector <OrderPair> move;
    for (int i = 0; i <static_cast<int>(orderList.size()) ; ++i) {
        if(orderList[i].first==id) {
            move.push_back(orderList[i]);
        }
    }
    eraseOrder(id);//remove from order list
    return move;
}


//aid function
void Table::eraseCustomer(int id) {
    std::vector <Customer*> temp;
    for (auto &i : customersList) {
        if(i->getId()!=id){
            temp.push_back(i->clone());
        }
        delete i;
    }
    customersList.clear();
    for (int j = 0; j <static_cast<int>(temp.size()) ; ++j) {
        customersList.push_back(temp[j]);
    }
    temp.clear();
}

//aid function
void Table::eraseOrder(int id) {
    std::vector <OrderPair> temp;
    for (auto &i : orderList) {
        if (i.first != id) {
            temp.push_back(i);
        }
    }
    orderList.clear();
    if(!temp.empty()) {
        for (const auto &j : temp) {
            orderList.push_back(j);
        }
    }
}



Customer* Table::getCustomer(int id) {
    int index = -1;
    for (int i = 0; i < static_cast<int>(customersList.size()); ++i) {
        if (customersList[i]->getId() == id)
            index = i;
    }
    if (index != -1) {
        return customersList[index];
    } //return the customer
    else return nullptr;
}


std::vector<Customer*>& Table::getCustomers(){
    std::vector<Customer*>&ans=customersList;
    return ans;
}


std::vector<OrderPair>& Table::getOrders() {
    std::vector<OrderPair>&ans=orderList;
    return ans;
}


void Table::order(const std::vector<Dish> &menu) {
    for (int i = 0; i <static_cast<int>(getCustomers().size()) ; ++i) {
        std::vector<int>Corder= getCustomers()[i]->order(menu);
        if (static_cast<int>(Corder.size()) == 1) {
            orderList.emplace_back(OrderPair(getCustomers()[i]->getId(), menu[Corder[0]]));
            std::cout<< getCustomers()[i]->getName()+" ordered "+menu[Corder[0]].getName()<<std::endl;

        }
        if (static_cast<int>(Corder.size()) == 2) {
            orderList.emplace_back(getCustomers()[i]->getId(), menu[Corder[0]]);
            std::cout<< getCustomers()[i]->getName()+" ordered "+menu[Corder[0]].getName()<<std::endl;
            orderList.emplace_back(getCustomers()[i]->getId(), menu[Corder[1]]);
            std::cout<< getCustomers()[i]->getName()+" ordered "+menu[Corder[1]].getName()<<std::endl;
        }
    }
}


void Table::setorder(const std::vector<OrderPair> &orders) {
    for (const auto &order : orders) {
        orderList.push_back(order);
    }
}


void  Table:: openTable() {
    this->open = true;
}


void  Table:: closeTable() {
    for (Customer* customer : customersList)
       delete customer;
    customersList.clear();
    orderList.clear();
    open = false;
}


int Table::getBill() {
    int bill = 0;
    for (auto &i : orderList) {
        bill = bill + i.second.getPrice();
    }
    return bill;
}


bool Table::isOpen() {
    return open;
}


std::string Table::Tstatus() {
            if(open){
                return "open";
            }
            else{
                return"close";
            }
}


////rule of 5
// Destructor
Table::~Table() {
    orderList.clear();

    for (auto &i : customersList) {
            delete i;
    }
    customersList.clear();

    open= false;
    capacity=0;
}

//copy constractor
Table::Table(const Table &other) {
    copy(other);
}

//copy assignment
Table& Table::operator=(const Table &other) {
    if(this!=&other) {
        this->distract();
        copy(other);
    }
    return *this;
}

//move constractor
Table::Table(Table &&other): capacity(), open(), customersList(), orderList() {
    this->copy(other);
}

//move assignment
Table& Table::operator=(Table &&other) {
    if(this!=&other){
        this->distract();
        steal(other);
    }
    return *this;
}


//aid function
//steal
void Table::steal(Table &other) {
    open = other.open;
    capacity = other.getCapacity();
    customersList=other.customersList;
    for (const auto &j : other.orderList) {
        this->orderList.push_back(j);
    }
}

//copy
void Table::copy(const Table &other) {
    open=other.open;
    capacity=other.getCapacity();
    for (auto i : other.customersList) {
        this->customersList.push_back(i->clone());
    }
    for (const auto &j : other.orderList) {
        this->orderList.push_back(j);
    }
}

//clean ////same as destructor
void Table::distract() {
    for (auto &j : customersList) {
        eraseOrder(j->getId());
        delete  j;
    }
    orderList.clear();

    for (auto &i : customersList) {
        delete i;
    }
    customersList.clear();
    open= false;
    capacity=0;
}