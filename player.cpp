#include "player.h"
#include <set>

Player::Player(string id, Cake* cake, int behaviour)
{
    m_id = id;
    m_cake = cake;
    m_entity_type = PLAYER;
    m_behaviour = behaviour;
}

Player::~Player()
{

}

void Player::buildEvaluationMap()
{
    int type;
    float evaluation, norm_evaluation, overall = 0;
    
    cout<< "Player "<< get_id() <<" evaluation map:"<<endl;

    for(int i = 0; i < N_SECTOR_TYPE; i++)
    {
	evaluation = (float)rand()/((float)RAND_MAX/10);
	m_evaluation_map.insert(make_pair(i, evaluation));
    }
    
    for(int i = 0; i < N_SECTORS; i++)
    {
	type = m_cake->get_type_at(i);
	overall += m_evaluation_map.find(type)->second;
    }
    
    for(int i = 0; i < N_SECTOR_TYPE; i++)
    {
	norm_evaluation = m_evaluation_map[i] / overall;
	
	m_norm_evaluation_map.insert(make_pair(i, norm_evaluation));
	cout<< i << " -> " << norm_evaluation<<endl;
    }

     m_true_evaluation_map = m_norm_evaluation_map;
  
    if(m_behaviour == CHEATER)
    {
	cout<<endl;
	cout<< "Player "<< get_id() <<" TRUE evaluation map:"<<endl;

	float val, add_half, sub_half;
	float val_max = 0, val_min = INF;
	int ind_max, ind_min;
	for(int i = 0; i < N_SECTOR_TYPE; i++)
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
	
	for(int i = 0; i < N_SECTOR_TYPE; i++)
	{
	    norm_evaluation = m_evaluation_map[i] / overall;
	    m_true_evaluation_map.find(i)->second = norm_evaluation;
	    cout<< i << " -> " << norm_evaluation<<endl;
	}
    }
    cout<<endl;
}

void Player::calculateTotalEvaluation()
{
    int type;
    
    m_result = 0;
    
    for(int i = 0; i < N_SECTORS; i++)
    {
	type = m_cake->get_type_at(i);
	m_result += m_norm_evaluation_map.find(type)->second;
    }
    m_halfpoint = m_result / 2;
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


void Player::printTotalEvaluation()
{
    cout << "Player "<< m_id << " evaluates the whole cake as:";
    cout << m_result << endl;
    cout << endl;
}

void Player::printPieceInfo()
{
    cout << endl;
	
    cout << "Player "<< m_id << " piece of cake is:" << endl;
    
    cout << "From sector " << m_piece_assigned->get_left_cut_sector() << " point " << m_piece_assigned->get_left_cut_point() << endl;
    cout << "To sector " << m_piece_assigned->get_right_cut_sector() << " point " << m_piece_assigned->get_right_cut_point() << endl;
    
    cout << endl;
}

void Player::cut()
{
    calculateCut();
}

void Player::choose()
{
    int type;
    float first_ev, second_ev;
    CakeCut *ck = m_cake->get_cake_cut(0);

    calculatePieceEvaluation(0, ck->get_cut_sector(), 0, ck->get_cut_point(), first_ev);
    cout << "Player " << m_id << " evaluates the first piece of cake as: "<< first_ev <<  endl;

    calculatePieceEvaluation(ck->get_cut_sector(), N_SECTORS - 1 , ck->get_cut_point(), 1, second_ev);
    cout << "Player " << m_id << " evaluates the second piece of cake as: "<< second_ev <<  endl;
    
    Piece *left_piece;
    Piece *right_piece;
    
    if(first_ev > second_ev)
    {
	left_piece = new Piece(this, new CakeCut(this, 0, 0), ck);
	set_piece(left_piece);
	m_cake->set_chosen(0);
    }
    else
    {
	right_piece = new Piece(this, ck, new CakeCut(this, N_SECTORS - 1, 1));
	set_piece(right_piece);
	m_cake->set_chosen(0);
    }
}

void Player::calculateCut()
{
    int type, sector;
    int i = 0;
    
    float diff, part;
    float ev = 0;
    
    while(m_halfpoint >= ev && i <= (N_SECTORS -1) )
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

void Player::printRealEvaluation()
{
    float result;
    
    calculatePieceEvaluation(m_piece_assigned->get_left_cut_sector(), m_piece_assigned->get_right_cut_sector(), m_piece_assigned->get_left_cut_point(), m_piece_assigned->get_right_cut_point(), result);
    
    cout << "Player "<< m_id << " real evaluation of his own piece is :" << result << endl;
}

void Player::take()
{	
    Piece *left_piece;
    Piece *right_piece;
    
    if(m_cake->get_chosen() == 1)
    {
	left_piece = new Piece(this, new CakeCut(this, 0, 0), m_cake->get_cake_cut(0));
	set_piece(left_piece);    }
    else
    {
	right_piece = new Piece(this, m_cake->get_cake_cut(0), new CakeCut(this, N_SECTORS - 1, 1));
	set_piece(right_piece); 
    }
}