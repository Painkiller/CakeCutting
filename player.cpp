#include "player.h"

Player::Player(string id, Cake* cake)
{
    m_id = id;
    m_cake = cake;
    m_entity_type = PLAYER;
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
    cout << "Player "<< m_id << " evaluates the whole cake as:" << endl;
    cout << m_result << endl;
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
    CakeCut *ck = m_cake->get_cake_cut(0);

    float first_ev, second_ev;
    
    first_ev = 0;
    second_ev = 0;
    
    for(int i = 0; i < ck->get_cut_sector(); i++)
    {
	type = m_cake->get_type_at(i);
	first_ev += m_norm_evaluation_map.find(type)->second;
    }
    
    first_ev += m_norm_evaluation_map.find(ck->get_cut_sector())->second * ck->get_cut_point();
    cout << "Player B evaluates the first piece of cake as: "<< first_ev <<  endl;
    for(int i = ck->get_cut_sector() + 1 ; i < N_SECTORS; i++)
    {
	type = m_cake->get_type_at(i);
	second_ev += m_norm_evaluation_map.find(type)->second;
    }
    
    second_ev += m_evaluation_map.find(ck->get_cut_sector())->second * (ck->get_cut_point());
    cout << "Player B evaluates the second piece of cake as: "<< second_ev <<  endl;
    if(first_ev > second_ev)
      m_chosen = 1;
    else
      m_chosen = 2;
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
    diff = ev - m_halfpoint;
    part = diff / m_norm_evaluation_map.find(type)->second;
    sector = i - 1;
    
    m_cake->setCakeCut((Entity*)this, sector, part);
}
