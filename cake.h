#ifndef CAKE_H
#define CAKE_H

#include <iostream>
#include <vector>

#include "shared.h"

using namespace std;
class Entity;
class Player;
class Referee;

class CakeCut
{
public:
    CakeCut(Entity *cutter, int cut_sector, float cut_point)
    {
	m_cutter = cutter;
	m_cut_sector = cut_sector;
	m_cut_point = cut_point;
    }
    ~CakeCut(){}
    

    inline Entity* get_cutter(){return m_cutter;}
    inline int get_cut_sector(){return m_cut_sector;}
    inline float get_cut_point(){return m_cut_point;}
    
private:
    Entity* m_cutter;
    int m_cut_sector;
    float m_cut_point;
};

class Piece
{
public:
    Piece(Entity *owner, CakeCut *left_cut, CakeCut *right_cut)
    {
	m_owner = owner;
	m_left_cut = left_cut;
	m_right_cut = right_cut;
    }
    ~Piece()
    {
	delete m_left_cut;
	delete m_right_cut;
    }
    inline int get_left_cut_sector(){return m_left_cut->get_cut_sector();}
    inline int get_right_cut_sector(){return m_right_cut->get_cut_sector();}
    inline float get_left_cut_point(){return m_left_cut->get_cut_point();}
    inline float get_right_cut_point(){return m_right_cut->get_cut_point();}
    inline Entity* get_owner(){return m_owner;}
    
private:
    Entity *m_owner;
    CakeCut *m_left_cut;
    CakeCut *m_right_cut;
};

class Cake
{

public:
    Cake();
    virtual ~Cake();
    
    inline int get_type_at(int pos){return m_sectors[pos];}
    inline int get_size(){return m_sectors.size();}
    
    inline CakeCut* get_cake_cut(int pos){return m_cake_cut[pos];}
    inline vector<CakeCut*> get_cake_cut_list(){return m_cake_cut;}
    
    inline void clear_cuts()
    {	
      for(int i = 0; i < m_cake_cut.size(); i++)
	  delete m_cake_cut[i];
      m_cake_cut.clear();
    }
	
    void buildCake();
    void setCakeCut(Entity *cutter, int sector, float point);
    void printSectors();

private:
    float m_partial;
    
    vector<int> m_sectors;
    vector<int> m_cut;
    
    vector<CakeCut*> m_cake_cut;
};
#endif // CAKE_H
