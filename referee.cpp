#include "referee.h"

Referee::Referee(Cake* cake)
{
    m_cake = cake;
    m_entity_type = REFEREE;
}

Referee::~Referee()
{

}

void Referee::cut()
{

}

void Referee::assignPlayer(Player* player)
{
    m_players_assigned.insert(make_pair(player, player->get_evaluation_map()));
}

void Referee::handleHalfpoints()
{
    
    vector<CakeCut*> tmp_list_ck = m_cake->get_cake_cut_list();
    vector<CakeCut*>::iterator itr;
    
    CakeCut *old_ck, *new_ck;
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
	    if(new_ck->get_cut_sector() == old_ck->get_cut_sector() && new_ck->get_cut_point() == old_ck->get_cut_point())
	    {	

		assignPiece(new_ck->get_cutter(), 0, 0, new_ck->get_cut_sector(), new_ck->get_cut_point());
		assignPiece(old_ck->get_cutter(), old_ck->get_cut_sector(), old_ck->get_cut_point(), m_cake->get_size() - 1, 1);
	    }
	    else if(new_ck->get_cut_sector() == old_ck->get_cut_sector())
	    {
		if(new_ck->get_cut_point() < old_ck->get_cut_point())
		{
		    assignPiece(new_ck->get_cutter(), 0, 0, new_ck->get_cut_sector(), new_ck->get_cut_point());
		    assignPiece(old_ck->get_cutter(), old_ck->get_cut_sector(), old_ck->get_cut_point(), m_cake->get_size() - 1, 1);
		    assignPiece(this, new_ck->get_cut_sector(), new_ck->get_cut_point(), old_ck->get_cut_sector(), old_ck->get_cut_point());
		}
		else
		{
		    assignPiece(old_ck->get_cutter(), 0, 0, old_ck->get_cut_sector(), old_ck->get_cut_point());
		    assignPiece(new_ck->get_cutter(), new_ck->get_cut_sector(), new_ck->get_cut_point(), m_cake->get_size() - 1, 1);
		    assignPiece(this, old_ck->get_cut_sector(), old_ck->get_cut_point(), new_ck->get_cut_sector(), new_ck->get_cut_point());
		}
	    }
	    else if(new_ck->get_cut_sector() < old_ck->get_cut_sector())
	    {
		assignPiece(new_ck->get_cutter(), 0, 0, new_ck->get_cut_sector(), new_ck->get_cut_point());
		assignPiece(old_ck->get_cutter(), old_ck->get_cut_sector(), old_ck->get_cut_point(), m_cake->get_size() - 1, 1);
		assignPiece(this, new_ck->get_cut_sector(), new_ck->get_cut_point(), old_ck->get_cut_sector(), old_ck->get_cut_point());

	    }
	    else
	    {
		assignPiece(old_ck->get_cutter(), 0, 0, old_ck->get_cut_sector(), old_ck->get_cut_point());
		assignPiece(new_ck->get_cutter(), new_ck->get_cut_sector(),  new_ck->get_cut_point(), m_cake->get_size() - 1, 1);
		assignPiece(this, old_ck->get_cut_sector(), new_ck->get_cut_point(), new_ck->get_cut_sector(), old_ck->get_cut_point());
	    }
	}
    }
}

