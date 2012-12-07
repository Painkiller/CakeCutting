#ifndef ENTITY_H
#define ENTITY_H

#include "cake.h"

class Entity
{
public:
    Entity();
    virtual ~Entity();
    virtual void calculateTotalEvaluation(){}
    virtual void cut(Cake* cake){}
    virtual void set_piece(Piece* piece){};
    virtual string& get_id(){return m_id;}
    
    virtual CakeCut* getCakecut(){};
    inline bool isPlayer()
    {
	if(m_entity_type == PLAYER)
	    return true;
	return false;
    }
protected:
    Cake *m_cake;
    int m_entity_type;
    string m_id;
};

#endif // ENTITY_H
