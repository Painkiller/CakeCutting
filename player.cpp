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

void Player::cut()
{

    calculate_cut();

}

void Player::choose()
{
    int type;
    int cut = m_cake->get_cut(0);
    float part = m_cake->get_partial();
    float first_ev, second_ev;
    
    first_ev = 0;
    second_ev = 0;
    
    for(int i = 0; i < cut; i++)
    {
	type = m_cake->get_type_at(i);
	first_ev += m_evaluation_map.find(type)->second;
    }
    
    first_ev += m_evaluation_map.find(cut)->second * part;
    cout << "Player B evaluates the first piece of cake as: "<< first_ev <<  endl;
    for(int i = cut + 1 ; i < N_SECTORS; i++)
    {
	type = m_cake->get_type_at(i);
	second_ev += m_evaluation_map.find(type)->second;
    }
    
    second_ev += m_evaluation_map.find(cut)->second * (1 - part);
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
    
    m_cake->set_cut(i, part, m_id);
}