void Referee::handleMiddle()
{
    
    int eq_sector; 
    float eq_point, middle_ev_first, middle_ev_second;
    float middle_other_ev_first, middle_other_ev_second;
    float res;
    map<Player*, map<int, float> >::iterator itr;
    Player *player;
    
    
    eq_sector = findEqSector();
    eq_point = findEqPoint(eq_sector);
    
    for (itr = m_players_assigned.begin(); itr != m_players_assigned.end(); itr++)
    {
	player = itr->first;
	
	if(m_pieces_assigned.find(player)->second == 0)
	{
	    calculatePieceEvaluation(0, m_middle_piece->get_left_cut_sector(), 0, m_middle_piece->get_left_cut_point(), itr->second, res);
	    cout << "Player " << player->get_id() << " evalues his own piece as: "<< res <<endl;
	    cout<<endl;
	    
	    calculatePieceEvaluation(m_middle_piece->get_left_cut_sector(), eq_sector, m_middle_piece->get_left_cut_point(), eq_point, itr->second, middle_ev_first); 
	    calculatePieceEvaluation(eq_sector, m_middle_piece->get_right_cut_sector(), eq_point, m_middle_piece->get_right_cut_point(), itr->second, middle_other_ev_first);
	    
	    cout << "Player " << player->get_id() << " receives suprlus piece from sector " << m_middle_piece->get_left_cut_sector() << " to " << eq_sector << " at point " << eq_point <<endl;
	    cout << "Player " << player->get_id() << " evaluation of his own surplus is " << middle_ev_first << endl;
	    cout << "Player " << player->get_id() << " evaluation of the other surplus is " << middle_other_ev_first << endl;
	    
	    clear_pieces();
	    assignPiece(player, 0, 0, eq_sector, eq_point);
	    
	    cout<<endl;
	    calculatePieceEvaluation(0, eq_sector, 0, eq_point, itr->second, res);
	    cout << "Player " << player->get_id() << " evalues his own piece as: "<< res <<endl;
	    cout<<endl;

	}
	else
	{
	    calculatePieceEvaluation(m_middle_piece->get_right_cut_sector(), m_cake->get_size() -1, m_middle_piece->get_right_cut_point(), 1, itr->second, res);
	    cout << "Player " << player->get_id() << " evalues his own piece as: "<< res <<endl;
	    cout<<endl;
	    
	    calculatePieceEvaluation(eq_sector, m_middle_piece->get_right_cut_sector(), eq_point, m_middle_piece->get_right_cut_point(), itr->second, middle_ev_second);
	    calculatePieceEvaluation(m_middle_piece->get_left_cut_sector(), eq_sector, m_middle_piece->get_left_cut_point(), eq_point, itr->second, middle_other_ev_second); 

	    cout << "Player " << player->get_id() << " receives suprlus piece from sector " << eq_sector << " to " << m_middle_piece->get_right_cut_sector() << " at point " << eq_point <<endl;
	    cout << "Player " << player->get_id() << " evaluation of his own surplus is " << middle_ev_second << endl;
	    cout << "Player " << player->get_id() << " evaluation of the other surplus is " << middle_other_ev_second << endl;
	    
	    clear_pieces();
	    assignPiece(player, eq_sector, eq_point, m_cake->get_size() -1, 1);
	    
	    cout<<endl;
	    calculatePieceEvaluation(eq_sector, m_cake->get_size() -1, eq_point, 1, itr->second, res);
	    cout << "Player " << player->get_id() << " evalues his own piece as: "<< res <<endl;
	    cout<<endl;

	}
    }
}

void Referee::handleEquitability()
{
    int eq_first_sector, eq_second_sector; 
    int size = m_players_assigned.size();
    int i, j, k; 
    float eq_first_point, eq_second_point, middle_ev_first, middle_ev_second;
    float middle_other_ev_first, middle_other_ev_second;
    float common_value;
	
    map<Player*, map<int, float> >::iterator itr;
    Player *player;

    
    for(j = 0; j < fact(size); j++)
    {
	i = 0;
	for (itr = m_players_assigned.begin(); itr != m_players_assigned.end(); itr++)
	{
	    if(j < size)
		k = (j + i) % size;
	    else
		k = (j - i) % size;
	    player = itr->first;
	    m_pieces_assigned.insert(make_pair(player, k));
	    i++;
	}
	
	findEqSectorMulti(eq_first_sector, eq_second_sector);
	findEqPointMulti(eq_first_sector, eq_second_sector);
		
	clear_pieces();
    }
    chooseBestCommonValue();
}

void Referee::assignPiece(Entity *owner, int sector_begin, float partial_begin,  int sector_end, float partial_end)
{
    CakeCut *cut_left = new CakeCut(owner, sector_begin, partial_begin);
    CakeCut *cut_right = new CakeCut(owner, sector_end, partial_end);
    
    Piece *piece = new Piece(owner, cut_left, cut_right);
    
    if(owner != this)
    {
	owner->set_piece(piece);
	if(sector_begin == 0)
	    m_pieces_assigned.insert(make_pair(owner, 0));
	else
	    m_pieces_assigned.insert(make_pair(owner, 1));
    }
    else
	m_middle_piece = piece;
}

