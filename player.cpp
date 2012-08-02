#include "player.h"

Player::Player(string id, Cake* cake)
{
    m_id = id;
    m_cake = cake;
}

Player::~Player()
{

}

void Player::build_evaluation_map()
{
    float evaluation;
//     srand(time(0));

    cout<< "Player evaluation map:"<<endl;
    for(int i = 0; i < N_SECTOR_TYPE; i++)
    {
	evaluation = (float)rand()/((float)RAND_MAX/10);
	m_evaluation_map.insert(make_pair(i, evaluation));
	cout<< i << " -> " << evaluation<<endl;
    }
     cout<<endl;
}

void Player::calculate_total_evaluation()
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

void Player::print_total_evaluation()
{
    cout << "Player A evaluates the whole cake as:" << endl;
    cout << m_result << endl;
}

void Player::print_piece_info()
{
    cout << endl;
	
    cout << "Player "<< m_id << " piece of cake is:" << endl;
    
    cout << "From sector " << m_piece_assigned.m_begin.m_sector << " perc " << m_piece_assigned.m_begin.m_part << endl;
    cout << "From sector " << m_piece_assigned.m_end.m_sector << " perc " << m_piece_assigned.m_end.m_part << endl;
    
    cout << endl;
}

void Player::cut()
{
    calculate_cut();
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
    
    first_ev += m_evaluation_map.find(ck.m_sector)->second * ck.m_part;
    cout << "Player B evaluates the first piece of cake as: "<< first_ev <<  endl;
    for(int i = ck.m_sector + 1 ; i < N_SECTORS; i++)
    {
	type = m_cake->get_type_at(i);
	second_ev += m_evaluation_map.find(type)->second;
    }
    
    second_ev += m_evaluation_map.find(ck.m_sector)->second * (1 - ck.m_part);
    cout << "Player B evaluates the second piece of cake as: "<< second_ev <<  endl;
    if(first_ev > second_ev)
      m_chosen = 1;
    else
      m_chosen = 2;
}

void Player::calculate_cut()
{
    int type;
    int i = 0;
    
    float diff, part;
    float ev = 0;
    
    while(m_halfpoint >= ev)
    {
	type = m_cake->get_type_at(i);
	ev += m_evaluation_map.find(type)->second;
	i++;
    }
    diff = ev - m_halfpoint;
    part = diff / m_evaluation_map.find(type)->second;
    
    m_cake->set_cake_cut(i, part, m_id);
}
