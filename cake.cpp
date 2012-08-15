#include "cake.h"
#include <stdlib.h>

Cake::Cake()
{
  
}

Cake::~Cake()
{

}

void Cake::buildCake()
{
    int value;
    int values[8] = {3,4,4,1,4,4,3,2}; 
    
    for(int i = 0; i < N_SECTORS; i++)
    {
	value = rand() % N_SECTOR_TYPE;
	m_sectors.push_back(values[i]);
// 	m_sectors.push_back(values[i]);
    }
}

void Cake::printSectors()
{
    cout << "Sectors in the cake:" << endl;
    for(int i = 0; i < N_SECTORS; i++)
    {
	cout << m_sectors[i] << " ";
    }
    cout << endl;
    cout << endl;
}