Player* Referee::getPlayerById(string m_cutter_id)
{
    map<Player*, map<int, float> >::iterator itr;
    
    for (itr = m_players_assigned.begin(); itr != m_players_assigned.end(); itr++)
    {
	Player *player = itr->first;
	if(player->get_id() == m_cutter_id)
	    return player;
    }
    return NULL;
}

void Referee::calculateTotalEvaluation()
{

}

void Referee::calculatePieceEvaluation(int sect_begin, int sect_end, float point_begin, float point_end, map<int, float> evaluation_map, float& result)
{
    int type;
    
    type = m_cake->get_type_at(sect_begin);
    
    if(sect_end < sect_begin)
    {
	result = 0;
	return;
    }
    if(sect_begin == sect_end)
      result = evaluation_map.find(type)->second * (point_end - point_begin);
    else
      result = evaluation_map.find(type)->second * (1 - point_begin);

    if(sect_begin != sect_end)
    {
	if( (sect_begin + 1) != sect_end)
	{
	    for(int i = (sect_begin + 1); i < sect_end; i++)
	    {
		type = m_cake->get_type_at(i);
		result += evaluation_map.find(type)->second;
	    }
	}
	type = m_cake->get_type_at(sect_end);
	result += evaluation_map.find(type)->second * point_end;
    }
}
int Referee::findEqSector()
{
    int l, r, mid;
    float res_first, res_second;
    
    map<Player*, map<int, float> >::iterator itr;
    Player *player;
    
    l = m_middle_piece->get_left_cut_sector();
    r = m_middle_piece->get_right_cut_sector();
    
    while( !(l == r) /*&& !((r - 1) == l)*/)
    {
	mid = ((l + r + 1) / 2);
	for (itr = m_players_assigned.begin(); itr != m_players_assigned.end(); itr++)
	{
	    player = itr->first;
	    if(m_pieces_assigned.find(player)->second == 0)
		calculatePieceEvaluation(m_middle_piece->get_left_cut_sector(), mid - 1, m_middle_piece->get_left_cut_point(), 1, itr->second, res_first); 
	    else
		calculatePieceEvaluation(mid, m_middle_piece->get_right_cut_sector(), 0, m_middle_piece->get_right_cut_point(), itr->second, res_second); 
	}
	if(res_first < res_second)
	{
	    if( (r - 1) == l)
		l = r;
	    else
		l = mid;
	}
	else
	{
	  if( (r - 1) == l)
	      r = l;
	  else
	      r = mid;
	}
    }    
    return l;
}

float Referee::findEqPoint(int sector)
{
    float l, r, mid;
    float res_first, res_second;
    
    map<Player*, map<int, float> >::iterator itr;
    Player *player;
    
    if(m_middle_piece->get_left_cut_sector() != m_middle_piece->get_right_cut_sector())
    {
	l = 0;
	r = 1;
    }
    else if (m_middle_piece->get_left_cut_sector() == m_middle_piece->get_right_cut_sector())
    {
	l = m_middle_piece->get_left_cut_point();
	r = m_middle_piece->get_right_cut_point();
    }
    else if(m_middle_piece->get_left_cut_sector() == sector)
    {
	l =  m_middle_piece->get_left_cut_point();
	r = 1;
    }
    else if(m_middle_piece->get_right_cut_sector() == sector)
    {
      	l = 0;
	r = m_middle_piece->get_right_cut_point();
    }

    while((r - l) > 0.0000001  )
    {
	mid = (l + r) / 2;
	for (itr = m_players_assigned.begin(); itr != m_players_assigned.end(); itr++)
	{
	    player = itr->first;
	    if(m_pieces_assigned.find(player)->second == 0)
		calculatePieceEvaluation(m_middle_piece->get_left_cut_sector(), sector, m_middle_piece->get_left_cut_point(), mid, itr->second, res_first); 
	    else
		calculatePieceEvaluation(sector, m_middle_piece->get_right_cut_sector(), mid,  m_middle_piece->get_right_cut_point(), itr->second, res_second); 
	}
	if(res_first < res_second)
	  l = mid;
	else
	  r = mid;
    }
    cout << "Sector: " << sector << "; Point " << l << endl;
    return l;
}

