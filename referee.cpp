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
    m_players_assigned.insert(make_pair(player, player->get_evaluation_map()));
}

void Referee::handle_halfpoints()
{
    
    vector<CakeCut> tmp_list_ck = m_cake->get_cake_cut_list();
    vector<CakeCut>::iterator itr;
    
    CakeCut old_ck, new_ck;
    bool skip = false;
    
    for (itr = tmp_list_ck.begin(); itr < tmp_list_ck.end(); itr++)
    {
// 	cout << "OOOOOOOOOOO "<< itr->m_id << " OOOOOOOOOO" << itr->m_sector << endl;
	if(!skip)
	{
	    old_ck = *itr;
	    skip = true;
	}
	else
	{
	    new_ck = *itr;
	    if(new_ck.m_sector == old_ck.m_sector && new_ck.m_point == old_ck.m_point)
	    {	

		assign_piece(new_ck.m_id, 0, 1, new_ck.m_sector, new_ck.m_point);
		assign_piece(old_ck.m_id, old_ck.m_sector, old_ck.m_point, m_cake->get_size() - 1, 1);
	    }
	    else if(new_ck.m_sector == old_ck.m_sector)
	    {
		if(new_ck.m_point < old_ck.m_point)
		{
		    assign_piece(new_ck.m_id, 0, 1, new_ck.m_sector, new_ck.m_point);
		    assign_piece(old_ck.m_id, old_ck.m_sector, old_ck.m_point, m_cake->get_size() - 1, 1);
		    assign_piece("R", new_ck.m_sector, new_ck.m_point, old_ck.m_sector, old_ck.m_point);
		}
		else
		{
		    assign_piece(old_ck.m_id, 0, 1, old_ck.m_sector, old_ck.m_point);
		    assign_piece(new_ck.m_id, new_ck.m_sector, new_ck.m_point, m_cake->get_size() - 1, 1);
		    assign_piece("R", old_ck.m_sector, old_ck.m_point, new_ck.m_sector, new_ck.m_point);
		}
	    }
	    else if(new_ck.m_sector < old_ck.m_sector)
	    {
		assign_piece(new_ck.m_id, 0, 1, new_ck.m_sector, new_ck.m_point);
		assign_piece(old_ck.m_id, old_ck.m_sector, old_ck.m_point, m_cake->get_size() - 1, 1);
		assign_piece("R", new_ck.m_sector, new_ck.m_point, old_ck.m_sector, old_ck.m_point);

	    }
	    else
	    {
		assign_piece(old_ck.m_id, 0, 1, old_ck.m_sector, old_ck.m_point);
		assign_piece(new_ck.m_id, new_ck.m_sector,  new_ck.m_point, m_cake->get_size() - 1, 1);
		assign_piece("R", old_ck.m_sector, new_ck.m_point, new_ck.m_sector, old_ck.m_point);
	    }
	}
    }
}

void Referee::handle_middle()
{
    
    int eq_sector; 
    float eq_point, middle_ev_first, middle_ev_second;
    float middle_other_ev_first, middle_other_ev_second;
    map<Player*, map<int, float> >::iterator itr;
    Player *player;
    
    
    eq_sector = find_eq_sector();
    eq_point = find_eq_point(eq_sector);
    
    for (itr = m_players_assigned.begin(); itr != m_players_assigned.end(); itr++)
    {
	player = itr->first;
	
	if(m_pieces_assigned.find(player)->second == 0)
	{
	    calculate_piece_evaluation(m_middle_piece.m_begin.m_sector, eq_sector, m_middle_piece.m_begin.m_point, eq_point, itr->second, middle_ev_first); 
	    calculate_piece_evaluation(eq_sector, m_middle_piece.m_end.m_sector, eq_point, m_middle_piece.m_end.m_point, itr->second, middle_other_ev_first);
// 	    cout<<"QQQQQQQQQQQQQQQQ"<<endl;
// 	    calculate_piece_evaluation(0, m_cake->get_size(), 1, 1, itr->second, middle_other_ev_first); 
// 	    cout<<"QQQQQQQQQQQQQQQQ"<<endl;
	    cout << "Player " << player->get_id() << " receives suprlus piece from sector " << m_middle_piece.m_begin.m_sector << " to " << eq_sector << " at point " << eq_point <<endl;
	    cout << "Player " << player->get_id() << " evalution of his own surplus is " << middle_ev_first << endl;
	    cout << "Player " << player->get_id() << " evalution of the other surplus is " << middle_other_ev_first << endl;
	}
	else
	{
	    calculate_piece_evaluation(eq_sector, m_middle_piece.m_end.m_sector, eq_point, m_middle_piece.m_end.m_point, itr->second, middle_ev_second);
	    calculate_piece_evaluation(m_middle_piece.m_begin.m_sector, eq_sector, m_middle_piece.m_begin.m_point, eq_point, itr->second, middle_other_ev_second); 

	    cout << "Player " << player->get_id() << " receives suprlus piece from sector " << eq_sector << " to " << m_middle_piece.m_end.m_sector << " at point " << eq_point <<endl;
	    cout << "Player " << player->get_id() << " evalution of his own surplus is " << middle_ev_second << endl;
	    cout << "Player " << player->get_id() << " evalution of the other surplus is " << middle_other_ev_second << endl;
	}
    }
}

