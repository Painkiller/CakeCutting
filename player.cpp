#include "player.h"
#include <set>

Player::Player(string id, Cake* cake, int behaviour)
{
    m_id = id;
    m_cake = cake;
    m_entity_type = PLAYER;
    m_behaviour = behaviour;
    m_player_cut = new CakeCut();
}

Player::~Player()
{

}

void Player::buildEvaluationMap(ProblemType problem, StrategyType strategy)
{
    int type;
    int n_sector_type = m_cake->get_n_sector_type();
    int n_sectors = m_cake->get_size();
    
    float evaluation, norm_evaluation, overall = 0;
    
    switch(problem)
    {
	case RANDOM:
	{
	    if(isLogEnabled())
	      cout<< "Player "<< get_id() <<" evaluation map:"<<endl;

	    for(int i = 0; i < n_sector_type ; i++)
	    {
		evaluation = (float)rand()/((float)RAND_MAX/10);
		m_evaluation_map.insert(make_pair(i, evaluation));
	    }
	    
	    for(int i = 0; i < n_sectors; i++)
	    {
		type = m_cake->get_type_at(i);
		overall += m_evaluation_map.find(type)->second;
	    }
	    
	    for(int i = 0; i < n_sector_type; i++)
	    {
		norm_evaluation = m_evaluation_map[i] / overall;
		
		m_norm_evaluation_map.insert(make_pair(i, norm_evaluation));
		m_true_evaluation_map.insert(make_pair(i, norm_evaluation));
		if(isLogEnabled())
		    cout<< i << " -> " << norm_evaluation<<endl;
	    }

// 	    m_true_evaluation_map = m_norm_evaluation_map;
// 	    std::copy( m_true_evaluation_map.begin(), m_true_evaluation_map.end(), m_norm_evaluation_map.begin() );
	    if(m_behaviour == CHEATER)
	    {
		if(isLogEnabled())
		{
		    cout<<endl;
		    cout<< "Player "<< get_id() <<" TRUE evaluation map:"<<endl;
		}
		switch(strategy)
		{
		    case SUBMAX_ADDMIN:
		    {
			float val, add_half, sub_half;
			float val_max = 0, val_min = INF;
			int ind_max, ind_min;
			for(int i = 0; i < n_sector_type; i++)
			{
			    val =  m_evaluation_map.find(i)->second * m_cake->countSectorOccurrences(i);
			    if(val)
			    {
				if(val <= val_min)
				{
				    val_min = val;
				    ind_min = i;
				}
				if(val >= val_max)
				{
				    val_max = val;
				    ind_max = i;
				}
			    }
			}
			
			val_min /= 2;
			add_half = val_min / m_cake->countSectorOccurrences(ind_max);
			sub_half = val_min / m_cake->countSectorOccurrences(ind_min);
			
			m_evaluation_map.find(ind_max)->second -= add_half;
			m_evaluation_map.find(ind_min)->second += sub_half;
			
			for(int i = 0; i < n_sector_type; i++)
			{
			    norm_evaluation = m_evaluation_map[i] / overall;
			    m_true_evaluation_map.find(i)->second = norm_evaluation;
			    if(isLogEnabled())
				cout<< i << " -> " << norm_evaluation<<endl;
			}
		    }
		    break;
		    case ADDMAX_SUBMIN:
		    {
			float val, add_half, sub_half;
			float val_max = 0, val_min = INF;
			int ind_max, ind_min;
			for(int i = 0; i < n_sector_type; i++)
			{
			    val =  m_evaluation_map.find(i)->second * m_cake->countSectorOccurrences(i);
			    if(val)
			    {
				if(val <= val_min)
				{
				    val_min = val;
				    ind_min = i;
				}
				if(val >= val_max)
				{
				    val_max = val;
				    ind_max = i;
				}
			    }
			}
			
			val_min /= 2;
			add_half = val_min / m_cake->countSectorOccurrences(ind_max);
			sub_half = val_min / m_cake->countSectorOccurrences(ind_min);
			
			m_evaluation_map.find(ind_max)->second += add_half;
			m_evaluation_map.find(ind_min)->second -= sub_half;
			
			for(int i = 0; i < n_sector_type; i++)
			{
			    norm_evaluation = m_evaluation_map[i] / overall;
			    m_true_evaluation_map.find(i)->second = norm_evaluation;
			    if(isLogEnabled())
				cout<< i << " -> " << norm_evaluation<<endl;
			}
		    }
		    break;
		    case SWAP:
		    {
			float val, old_max = INF, old_min = 0;
			float val_max = 0, val_min = INF;
			int ind_max, ind_min;
			float tmp_min, tmp_max;
// 			if(size % 2)
			for(int k = 0; k < n_sector_type / 2; k++)
			{
			    for(int i = 0; i < n_sector_type; i++)
			    {
				val =  m_true_evaluation_map.find(i)->second;
				if(val)
				{
				    if(val <= val_min && val > old_min)
				    {
					val_min = val;
					ind_min = i;
				    }
				    if(val >= val_max && val < old_max)
				    {
					val_max = val;
					ind_max = i;
				    }
				}
			    }
			    tmp_min = m_true_evaluation_map.find(ind_min)->second;
			    tmp_max = m_true_evaluation_map.find(ind_max)->second;
			    m_true_evaluation_map.find(ind_min)->second = tmp_max;
			    m_true_evaluation_map.find(ind_max)->second = tmp_min;

			    old_min = val_min;
			    old_max = val_max;
			    val_min = INF;
			    val_max = 0;
			}
			for(int i = 0; i < n_sector_type; i++)
			{
			    if(isLogEnabled())
				cout<< i << " -> " << m_true_evaluation_map.at(i)<<endl;
			}
		    }
		    break;
		    case FLAT:
		    {
			float val = 1.0 / m_cake->get_size();
			for(int i = 0; i < n_sector_type; i++)
			{
			    
			    m_true_evaluation_map.find(i)->second = val;
			    if(isLogEnabled())
				cout<< i << " -> " << m_true_evaluation_map.at(i)<<endl;
			}
		    }
		    break;
		}
	    }
	    if(isLogEnabled())
		cout<<endl;
	    break;
	}
	case BOSNIA:
	{
	    int num;
	    int j = 0;
	    int values[103];
	    
	    char* ifile;
	    
	    string tmps;
	    ifstream indata;
	    
	    tmps = "../data" + m_id;
	    ifile=(char*)tmps.c_str();
	    
	    cout<< ifile <<endl;
	    
	    indata.open(ifile);
	    
	    if(!indata) 
	    {
		cerr << "Error nel aprire data.data" << endl;
		exit(1);
	    }
	    indata >> num;
	    while (!indata.eof()) 
	    {
		values[j] = num;
		j++;
		indata >> num;
	    }
	     
	    for(int i = 0; i < N_BOSNIA_SECTORS; i++)
	    {
		evaluation = values[i];
		m_evaluation_map.insert(make_pair(i, evaluation));
	    }
	    
	    for(int i = 0; i < N_BOSNIA_SECTORS; i++)
	    {
		type = m_cake->get_type_at(i);
		overall += m_evaluation_map.find(type)->second;
	    }
	    
	    for(int i = 0; i < N_BOSNIA_SECTORS; i++)
	    {
		norm_evaluation = m_evaluation_map[i];	

		m_norm_evaluation_map.insert(make_pair(i, norm_evaluation));
		cout<< i << " -> " << norm_evaluation<<endl;
	    }
	}
    }
}