void Referee::findEqSectorMulti(int& sector_first, int& sector_second)
{
    int r, m, n, l;
    int fix_one, fix_two, fix_three, fix_four;
    int plr;
    float res_first[2], res_second[2], res_third[2];
    float tmp_res_one, tmp_res_two;
    float max_min, min_max;
    bool found = false;
    
    map<Player*, map<int, float> >::iterator itr;
    Player *player;
    
    l = 0;
    r = m_cake->get_size() - 1;
    
    n = floor((l + r + 1) * 2 / 3);
    m = floor((l + r + 1) / 3);
    
    fix_one = m;
    fix_two = n;
    
    cout << "n " << n << " m " << m << " l " << l<< " r "<< r << endl;
    while(!found)
    {
	for (itr = m_players_assigned.begin(); itr != m_players_assigned.end(); itr++)
	{
	    player = itr->first;
	    switch(m_pieces_assigned.find(player)->second)
	    {
		case 0:
		    calculatePieceEvaluation(0, m - 1, 0, 1, itr->second, res_first[0]); 
		    calculatePieceEvaluation(m, m, 0, 1, itr->second, tmp_res_one); 
		    res_first[1] = res_first[0] + tmp_res_one;
		break;
		case 1:
		    calculatePieceEvaluation(m + 1, n - 1 , 0, 1, itr->second, res_second[0]); 
		    calculatePieceEvaluation(m, m, 0, 1, itr->second, tmp_res_one); 
		    calculatePieceEvaluation(n, n, 0, 1, itr->second, tmp_res_two); 
		    res_second[1] = res_second[0] + tmp_res_one + tmp_res_two;
		break;
		case 2:
		    calculatePieceEvaluation(n + 1, m_cake->get_size() - 1 , 0, 1, itr->second, res_third[0]); 
		    calculatePieceEvaluation(n, n, 0, 1, itr->second, tmp_res_one); 
		    res_third[1] = res_third[0] + tmp_res_one;
		break;
	    }
	}
	
	max_min = res_first[0];
	plr = 0;
	if(res_second[0] > max_min)
	{
	    max_min = res_second[0];
	    plr = 1;
	}
	if(res_third[0] > max_min)
	{
	    max_min = res_third[0];
	    plr = 2;
	}
	
	min_max = res_first[1];
	if(res_second[1] < min_max)
	    min_max = res_second[1];
	if(res_third[1] < min_max)
	    min_max = res_third[1];
			
	if(min_max >= max_min)
	  found = true;
	else
	{
	    switch(plr)
	    {
	      case 0:
	      {
		m -= 1;
		break;
	      }
		
	      case 1:
	      {
		m += 1;
		n -= 1;
		break;
	      }
	      
	      case 2:
		n += 1;
		break;
	    }
	}


    }

    sector_first = m;
    sector_second = n;
}

