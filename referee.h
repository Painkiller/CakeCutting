#ifndef REFEREE_H
#define REFEREE_H
#include "entity.h"
#include "player.h"
#include "math.h"
#include "util.h"

using namespace std;
using namespace util;

class Referee : public Entity
{

public:
    Referee(Cake *cake);
    virtual ~Referee();
    void cut();
    void handleHalfpoints();
    void assignPlayer(Player *player);
    void calculateTotalEvaluation();
    void calculatePieceEvaluation(int sect_begin, int sect_end, float point_begin, float point_end, map<int, float> evaluation_map, float &result);
    void handleMiddle();
    void handleEquitability();
    
    inline void clear_pieces()
    {
	map<Player*, map<int, float> >::iterator itr;
	for (itr = m_players_assigned.begin(); itr != m_players_assigned.end(); itr++)
	{
	    itr->first->clear_piece();
	}
	m_pieces_assigned.clear();
    }
    void unassign_players(){m_players_assigned.clear();}
    void clear_result_map(){m_result_map.clear();}
    inline string& get_id(){return m_id;}
    
private:
    map<Player*, map< int, float> > m_players_assigned;
    map<Entity*, int> m_pieces_assigned;
    map<vector<Piece*>, float> m_result_map;
    Piece *m_middle_piece;
    float m_max_res;
    
    void assignPiece(Entity *owner, int sector_begin, float partial_begin,  int sector_end, float partial_end);
    bool isValidResult(float res_first, float res_second, float res_third);

    int findEqSector();
    void findEqSectorMulti(int& sector_first, int& sector_second);
    
    float findEqPoint(int arg1);
    void findEqPointMulti(int sector_first, int sector_second);
    
    void chooseBestCommonValue();
    
    Player* getPlayerById(string m_id);

};

#endif // REFEREE_H