void Referee::handle_equitability()
{
    int eq_first_sector, eq_second_sector; 
    
    float eq_first_point, eq_second_point, middle_ev_first, middle_ev_second;
    float middle_other_ev_first, middle_other_ev_second;
    
    map<Player*, map<int, float> >::iterator itr;
    Player *player;
    int i = 0;
    
    for (itr = m_players_assigned.begin(); itr != m_players_assigned.end(); itr++)
    {
	player = itr->first;
	m_pieces_assigned.insert(make_pair(player, i));
	i++;
    }
    
    find_eq_sector_multi(eq_first_sector, eq_second_sector);
    find_eq_point_multi(eq_first_sector, eq_second_sector, eq_first_point, eq_second_point);
}

void Referee::assign_piece(string player_id, int sector_begin, float partial_begin,  int sector_end, float partial_end)
{
    Piece piece;
    
    CakeCut p_begin = {player_id, sector_begin, partial_begin};
    CakeCut p_end = {player_id, sector_end, partial_end};
    
    piece.m_begin = p_begin;
    piece.m_end = p_end;
    
    Player *player = get_player_by_id(player_id);
    
    if(player_id != "R")
    {
	player->set_piece(piece);
	if(sector_begin == 0)
	{
	  cout << "play0 " << player_id << " sect_begin " << sector_begin << endl;
	  m_pieces_assigned.insert(make_pair(player, 0));
	}
	else
	{
	  m_pieces_assigned.insert(make_pair(player, 1));
	  cout << "play1 " << player_id << " sect_begin " << sector_begin << endl;
	}
    }
    else
	m_middle_piece = piece;
}

Player* Referee::get_player_by_id(string m_id)
{
    map<Player*, map<int, float> >::iterator itr;
    
    for (itr = m_players_assigned.begin(); itr != m_players_assigned.end(); itr++)
    {
	Player *player = itr->first;
	if(player->get_id() == m_id)
	    return player;
    }
    return NULL;
}

void Referee::calculate_total_evaluation()
{

}

