//
// Created by gorjalts@wincs.cs.bgu.ac.il on 11/9/18.
//

#ifndef TABLE_H_
#define TABLE_H_

#include <vector>
#include "Customer.h"
#include "Dish.h"

typedef std::pair<int, Dish> OrderPair;

class Table{
public:
    Table();//ours
    Table(int t_capacity);
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    void eraseCustomer(int id);//ours
    void eraseOrder(int id);//ours
    std::vector<OrderPair> removeOrders(int id);//ours
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const std::vector<Dish> &menu);
    void setorder(const std::vector<OrderPair> &orders);//ours
    void openTable();
    void closeTable();
    int getBill();
    bool isOpen();
    std::string Tstatus();//ours
    //rule of 5
    virtual ~Table();
    Table(const Table &other);
    Table (Table &&other);
    Table& operator=(const Table &other);
    Table& operator=(Table &&other);
    void steal (Table &other);
    void copy (const Table& other);
    void distract();
private:
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order in a table - (customer_id, Dish)
};


#endif
