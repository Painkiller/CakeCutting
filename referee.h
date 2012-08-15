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
    
    inline void clear_pieces(){m_pieces_assigned.clear();}
    
private:
    map<Player*, map< int, float> > m_players_assigned;
    map<Player*, int> m_pieces_assigned;
    Piece *m_middle_piece;
    
    void assignPiece(Entity *owner, int sector_begin, float partial_begin,  int sector_end, float partial_end);
    bool isValidResult(float res_first, float res_second, float res_third);

    int findEqSector();
    void findEqSectorMulti(int& sector_first, int& sector_second);
    
    float findEqPoint(int arg1);
    float findEqPointMulti(int sector_first, int sector_second, float& eq_first_point, float& eq_second_point);
    
    Player* getPlayerById(string m_id);
};

#endif // REFEREE_H
