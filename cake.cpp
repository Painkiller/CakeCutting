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
    
    srand(time(0));
    
    for(int i = 0; i < N_SECTORS; i++)
    {
	value = rand() % N_SECTOR_TYPE;
	m_sectors.push_back(value);
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
