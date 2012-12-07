#include "referee.h"

Referee::Referee(Cake* cake)
{
    m_cake = cake;
    m_entity_type = REFEREE;
    m_id = "REFEREE";
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
    
    Entity* old_player, *new_player;
    CakeCut *old_ck, *new_ck;
    bool skip = false;
    int old_sect, new_sect;
    float old_part, new_part;
    for (itr = tmp_list_ck.begin(); itr < tmp_list_ck.end(); itr++)
    {
	if(!skip)
	{
	  
	    old_ck = *itr;
	    skip = true;
	    old_player = old_ck->get_cutter();
	    old_sect = old_ck->get_cut_sector();
	    old_part = old_ck->get_cut_point();
	}
	else
	{
	    new_ck = *itr;
	    new_player = new_ck->get_cutter();
	    new_sect = new_ck->get_cut_sector();
	    new_part = new_ck->get_cut_point();
	    
	    if(new_sect == old_sect && new_part == old_part)
	    {	
		assignPiece(old_player, old_sect, old_part, m_cake->get_size() - 1, 1);
		assignPiece(new_player, 0, 0, new_sect, new_part);
		assignPiece(this, new_sect, new_part, old_sect, old_part);
	    }
	    else if(new_sect == old_sect)
	    {
		if(new_part < old_part)
		{
		    assignPiece(old_player, old_sect, old_part, m_cake->get_size() - 1, 1);
		    assignPiece(new_player, 0, 0, new_sect, new_part);
		    assignPiece(this, new_sect, new_part, old_sect, old_part);
		}
		else
		{
		    assignPiece(old_player, 0, 0, old_sect, old_part);
		    assignPiece(new_player, new_sect, new_part, m_cake->get_size() - 1, 1);
		    assignPiece(this, old_sect, old_part, new_sect, new_part);
		}
	    }
	    else if(new_sect < old_sect)
	    {
		assignPiece(old_player, old_sect, old_part, m_cake->get_size() - 1, 1);
		assignPiece(new_player, 0, 0, new_sect, new_part);
		assignPiece(this, new_sect, new_part, old_sect, old_part);

	    }
	    else
	    {
		assignPiece(old_player, 0, 0, old_sect, old_part);
		assignPiece(new_player, new_sect,  new_part, m_cake->get_size() - 1, 1);
		assignPiece(this, old_sect, old_part, new_sect, new_part);
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
	    
	    calculatePieceEvaluation(m_middle_piece->get_left_cut_sector(), eq_sector, m_middle_piece->get_left_cut_point(), eq_point, itr->second, middle_ev_first); 
	    calculatePieceEvaluation(eq_sector, m_middle_piece->get_right_cut_sector(), eq_point, m_middle_piece->get_right_cut_point(), itr->second, middle_other_ev_first);
	    if(isLogEnabled())
	    {
		cout << "Playerr " << player->get_id() << " receives suprlus piece from sector: "<< endl;
		cout << "Sector " << m_middle_piece->get_left_cut_sector() << " point " << m_middle_piece->get_left_cut_point() << " <-------> Sector " << eq_sector << " point " << eq_point <<endl;
		cout << "Player " << player->get_id() << " evaluation of his own surplus is " << middle_ev_first << endl;
		cout << "Player " << player->get_id() << " evaluation of the other surplus is " << middle_other_ev_first << endl;
	    }
	    m_pieces_assigned.erase(player);
	    assignPiece(player, 0, 0, eq_sector, eq_point);
	    if(isLogEnabled())
		cout << endl;
	    calculatePieceEvaluation(0, eq_sector, 0, eq_point, itr->second, res);
	    if(isLogEnabled())
	    {
		cout << "Player " << player->get_id() << " evalues his own piece as: "<< res <<endl;
		cout << endl;
	    }
	}
	else
	{
	    calculatePieceEvaluation(m_middle_piece->get_right_cut_sector(), m_cake->get_size() - 1, m_middle_piece->get_right_cut_point(), 1, itr->second, res);
	    
	    calculatePieceEvaluation(eq_sector, m_middle_piece->get_right_cut_sector(), eq_point, m_middle_piece->get_right_cut_point(), itr->second, middle_ev_second);
	    calculatePieceEvaluation(m_middle_piece->get_left_cut_sector(), eq_sector, m_middle_piece->get_left_cut_point(), eq_point, itr->second, middle_other_ev_second); 
	    if(isLogEnabled())
	    {
		cout << "Player " << player->get_id() << " receives suprlus piece from: " << endl;
		cout << "Sector "<<  eq_sector << ", point " << eq_point << " <-------> Sector "<< m_middle_piece->get_right_cut_sector() << ", point " <<  m_middle_piece->get_right_cut_point() <<endl;
		cout << "Player " << player->get_id() << " evaluation of his own surplus is " << middle_ev_second << endl;
		cout << "Player " << player->get_id() << " evaluation of the other surplus is " << middle_other_ev_second << endl;
	    }
	    m_pieces_assigned.erase(player);
	    assignPiece(player, eq_sector, eq_point, m_cake->get_size() -1, 1);
	    if(isLogEnabled())
		cout << endl;
	    calculatePieceEvaluation(eq_sector, m_cake->get_size() -1, eq_point, 1, itr->second, res);
	    if(isLogEnabled())
	    {
		cout << "Player " << player->get_id() << " evalues his own piece as: "<< res <<endl;
		cout << endl;
	    }
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

    m_max_res = 0;
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
    CakeCut *cut_left; /*= new CakeCut(owner, sector_begin, partial_begin);*/
    CakeCut *cut_right;/* = new CakeCut(owner, sector_end, partial_end);*/
    Piece *piece;/* = new Piece(owner, cut_left, cut_right);*/
    if(owner != this)
    {
	
	if(sector_begin == 0)
	{
	    piece = m_cake->get_piece(0, ANY);
	    m_pieces_assigned.insert(make_pair(owner, 0));
	}
	else if(sector_end == m_cake->get_size() - 1)
	{
	    piece = m_cake->get_piece(2, ANY);
	    m_pieces_assigned.insert(make_pair(owner, 2));
	}
	else
	{
	    piece = m_cake->get_piece(1, ANY);
	    m_pieces_assigned.insert(make_pair(owner, 1));
	}
    }
    else
    {
	piece = m_cake->get_piece(-1, ANY);
	m_middle_piece = piece;
    }
    cut_left = piece->get_ck_left();
    cut_right = piece->get_ck_right();
    cut_left->set_cakecut(owner, sector_begin, partial_begin);
    cut_right-> set_cakecut(owner, sector_end, partial_end);
//     	    	    cout << owner->get_id() << sector_begin <<" " <<  sector_end << endl;
// 		    cout << owner->get_id() << partial_begin <<" " <<  partial_end << endl;
    piece->set_piece(owner, cut_left, cut_right);
    owner->set_piece(piece);
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

    while((r - l) > MIN_ERR  )
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
    if(isLogEnabled())    
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
    
//     cout << "n " << n << " m " << m << " l " << l<< " r "<< r << endl;
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
		m --;
		break;
	      }
		
	      case 1:
	      {
		m ++;
		n --;
		break;
	      }
	      
	      case 2:
		n ++;
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
    	int iter = 0;
    while(!found)
    {
// 	    cout << fix_one << " ";
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
		    if( fabs(res_second - res_third) > MAX_ERR )
		    {
			if(res_first > res_second && res_second > res_third)
			{
			    sector_first = sector_first - 1;
			}
			else if(res_first < res_second && res_second < res_third)
			{
			    sector_second = sector_second + 1;
			}
			else if(res_first < res_second && res_second > res_third)
			{
			    if(res_first > res_third)
				sector_second = sector_second - 1;
			    else
				sector_first = sector_first + 1;
			}
		    }
		    else
		    {
			if(res_first > res_second)
			    sector_first = sector_first - 1;
			else
			    sector_first = sector_first + 1;
		    }
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
		    if(iter < 100)
		iter ++;
	    else
	      found = true;
    }
//     cout <<endl;
//     cout << endl;
    if(isLogEnabled())
    {
	cout << "******"<<  endl;
	cout << "f1 " << fix_one << " f2 " << fix_two << " res " << res_first << endl;
	cout << "******"<<  endl;
    }
    
    vector<Piece*> pieces_list;
    Piece *piece;
    CakeCut *cut_left, *cut_right;
    
    for (itr = m_players_assigned.begin(); itr != m_players_assigned.end(); itr++)
    {
	player = itr->first;
	piece = m_cake->get_piece(m_pieces_assigned.find(player)->second, ANY);
	cut_left = piece->get_ck_left();
	cut_right = piece->get_ck_right();
	
	switch(m_pieces_assigned.find(player)->second)
	{
	    case 0:
	      cut_left->set_cakecut(player, 0, 0);
	      cut_right->set_cakecut(player, sector_first, fix_one);

	    break;
	    case 1:
	      cut_left->set_cakecut(player, sector_first, fix_one);
	      cut_right->set_cakecut(player, sector_second, fix_two);

	    break;
	    case 2:
	      cut_left->set_cakecut(player, sector_second, fix_two);
	      cut_right->set_cakecut(player, m_cake->get_size() - 1, 1);
	    break;
	}
	piece->set_piece(player, cut_left, cut_right);
	pieces_list.push_back(piece);
    }
    if(res_first > m_max_res)
    {
	m_max_res = res_first;
	vector<Piece* >::iterator itr_p;
	Piece *tmp_piece, *best_piece;
	int k = 0;
	for(itr_p = pieces_list.begin(); itr_p != pieces_list.end(); itr_p++)
	{
	    tmp_piece = *itr_p;
	    best_piece = m_cake->get_piece(k, BEST);
	    cut_left = best_piece->get_ck_left();
	    cut_right = best_piece->get_ck_right();
	    cut_left->set_cakecut(tmp_piece->get_owner(), tmp_piece->get_ck_left()->get_cut_sector(), tmp_piece->get_ck_left()->get_cut_point());
	    cut_right->set_cakecut(tmp_piece->get_owner(), tmp_piece->get_ck_right()->get_cut_sector(), tmp_piece->get_ck_right()->get_cut_point());
	    best_piece->set_piece(tmp_piece->get_owner(), cut_left, cut_right);
	    k++;
	}
    }
//     m_result_map.insert(make_pair(pieces_list, res_first));
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
    
//     for (map_itr = m_result_map.begin(); map_itr != m_result_map.end(); map_itr++)
//     {
// 	float ckp1,ckp2;
// 	float cks1, cks2;
// 	tmp_res = map_itr->second;
// 	if(tmp_res > max_res)
// 	{
// 	    max_res = tmp_res;
// 	    best_common_cfg = map_itr->first;
// 	    ckp1 = map_itr->first[0]->get_ck_left()->get_cut_point();
// 	     ckp2 = map_itr->first[0]->get_ck_right()->get_cut_point();
// 	     cks1 = map_itr->first[0]->get_ck_left()->get_cut_sector();
// 	     cks1 = map_itr->first[0]->get_ck_right()->get_cut_sector();
// // 	    cout << max_res<< " "<<  cks1 << " " << ckp1 << " " << cks2 << " " << cks2<<endl;
// 	}
//     }
    
    Piece *piece;
    Entity *owner;
    int left_sector, right_sector;
    float left_point, right_point;
    
    clear_pieces();
//     for (list_itr = best_common_cfg.begin(); list_itr != best_common_cfg.end(); list_itr++)
    for(int k = 0; k < 3; k++)
    {
	piece = m_cake->get_piece(k, BEST);
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
	if(isLogEnabled())
	{
	    cout << "Player " << owner->get_id() << " receives piece: "<< endl;
	    cout << "Sector " << left_sector <<  ", Point " << left_point << " <------> ";
	    cout << "Sector " << right_sector <<  ", Point " << right_point <<endl;
	}
    }
    if(isLogEnabled())
    {
	cout << endl;
	cout << "Each player evalues his own piece as: "<< max_res <<endl;
    }
}