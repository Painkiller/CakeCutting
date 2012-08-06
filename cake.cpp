#include "cake.h"
#include <stdlib.h>

Cake::Cake()
{
  
}

Cake::~Cake()
{

}

void Cake::build_cake()
{
    int value;
    int values[8] = {3,4,4,1,4,4,3,2}; 
    
    for(int i = 0; i < N_SECTORS; i++)
    {
	value = rand() % N_SECTOR_TYPE;
	m_sectors.push_back(value);
// 	m_sectors.push_back(values[i]);
    }
}

int Cake::get_type_at(int pos)
{
    return m_sectors[pos];
}

void Cake::print_sectors()
{
    cout << "Sectors in the cake:" << endl;
    for(int i = 0; i < N_SECTORS; i++)
    {
	cout << m_sectors[i] << " ";
    }
    cout << endl;
    cout << endl;
}
