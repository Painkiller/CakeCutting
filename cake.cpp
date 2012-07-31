#include "cake.h"
#include <stdlib.h>

Cake::Cake()
{
    m_sectors = new int[N_SECTORS];
}

Cake::~Cake()
{

}

void Cake::build_cake()
{
    srand(time(0));
    for(int i = 0; i < N_SECTORS; i++)
    {
	m_sectors[i] = rand() % N_SECTOR_TYPE;
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
