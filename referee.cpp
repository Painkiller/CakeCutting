#include "referee.h"

Referee::Referee(Cake* cake)
{
    m_cake = cake;
}

Referee::~Referee()
{

}

void Referee::cut()
{

}

void Referee::assign_player(Player* player)
{
    m_players_assigned.push_back(player);
}

void Referee::handle_halfpoints()
{
    
    vector<CakeCut> tmp_list_ck = m_cake->get_cake_cut_list();
    vector<CakeCut>::iterator itr;
    
    CakeCut old_ck, new_ck;
    bool skip = false;
    
    for (itr = tmp_list_ck.begin(); itr < tmp_list_ck.end(); itr++)
    {
	if(!skip)
	{
	    old_ck = *itr;
	    skip = true;
	}
	else
	{
	    new_ck = *itr;
	    if(new_ck.m_sector == old_ck.m_sector && new_ck.m_part == old_ck.m_part)
	    {	

		assign_piece(new_ck.m_id, 0, 1, new_ck.m_sector, new_ck.m_part);
		assign_piece(old_ck.m_id, old_ck.m_sector, (1 - old_ck.m_part), m_cake->get_size(), 1);
	    }
	    else if(new_ck.m_sector == old_ck.m_sector)
	    {
		if(new_ck.m_part < old_ck.m_part)
		{
		    assign_piece(new_ck.m_id, 0, 1, new_ck.m_sector, new_ck.m_part);
		    assign_piece(old_ck.m_id, old_ck.m_sector, (1 - old_ck.m_part), m_cake->get_size(), 1);
		}
		else
		{
		    assign_piece(old_ck.m_id, 0, 1, old_ck.m_sector, old_ck.m_part);
		    assign_piece(new_ck.m_id, new_ck.m_sector, (1 - new_ck.m_part), m_cake->get_size(), 1);
		}
	    }
	    else if(new_ck.m_sector < old_ck.m_sector)
	    {
		assign_piece(new_ck.m_id, 0, 1, new_ck.m_sector, new_ck.m_part);
		assign_piece(old_ck.m_id, old_ck.m_sector, (1 - old_ck.m_part), m_cake->get_size(), 1);
	    }
	    else
	    {
		assign_piece(old_ck.m_id, 0, 1, old_ck.m_sector, old_ck.m_part);
		assign_piece(new_ck.m_id, new_ck.m_sector, (1 - new_ck.m_part), m_cake->get_size(), 1);
	    }
	}
    }
}

void Referee::assign_piece(string player_id, int sector_begin, float partial_begin,  int sector_end, float partial_end)
{
    Piece piece;
    
    CakeCut p_begin = {player_id, sector_begin, partial_begin};
    CakeCut p_end = {player_id, sector_end, partial_end};
    
    piece.m_begin = p_begin;
    piece.m_end = p_end;
    
    Player *player = get_player_by_id(player_id);
    
    player->set_piece(piece);
}

Player* Referee::get_player_by_id(string m_id)
{
    vector<Player*>::iterator itr;
    
    for (itr = m_players_assigned.begin(); itr < m_players_assigned.end(); itr++)
    {
	Player *player = *itr;
	if(player->get_id() == m_id)
	    return player;
    }
    return NULL;
}
