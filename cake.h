#ifndef CAKE_H
#define CAKE_H

#include <iostream>
#include <vector>

#include "shared.h"

using namespace std;

struct CakeCut
{
    string m_id;
    int m_sector;
    float m_part;
};

class Cake
{

public:
    Cake();
    virtual ~Cake();
    
    void build_cake();
    int get_type_at(int pos);
    
    inline void set_cake_cut(int cut, float partial, string player_id)
    {
	CakeCut ck;
	
	ck.m_sector = cut;
	ck.m_part = partial;
	ck.m_id = player_id;
	
	m_cake_cut.push_back(ck);
	cout << "Player A cuts the cake at sector " << cut << " with percentage " << partial << endl;
    }

    inline CakeCut get_cake_cut(int pos){return m_cake_cut[pos];}

    inline void clear_cuts(){m_cut.clear();}
	
    void print_sectors();

private:
    float m_partial;
    
    vector<int> m_sectors;
    vector<int> m_cut;
    
    vector<CakeCut> m_cake_cut;
};
#endif // CAKE_H
