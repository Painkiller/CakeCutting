#ifndef REFEREE_H
#define REFEREE_H
#include "entity.h"
#include "player.h"

class Referee : public Entity
{

public:
    Referee(Cake *cake);
    virtual ~Referee();
    void cut();
    void handle_halfpoints();
    void assign_player(Player *player);
    
private:
    vector<Player*> m_players_assigned;
    void assign_piece(string player_id, int sector_begin, float partial_begin,  int sector_end, float partial_end);
    Player* get_player_by_id(string m_id);
};

#endif // REFEREE_H
