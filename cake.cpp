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

void Cake::buildCake(int problem)
{
    int value;
    int n_sectors;
    
    	switch(problem)
	{
	    case RANDOM:
	    {
		cout << "Insert number of sectors:";
		cin >> n_sectors;
		cout << endl;
		
		cout << "Insert number of sector types:";
		cin >> m_sector_types;
		cout << endl;
		
		
		for(int i = 0; i < n_sectors; i++)
		{
		    value = rand() % m_sector_types;
		    m_sectors.push_back(value);
		}
		break;
	    }
	    case BOSNIA:
	    {
		m_sector_types = N_BOSNIA_SECTORS;
		for(int i = 0; i < N_BOSNIA_SECTORS; i++)
		{
		    value = i;
		    m_sectors.push_back(value);
		}
		break;
	    }
	    
	}
}

void Cake::printSectors()
{
    cout << "Sectors in the cake:" << endl;
    for(int i = 0; i < m_sectors.size(); i++)
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
	
	if(isLogEnabled())
	{
	    if(cutter->isPlayer())
		cout << "Player " << cutter->get_id() << " cuts the cake at sector " << sector << " at point " << point << endl;
	    else
		cout << "Referee cuts the cake at sector " << sector << " at point " << point << endl;
	}
	m_cake_cut.push_back(ck);
    }
}

int Cake::countSectorOccurrences(int sec_type)
{
    int count = 0;
    
    for(int i = 0; i < m_sectors.size(); i++)
    {
	if(m_sectors[i] == sec_type)
	    count++;
    }
    return count;
}
