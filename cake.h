#ifndef CAKE_H
#define CAKE_H
#include <iostream>
#include "shared.h"

using namespace std;

class Cake
{

public:
    Cake();
    virtual ~Cake();
    
    void build_cake();
    int get_type_at(int pos);
    
    void set_cut(int cut, float partial);
    
    inline int get_cut(){return m_cut;}
    inline float get_partial(){return m_partial;}
    void print_sectors();
private:
    int *m_sectors;
    int m_cut;
    float m_partial;
};

#endif // CAKE_H