void Referee::findEqPointMulti(int first, int second)
{
    float l, r, m, n;
    float fix_one, fix_two, fix_three;
    float res_first, res_second, res_third;
    bool found = false;
    int sector_first = first;
    int sector_second = second;
    map<Player*, map<int, float> >::iterator itr;
    Player *player;
    
    l = 0;
    r = 1;
    m = (l + r) / 2;
    n = m;
    
    fix_one = m;
    fix_two = n;
    
    while(!found)
    {
	while( (r - l) > MIN_ERR)
	{
	    for (itr = m_players_assigned.begin(); itr != m_players_assigned.end(); itr++)
	    {
		player = itr->first;
		switch(m_pieces_assigned.find(player)->second)
		{
		    case 0:
			calculatePieceEvaluation(0, sector_first, 0, m, itr->second, res_first); 
		    break;
		    case 1:
			calculatePieceEvaluation(sector_first, sector_second , m, fix_two, itr->second, res_second); 
		    break;
		    default:
		    break;
		}
	    }
	    
	    if(res_first < res_second)
		l = m;
	    else
		r = m;
	    
	    fix_one = m;
	    m = (l + r) / 2;
	}
	
	fix_three = fix_two;
	l = 0;
	r = 1;
	
	while( (r - l) > MIN_ERR)
	{
	    for (itr = m_players_assigned.begin(); itr != m_players_assigned.end(); itr++)
	    {
		player = itr->first;
		switch(m_pieces_assigned.find(player)->second)
		{
		    case 1:
			calculatePieceEvaluation(sector_first, sector_second , fix_one,  n, itr->second, res_second); 
		    break;
		    case 2:
			calculatePieceEvaluation(sector_second, m_cake->get_size() - 1 , n, 1, itr->second, res_third); 
		    break;
		    default:
		    break;
		}
	    }
	    if(res_second < res_third)
		l = n;
	    else
		r = n;
	    
	    fix_two = n;
	    n = (l + r) / 2;
	}
	if( fabs(fix_three - fix_two) <= MIN_ERR )
	{
	    if(isValidResult(res_first, res_second, res_third))
		found = true;	
	    else
	    {
		if( fabs(res_first - res_second) > MAX_ERR )
		{
		  if(res_first > res_second)
		      sector_first = sector_first - 1;
		  else if(res_first < res_second)
		    sector_first = sector_first + 1;
		}
		else if( fabs(res_second - res_third) > MAX_ERR )
		{
		  if(res_third < res_second)
		      sector_second = sector_second - 1;
		  else if(res_third > res_second)
		      sector_second = sector_second + 1;
		}
	    }
	}
	else
	{
	    l = 0;
	    r = 1;
	    n = (fix_three + fix_two) / 2;
	    m = (l + r) / 2;
	    fix_one = m;
	    fix_two = n;
	}
    }
    cout << "******"<<  endl;
    cout << "f1 " << fix_one << " f2 " << fix_two << " res " << res_first << endl;
    cout << "******"<<  endl;
    vector<Piece*> pieces_list;
    Piece *piece;
    for (itr = m_players_assigned.begin(); itr != m_players_assigned.end(); itr++)
    {
	player = itr->first;
	switch(m_pieces_assigned.find(player)->second)
	{
	    case 0:
	      piece = new Piece(player, new CakeCut(player, 0, 0), new CakeCut(player, sector_first, fix_one));
	      pieces_list.push_back(piece);
	    break;
	    case 1:
	      piece = new Piece(player, new CakeCut(player, sector_first, fix_one), new CakeCut(player, sector_second, fix_two));
	      pieces_list.push_back(piece);
	    break;
	    case 2:
	      piece = new Piece(player, new CakeCut(player, sector_second, fix_two), new CakeCut(player, m_cake->get_size() - 1, 1));
	      pieces_list.push_back(piece);
	    break;
	}
    }
    m_result_map.insert(make_pair(pieces_list, res_first));
}

bool Referee::isValidResult(float res_first, float res_second, float res_third)
{
    if(fabs(res_first - res_second) > MAX_ERR || fabs(res_first - res_third) > MAX_ERR || fabs(res_second - res_third) > MAX_ERR)
      return false;
    
    return true;
}

void Referee::chooseBestCommonValue()
{
    map<vector<Piece*>, float >::iterator map_itr;   
    vector<Piece*>::iterator list_itr;   

    float max_res = 0, tmp_res;

    vector<Piece*> best_common_cfg;
    
    for (map_itr = m_result_map.begin(); map_itr != m_result_map.end(); map_itr++)
    {
	tmp_res = map_itr->second;
	if(tmp_res > max_res)
	{
	    max_res = tmp_res;
	    best_common_cfg = map_itr->first;
	}
    }
    
    Piece *piece;
    Entity *owner;
    int left_sector, right_sector;
    float left_point, right_point;
    
    for (list_itr = best_common_cfg.begin(); list_itr != best_common_cfg.end(); list_itr++)
    {
	piece = *list_itr;
	owner = piece->get_owner();
	left_sector = piece->get_left_cut_sector();
	right_sector = piece->get_right_cut_sector();
	left_point =  piece->get_left_cut_point();
	right_point = piece->get_right_cut_point();
	
	if(left_point < MIN_ERR)
	    left_point = 0;
	if(right_point < MIN_ERR)
	    right_point = 0;
	
	assignPiece(owner, left_sector, left_point, right_sector, right_point);
	cout << "Player " << owner->get_id() << " receives piece: "<< endl;
	cout << "Sector " << left_sector <<  ", Point " << left_point << " <------> ";
	cout << "Sector " << right_sector <<  ", Point " << right_point <<endl;
    }
    
    cout << endl;
    cout << "Each player evalues his own piece as: "<< max_res <<endl;
}