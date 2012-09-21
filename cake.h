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
    CakeCut()
    {
    }
    ~CakeCut(){}
    

    inline Entity* get_cutter(){return m_cutter;}
    inline int get_cut_sector(){return m_cut_sector;}
    inline float get_cut_point(){return m_cut_point;}
    inline void set_cakecut(Entity* cutter, int cut_sector, float cut_point)
    {
	m_cutter = cutter;
	m_cut_sector = cut_sector;
	m_cut_point = cut_point;
    }
    
private:
    Entity* m_cutter;
    int m_cut_sector;
    float m_cut_point;
};

class Piece
{
public:
    Piece()
    {
	m_left_cut = new CakeCut();
	m_right_cut = new CakeCut();
    }
    ~Piece()
    {
    }
    inline void set_piece(Entity *owner, CakeCut *left_cut, CakeCut *right_cut)
    {
      	m_owner = owner;
	m_left_cut = left_cut;
	m_right_cut = right_cut;
    }
    inline int get_left_cut_sector(){return m_left_cut->get_cut_sector();}
    inline int get_right_cut_sector(){return m_right_cut->get_cut_sector();}
    inline float get_left_cut_point(){return m_left_cut->get_cut_point();}
    inline float get_right_cut_point(){return m_right_cut->get_cut_point();}
    inline Entity* get_owner(){return m_owner;}
    inline void clear()
    {
    }
    inline CakeCut* get_ck_left(){return m_left_cut;}
    inline CakeCut* get_ck_right(){return m_right_cut;}
    
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
    inline int get_n_sector_type(){return m_sector_types;}
    
    inline CakeCut* get_cake_cut(int pos){return m_cake_cut[pos];}
    inline vector<CakeCut*> get_cake_cut_list(){return m_cake_cut;}
    
    inline void clear_cuts()
    {	
//       for(int i = 0; i < m_cake_cut.size(); i++)
// 	  delete m_cake_cut[i];
      m_cake_cut.clear();
      m_cut.clear();
    }
    inline void set_chosen(int chosen){m_chosen = chosen;}
    inline int get_chosen(){return m_chosen;}
    inline Piece* get_piece(int num)
    {
	switch(num)
	{
	   case 0:
	      return m_first_piece;
	   case 1:
	      return m_second_piece;
	   case 2:
	      return m_third_piece;	  
	   default :
	      return m_middle_piece;
	}
    }
    void buildCake(int problem);
    void setCakeCut(Entity *cutter, int sector, float point);
    void printSectors();
    
    int countSectorOccurrences(int sec_type);

private:
    int m_chosen;
    int m_sector_types;
    float m_partial;
    
    vector<int> m_sectors;
    vector<int> m_cut;
    
    vector<CakeCut*> m_cake_cut;
    
    Piece *m_first_piece;
    Piece *m_second_piece;
    Piece *m_third_piece;
    Piece *m_middle_piece;
};
#endif // CAKE_H