void Player::calculateTotalEvaluation()
{
    int type;
    int n_sectors = m_cake->get_size();
    m_result = 0;
    
    for(int i = 0; i < n_sectors; i++)
    {
	type = m_cake->get_type_at(i);
	m_result += m_norm_evaluation_map.find(type)->second;
    }
    m_halfpoint = m_result / 2;
}

void Player::calculateTruePieceEvaluation(int sect_begin, int sect_end, float point_begin, float point_end, float& result)
{
    int type;
    
    type = m_cake->get_type_at(sect_begin);
    
    if(sect_end < sect_begin)
    {
	result = 0;
	return;
    }
    if(sect_begin == sect_end)
      result = m_true_evaluation_map.find(type)->second * (point_end - point_begin);
    else
      result = m_true_evaluation_map.find(type)->second * (1 - point_begin);

    if(sect_begin != sect_end)
    {
	if( (sect_begin + 1) != sect_end)
	{
	    for(int i = (sect_begin + 1); i < sect_end; i++)
	    {
		type = m_cake->get_type_at(i);
		result += m_true_evaluation_map.find(type)->second;
	    }
	}
	type = m_cake->get_type_at(sect_end);
	result += m_true_evaluation_map.find(type)->second * point_end;
    }
}

void Player::calculatePieceEvaluation(int sect_begin, int sect_end, float point_begin, float point_end, float& result)
{
    int type;
    
    type = m_cake->get_type_at(sect_begin);
    
    if(sect_end < sect_begin)
    {
	result = 0;
	return;
    }
    if(sect_begin == sect_end)
      result = m_norm_evaluation_map.find(type)->second * (point_end - point_begin);
    else
      result = m_norm_evaluation_map.find(type)->second * (1 - point_begin);

    if(sect_begin != sect_end)
    {
	if( (sect_begin + 1) != sect_end)
	{
	    for(int i = (sect_begin + 1); i < sect_end; i++)
	    {
		type = m_cake->get_type_at(i);
		result += m_norm_evaluation_map.find(type)->second;
	    }
	}
	type = m_cake->get_type_at(sect_end);
	result += m_norm_evaluation_map.find(type)->second * point_end;
    }
}

