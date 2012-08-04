#ifndef REFEREE_H
#define REFEREE_H
#include "entity.h"
#include "player.h"
#include "math.h"

using namespace std;

class Referee : public Entity
{

public:
    Referee(Cake *cake);
    virtual ~Referee();
    void cut();
    void handle_halfpoints();
    void assign_player(Player *player);
    void calculate_total_evaluation();
    void calculate_piece_evaluation(int sect_begin, int sect_end, float part_begin, float part_end, map<int, float> evaluation_map, float &result);
    void handle_middle();
    
private:
    map<Player*, map< int, float> > m_players_assigned;
    map<Player*, int> m_pieces_assigned;
    Piece m_middle_piece;
    
    void assign_piece(string player_id, int sector_begin, float partial_begin,  int sector_end, float partial_end);
    Player* get_player_by_id(string m_id);
    int find_eq_sector();
    float find_eq_point(int arg1);
};

#endif // REFEREE_H
