#include "player.h"

Player::Player(string id, Cake* cake)
{
    m_id = id;
    m_cake = cake;
}

Player::~Player()
{

}

void Player::buildEvaluationMap()
{
    int evaluation;
//     srand(time(0));
//     int ev_a[5] = {6,1,9,7,7};
//     int ev_b[5] = {6,4,3,0,1};
//     int ev_c[5] = {3,5,9,7,1};
    
//     int ev_a[5] = {3,1,5,9,4};
//     int ev_b[5] = {4,7,3,9,0};
//     int ev_c[5] = {1,4,6,5,4};
    
        
//     int ev_a[5] = {1,5,8,3,4};
//     int ev_b[5] = {1,1,8,3,0};
//     int ev_c[5] = {1,6,8,0,9};
    
    int ev_a[5] = {2,7,4,8,3};
    int ev_b[5] = {9,3,9,8,8};
    int ev_c[5] = {4,5,4,5,4};
    
    int *ev_s;
    if(m_id == "A")
	ev_s = ev_a;
    else if(m_id == "B")
	ev_s = ev_b;
    else	
	ev_s = ev_c;
    
    cout<< "Player evaluation map:"<<endl;
    for(int i = 0; i < N_SECTOR_TYPE; i++)
    {
	evaluation = (float)rand()/((float)RAND_MAX/10);
	m_evaluation_map.insert(make_pair(i, evaluation));
	cout<< i << " -> " << evaluation<<endl;
// 	cout<< i << " -> " << ev_s[i]<<endl;
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
	m_result += m_evaluation_map.find(type)->second;
    }
    m_halfpoint = m_result / 2;
}

void Player::calculatePieceEvaluation()
{
    int type;
    
    int sect_begin = m_piece_assigned.m_begin.m_sector;
    int sect_end = m_piece_assigned.m_end.m_sector;
    
    float part_begin = m_piece_assigned.m_begin.m_point;
    float part_end = m_piece_assigned.m_end.m_point;
    
    type = m_cake->get_type_at(sect_begin);
    m_piece_result = m_evaluation_map.find(type)->second * part_begin;
    
    for(int i = sect_begin + 1; i < sect_end - 1; i++)
    {
	type = m_cake->get_type_at(i);
	m_piece_result += m_evaluation_map.find(type)->second;
    }
    
    type = m_cake->get_type_at(sect_end);
    m_piece_result = m_evaluation_map.find(type)->second * part_end;
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
    
    cout << "From sector " << m_piece_assigned.m_begin.m_sector << " point " << m_piece_assigned.m_begin.m_point << endl;
    cout << "To sector " << m_piece_assigned.m_end.m_sector << " point " << m_piece_assigned.m_end.m_point << endl;
    
    cout << endl;
}

void Player::cut()
{
    calculateCut();
}

void Player::choose()
{
    int type;
    CakeCut ck = m_cake->get_cake_cut(0);

    float first_ev, second_ev;
    
    first_ev = 0;
    second_ev = 0;
    
    for(int i = 0; i < ck.m_sector; i++)
    {
	type = m_cake->get_type_at(i);
	first_ev += m_evaluation_map.find(type)->second;
    }
    
    first_ev += m_evaluation_map.find(ck.m_sector)->second * ck.m_point;
    cout << "Player B evaluates the first piece of cake as: "<< first_ev <<  endl;
    for(int i = ck.m_sector + 1 ; i < N_SECTORS; i++)
    {
	type = m_cake->get_type_at(i);
	second_ev += m_evaluation_map.find(type)->second;
    }
    
    second_ev += m_evaluation_map.find(ck.m_sector)->second * (1 - ck.m_point);
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
	ev += m_evaluation_map.find(type)->second;
	i++;
    }
    diff = ev - m_halfpoint;
    part = diff / m_evaluation_map.find(type)->second;
    sector = i - 1;
    
    m_cake->set_cake_cut(sector, part, m_id);
}