void Player::printTotalEvaluation()
{
    if(isLogEnabled())
    {
	cout << "Player "<< m_id << " evaluates the whole cake as:";
	cout << m_result << endl;
	cout << endl;
    }
}

void Player::printPieceInfo()
{
    if(isLogEnabled())
    {
	cout << endl;
	    
	cout << "Player "<< m_id << " piece of cake is:" << endl;
	
	cout << "From sector " << m_piece_assigned->get_left_cut_sector() << " point " << m_piece_assigned->get_left_cut_point() << endl;
	cout << "To sector " << m_piece_assigned->get_right_cut_sector() << " point " << m_piece_assigned->get_right_cut_point() << endl;
	
	cout << endl;
    }
}

void Player::cut()
{
    calculateCut();
}

void Player::choose()
{
    int type;
    int n_sectors = m_cake->get_size();
    float first_ev, second_ev;
    
    CakeCut *ck = m_cake->get_cake_cut(0);

    calculatePieceEvaluation(0, ck->get_cut_sector(), 0, ck->get_cut_point(), first_ev);	    
    
    if(isLogEnabled())
	cout << "Player " << m_id << " evaluates the first piece of cake as: "<< first_ev <<  endl;

    calculatePieceEvaluation(ck->get_cut_sector(), n_sectors - 1 , ck->get_cut_point(), 1, second_ev);
    if(isLogEnabled())
	cout << "Player " << m_id << " evaluates the second piece of cake as: "<< second_ev <<  endl;
    
    Piece *piece;
    CakeCut *cut_left, *cut_right;
    
    if(first_ev > second_ev)
    {
	piece = m_cake->get_piece(0, ANY);
	cut_left = piece->get_ck_left();
	cut_left->set_cakecut(this, 0, 0);
	piece->set_piece(this, cut_left, ck);
	m_cake->set_chosen(0);
    }
    else
    {
	piece = m_cake->get_piece(1, ANY);
	cut_right= piece->get_ck_right();
	cut_right->set_cakecut(this, n_sectors - 1, 1);
	piece->set_piece(this, ck, cut_right);
	m_cake->set_chosen(1);
    }
    set_piece(piece);

}

void Player::calculateCut()
{
    int type, sector;
    int i = 0;
    int n_sectors = m_cake->get_size();
    
    float diff, part;
    float ev = 0;
    
    
    while(m_halfpoint >= ev && i <= (n_sectors -1) )
    {
	type = m_cake->get_type_at(i);
	ev += m_norm_evaluation_map.find(type)->second;
	i++;
    }
    diff = fabs(ev - m_halfpoint);
    part = ( m_norm_evaluation_map.find(type)->second - diff )/ m_norm_evaluation_map.find(type)->second;
    sector = i - 1;
    
    m_cake->setCakeCut((Entity*)this, sector, part);
}

float Player::getRealPieceEvaluation()
{
    float result;
    
    calculateTruePieceEvaluation(m_piece_assigned->get_left_cut_sector(), m_piece_assigned->get_right_cut_sector(), m_piece_assigned->get_left_cut_point(), m_piece_assigned->get_right_cut_point(), result);
    if(isLogEnabled())
	cout << "Player "<< m_id << " real evaluation of his own piece is :" << result << endl;
    
    return result;
}

float Player::getPieceEvaluation()
{
    float result;
    
    calculatePieceEvaluation(m_piece_assigned->get_left_cut_sector(), m_piece_assigned->get_right_cut_sector(), m_piece_assigned->get_left_cut_point(), m_piece_assigned->get_right_cut_point(), result);
    if(isLogEnabled())
	cout << "Player "<< m_id << " evaluation of his own piece is :" << result << endl;
    
    return result;
}

CakeCut* Player::getCakecut()
{
    return m_player_cut;
}
void Player::take()
{	
  
    int n_sectors = m_cake->get_size();
    int chosen = m_cake->get_chosen();
    Piece *piece;
    CakeCut *cut_left, *cut_right;

    piece = m_cake->get_piece(chosen, ANY);

    if(!chosen)
    {
        cut_left = piece->get_ck_left();
	cut_left->set_cakecut(this, 0, 0);
	piece->set_piece(this, cut_left, m_cake->get_cake_cut(0));
    }
    else
    {
        cut_right= piece->get_ck_right();
	cut_right->set_cakecut(this, n_sectors - 1, 1);
	piece ->set_piece(this, m_cake->get_cake_cut(0), cut_right);
    }
    
    set_piece(piece); 
}