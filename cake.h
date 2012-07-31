#ifndef CAKE_H
#define CAKE_H

#include <iostream>
#include <vector>

#include "shared.h"

using namespace std;

class Cake
{

public:
    Cake();
    virtual ~Cake();
    
    void build_cake();
    int get_type_at(int pos);
    
    inline int get_cut(int pos){return m_cut.at(pos);}
    inline void set_cut(int cut, float partial)
    {
	m_cut.push_back(cut);
	m_partial = partial;
	cout << "Player A cuts the cake at sector " << cut << " with percentage " << m_partial << endl;
    }

    inline float get_partial(){return m_partial;}
    
    inline void clear_cuts(){m_cut.clear();}
	
    void print_sectors();

    
private:
    int *m_sectors;
    vector<int> m_cut;
    float m_partial;
};

#endif // CAKE_H
