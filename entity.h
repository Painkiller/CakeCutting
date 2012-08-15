#ifndef ENTITY_H
#define ENTITY_H

#include "cake.h"

class Entity
{
public:
    Entity();
    virtual ~Entity();
//     void build_evaluation_map();
    virtual void calculateTotalEvaluation(){}
//     void print_total_evaluation();
    virtual void cut(Cake* cake){}
//     void choose(Cake* cake);
    inline bool isPlayer()
    {
	if(m_entity_type == PLAYER)
	    return true;
	return false;
    }
protected:
    Cake *m_cake;
    int m_entity_type;
};

#endif // ENTITY_H