void Referee::calculate_piece_evaluation(int sect_begin, int sect_end, float point_begin, float point_end, map<int, float> evaluation_map, float& result)
{
    int type;
    
    type = m_cake->get_type_at(sect_begin);
    if(sect_begin == sect_end)
      result = evaluation_map.find(type)->second * (point_end - point_begin);
    else
      result = evaluation_map.find(type)->second * (1 - point_begin);
//     cout<<result<<endl;
      
//      cout << "sect_begin: " << sect_begin << " part_begin: " << point_begin << " sect_end: " << sect_end << " res: " << result << endl;
    if(sect_begin != sect_end)
    {
	if( (sect_begin + 1) != sect_end)
	{
	    for(int i = (sect_begin + 1); i < sect_end; i++)
	    {
		type = m_cake->get_type_at(i);
		result += evaluation_map.find(type)->second;
// 		cout<<evaluation_map.find(type)->second<<endl;
// 		cout << " res: " << result << endl;
	    }
	}
	type = m_cake->get_type_at(sect_end);
	result += evaluation_map.find(type)->second * point_end;
// 			cout<<evaluation_map.find(type)->second<<" sec end " << sect_end << " type " << type << endl;

    }
}
int Referee::find_eq_sector()
{
    int l, r, mid;
    float res_first, res_second;
    
    map<Player*, map<int, float> >::iterator itr;
    Player *player;
    
    l = m_middle_piece.m_begin.m_sector;
    r = m_middle_piece.m_end.m_sector;
    
    while( !(l == r) /*&& !((r - 1) == l)*/)
    {
	mid = ((l + r + 1) / 2);
	for (itr = m_players_assigned.begin(); itr != m_players_assigned.end(); itr++)
	{
	    player = itr->first;
	    cout << "mid " << mid<< " l " << l<< " r "<< r << endl;
	    if(m_pieces_assigned.find(player)->second == 0)
	    {
		calculate_piece_evaluation(m_middle_piece.m_begin.m_sector, mid - 1, m_middle_piece.m_begin.m_point, 1, itr->second, res_first); 
				cout << "res first " << res_first<< endl;

	    }
	    else
	    {
		calculate_piece_evaluation(mid, m_middle_piece.m_end.m_sector, 0, m_middle_piece.m_end.m_point, itr->second, res_second); 
		cout << "res_second " << res_second << endl;
	    }
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
    cout << "mid " << mid<< " l " << l<< " r "<< r << endl;
    return l;
}

float Referee::find_eq_point(int sector)
{
    float l, r, mid;
    float res_first, res_second;
    
    map<Player*, map<int, float> >::iterator itr;
    Player *player;
    
    if(m_middle_piece.m_begin.m_sector != m_middle_piece.m_end.m_sector)
    {
	l = 0;
	r = 1;
    }
    else if (m_middle_piece.m_begin.m_sector == m_middle_piece.m_end.m_sector)
    {
	l = m_middle_piece.m_begin.m_point;
	r = m_middle_piece.m_end.m_point;
    }
    else if(m_middle_piece.m_begin.m_sector == sector)
    {
	l =  m_middle_piece.m_begin.m_point;
	r = 1;
    }
    else if(m_middle_piece.m_end.m_sector == sector)
    {
      	l = 0;
	r = m_middle_piece.m_end.m_point;
    }

    	cout << "l: " << l << "; r " << r << endl;

    while((r - l) > 0.0000001  )
    {
	mid = (l + r) / 2;
	for (itr = m_players_assigned.begin(); itr != m_players_assigned.end(); itr++)
	{
	    player = itr->first;
	    if(m_pieces_assigned.find(player)->second == 0)
		calculate_piece_evaluation(m_middle_piece.m_begin.m_sector, sector, m_middle_piece.m_begin.m_point, mid, itr->second, res_first); 
	    else
		calculate_piece_evaluation(sector, m_middle_piece.m_end.m_sector, mid,  m_middle_piece.m_end.m_point, itr->second, res_second); 
	}
// 	cout << "fi: " << res_first << "; se " << res_second << endl;
// 	cout << "mid " << mid<< " l " << l<< " r "<< r << endl;
	if(res_first < res_second)
	  l = mid;
	else
	  r = mid;
    }
    cout << "Sector: " << sector << "; Point " << l << endl;
    return l;
}

void Referee::find_eq_sector_multi(int& sector_first, int& sector_second)
{
    int r, m, n, l;
    int fix_one, fix_two, fix_three;
    float res_first, res_second, res_third;
    bool found = false;
    
    map<Player*, map<int, float> >::iterator itr;
    Player *player;
    
    l = 0;
    r = m_cake->get_size() - 1;
    
//     m = floor((l + r + 1) / 3);
    n = floor((l + r + 1) * 2 / 3);
    m = floor(n / 2);
    
    fix_one = m;
    fix_two = n;
    
    cout << "n " << n << " m " << m << " l " << l<< " r "<< r << endl;
    while(!found)
    {
	while( !(l == n))
	{
	    for (itr = m_players_assigned.begin(); itr != m_players_assigned.end(); itr++)
	    {
		player = itr->first;
		switch(m_pieces_assigned.find(player)->second)
		{
		    case 0:
			calculate_piece_evaluation(0, m - 1, 0, 1, itr->second, res_first); 
// 				cout << "res first " << res_first<< endl;
		    break;
		    case 1:
			calculate_piece_evaluation(m, fix_two - 1 , 0, 1, itr->second, res_second); 
// 				cout << "res second " << res_second<< endl;
		    break;
		    case 2:
    // 		    calculate_piece_evaluation(n, m_cake->get_size() - 1 , 0, 1, itr->second, res_third); 
    // 			    cout << "res third " << res_third<< endl;
		    break;
		}
	    }
	    
	    if(res_first < res_second)
	    {
		if( (n - 1) == l)
		{
		    l = n;
		    fix_one = l;
		}
		else
		    l = m;
	    }
	    else
	    {
	      if( (n - 1) == l)
	      {
		  n = l;
		  fix_one = l;
	      }
	      else
		  n = m;
	    }
// 	    fix_one = m;
	    m = (l + n + 1) / 2;
	}
	
	m = fix_one;
	n = fix_two;
	fix_three = fix_two;
	
	while( !(m == r))
	{
	    for (itr = m_players_assigned.begin(); itr != m_players_assigned.end(); itr++)
	    {
		player = itr->first;
		switch(m_pieces_assigned.find(player)->second)
		{
		    case 0:
// 			calculate_piece_evaluation(0, m - 1, 0, 1, itr->second, res_first); 
// 				cout << "res first " << res_first<< endl;
		    break;
		    case 1:
			calculate_piece_evaluation(fix_one, n - 1 , 0, 1, itr->second, res_second); 
// 			    cout << "res second " << res_second<< endl;
		    break;
		    case 2:
			calculate_piece_evaluation(n, m_cake->get_size() - 1 , 0, 1, itr->second, res_third); 
//     			    cout << "res third " << res_third<< endl;
		    break;
		}
	    }
	    
	    if(res_second < res_third)
	    {
		if( (r - 1) == m)
		{
		    m = r;
		    fix_two = m;
		}
		else
		    m = n;
	    }
	    else
	    {
	      if( (r - 1) == m)
	      {
		  r = m;
		  fix_two = m;
	      }
	      else
		  r = n;
	    }
	    n = floor((m + r + 1) / 2);
	}
	if(fix_three == fix_two)
	    found = true;	
	else
	{
	    l = 0;
	    r = m_cake->get_size() - 1;
	    n = floor((fix_three + fix_two + 1) / 2);
	    m = floor(n + 1 / 2);
	    fix_one = m;
	    fix_two = n;
	}
	
// 	if(res_first < res_second)
// 	{
// 	    l = m;
// 	    if(res_third < res_second)
// 		r = n;
// 	    
// 	    m = floor((l + r + 1) / 3);
// 	    n = floor((l + r + 1) * 2 / 3); 
// 	}
// 	else if(res_third < res_second)
// 	{
// 	    r = n;
// 	    m = floor((l + r + 1) / 3);
// 	    n = floor((l + r + 1) * 2 / 3);
// 	}
// 	else
// 	{
// 	    m = floor((l + m + 1) / 2);
// 	    n = floor((n + r + 1) / 2);
// 	}
	    cout << "l " << l  << " m " << m << " n " << n<< " r "<< r << endl;
// 	    found = true;
    }
    	    for (itr = m_players_assigned.begin(); itr != m_players_assigned.end(); itr++)
	    {
		player = itr->first;
		switch(m_pieces_assigned.find(player)->second)
		{
		    case 0:
			calculate_piece_evaluation(0, fix_one - 1, 0, 1, itr->second, res_first); 
				cout << "res first " << res_first<< endl;
		    break;
		    case 1:
			calculate_piece_evaluation(fix_one, fix_two - 1 , 0, 1, itr->second, res_second); 
			    cout << "res second " << res_second<< endl;
		    break;
		    case 2:
			calculate_piece_evaluation(fix_two, m_cake->get_size() - 1 , 0, 1, itr->second, res_third); 
    			    cout << "res third " << res_third<< endl;
		    break;
		}
	    }
	cout << "un " << fix_one  << " du " << fix_two  << endl;    
	sector_first = fix_one;
	sector_second = fix_two;
//     cout << "res first " << res_first<< endl;
//     cout << "res second " << res_second<< endl;
//     cout << "res third " << res_third<< endl;
}

void Referee::find_eq_point_multi(int sector_first, int sector_second, float& eq_first_point, float& eq_second_point)
{
    float l, r, m, n;
    float fix_one, fix_two, fix_three;
    float res_first, res_second, res_third;
    bool found = false;
    
    map<Player*, map<int, float> >::iterator itr;
    Player *player;
    
    l = 0;
    r = 1;
//     m = floor((l + r + 1) / 3);
    m = (l + r) / 2;
    n = m;
    
    fix_one = m;
    fix_two = n;
    
    while(!found)
    {
	while( (r - l) > 0.0000001)
	{
	    for (itr = m_players_assigned.begin(); itr != m_players_assigned.end(); itr++)
	    {
		player = itr->first;
		switch(m_pieces_assigned.find(player)->second)
		{
		    case 0:
			calculate_piece_evaluation(0, sector_first, 0, m, itr->second, res_first); 
// 				cout << "res first " << res_first<< endl;
		    break;
		    case 1:
			calculate_piece_evaluation(sector_first + 1, sector_second , m, fix_two, itr->second, res_second); 
// 				cout << "res second " << res_second<< endl;
		    break;
		    case 2:
    // 		    calculate_piece_evaluation(n, m_cake->get_size() - 1 , 0, 1, itr->second, res_third); 
    // 			    cout << "res third " << res_third<< endl;
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
	
	while( (r - l) > 0.0000001)
	{
	    for (itr = m_players_assigned.begin(); itr != m_players_assigned.end(); itr++)
	    {
		player = itr->first;
		switch(m_pieces_assigned.find(player)->second)
		{
		    case 0:
// 			calculate_piece_evaluation(0, m - 1, 0, 1, itr->second, res_first); 
// 				cout << "res first " << res_first<< endl;
		    break;
		    case 1:
			calculate_piece_evaluation(sector_first + 1, sector_second , fix_one,  n, itr->second, res_second); 
// 			    cout << "res second " << res_second<< endl;
		    break;
		    case 2:
			calculate_piece_evaluation(sector_second + 1 , m_cake->get_size() - 1 , n, 1, itr->second, res_third); 
//     			    cout << "res third " << res_third<< endl;
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
	
	if( ( ((fix_three - fix_two) <= 0.0000001) && (fix_three >= fix_two) ) || ( ((fix_two - fix_three) <= 0.0000001) && (fix_three <= fix_two) ))
	    found = true;	
	else
	{
	    l = 0;
	    r = 1;
	    n = (fix_three + fix_two) / 2;
	    m = (l + r) / 2;
	    fix_one = m;
	    fix_two = n;
	}
	
// 	if(res_first < res_second)
// 	{
// 	    l = m;
// 	    if(res_third < res_second)
// 		r = n;
// 	    
// 	    m = floor((l + r + 1) / 3);
// 	    n = floor((l + r + 1) * 2 / 3); 
// 	}
// 	else if(res_third < res_second)
// 	{
// 	    r = n;
// 	    m = floor((l + r + 1) / 3);
// 	    n = floor((l + r + 1) * 2 / 3);
// 	}
// 	else
// 	{
// 	    m = floor((l + m + 1) / 2);
// 	    n = floor((n + r + 1) / 2);
// 	}

// 	    found = true;
    }
    	    cout << "******"<<  endl;
	    cout << "f1 " << fix_one << " f2 " << fix_two << endl;
	    cout << "******"<<  endl;
    	    for (itr = m_players_assigned.begin(); itr != m_players_assigned.end(); itr++)
	    {
		player = itr->first;
		switch(m_pieces_assigned.find(player)->second)
		{
		    case 0:
			calculate_piece_evaluation(0, sector_first, 0, fix_one, itr->second, res_first); 
				cout << "res first " << res_first<< endl;
		    break;
		    case 1:
			calculate_piece_evaluation(sector_first + 1, sector_second , fix_one,  fix_two, itr->second, res_second); 
			    cout << "res second " << res_second<< endl;
		    break;
		    case 2:
			calculate_piece_evaluation(sector_second + 1 , m_cake->get_size() - 1 , fix_two, 1, itr->second, res_third); 
    			    cout << "res third " << res_third<< endl;
		    break;
		}
	    }
}
