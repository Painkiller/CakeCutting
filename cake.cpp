#include "cake.h"
#include <stdlib.h>
#include "player.h"
#include "referee.h"

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
	m_sectors.push_back(value);
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

void Cake::setCakeCut(Entity* cutter, int sector, float point)
{
    {
	CakeCut *ck = new CakeCut(cutter, sector, point);
	
	if(cutter->isPlayer())
	    cout << "Player " << cutter->get_id() << " cuts the cake at sector " << sector << " at point " << point << endl;
	else
	    cout << "Referee cuts the cake at sector " << sector << " at point " << point << endl;
	m_cake_cut.push_back(ck);
    }
}

int Cake::countSectorOccurrences(int sec_type)
{
    int count = 0;
    
    for(int i = 0; i < N_SECTORS; i++)
    {
	if(m_sectors[i] == sec_type)
	    count++;
    }
    return count;
}
