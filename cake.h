#ifndef CAKE_H
#define CAKE_H

#include <iostream>
#include <vector>

#include "shared.h"

using namespace std;

struct CakeCut
{
    string m_cutter_id;
    int m_sector;
    float m_point;
};

struct Piece
{
    CakeCut m_begin;
    CakeCut m_end;
};

class Cake
{

public:
    Cake();
    virtual ~Cake();
    
    void buildCake();
    
    inline int get_type_at(int pos){return m_sectors[pos];}
    inline void set_cake_cut(int sector, float point, string player_id)
    {
	CakeCut ck;
	
	ck.m_sector = sector;
	ck.m_point = point;
	ck.m_cutter_id = player_id;
	
	m_cake_cut.push_back(ck);
	cout << "Player " << player_id << " cuts the cake at sector " << sector << " at point " << point << endl;
    }
    inline int get_size(){return m_sectors.size();}
    
    inline CakeCut get_cake_cut(int pos){return m_cake_cut[pos];}
    inline vector<CakeCut> get_cake_cut_list(){return m_cake_cut;}
    
    inline void clear_cuts(){m_cake_cut.clear();}
	
    void printSectors();

private:
    float m_partial;
    
    vector<int> m_sectors;
    vector<int> m_cut;
    
    vector<CakeCut> m_cake_cut;
};
#endif // CAKE_H
