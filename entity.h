#ifndef ENTITY_H
#define ENTITY_H

#include "cake.h"

class Entity
{
public:
    Entity();
    virtual ~Entity();
//     void build_evaluation_map();
//     void calculate_total_evaluation(Cake *cake);
//     void print_total_evaluation();
    virtual void cut(Cake* cake){}
//     void choose(Cake* cake);
protected:
    Cake *m_cake;
};

#endif // ENTITY_H
