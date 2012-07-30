#include "player.h"

Player::Player()
{

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

void Player::calculate_total_evaluation(Cake *cake)
{
    int type;
    
    m_result = 0;
    
    for(int i = 0; i < N_SECTORS; i++)
    {
	type = cake->get_type_at(i);
	m_result += m_evaluation_map.find(type)->second;
    }
}

void Player::print_total_evaluation()
{
    cout << "Player A evaluates the whole cake as:" << endl;
    cout << m_result << endl;
}

void Player::cut(Cake* cake)
{
    int type;
    int i = 0;
    
    float diff, part;
    float half = m_result / 2;
    float ev = 0;
    
    while(half >= ev)
    {
	type = cake->get_type_at(i);
	ev += m_evaluation_map.find(type)->second;
	i++;
    }
    diff = ev - half;
    part = diff / m_evaluation_map.find(type)->second;
    
    cake->set_cut(i, part);
}

void Player::choose(Cake* cake)
{
    int type;
    int cut = cake->get_cut();
    float part = cake->get_partial();
    float first_ev, second_ev;
    
    first_ev = 0;
    second_ev = 0;
    
    for(int i = 0; i < cut; i++)
    {
	type = cake->get_type_at(i);
	first_ev+= m_evaluation_map.find(type)->second;
    }
    
    first_ev += m_evaluation_map.find(cut)->second * part;
    cout << "Player B evaluates the first piece of cake as: "<< first_ev <<  endl;
    for(int i = cut + 1 ; i < N_SECTORS; i++)
    {
	type = cake->get_type_at(i);
	second_ev += m_evaluation_map.find(type)->second;
    }
    
    second_ev += m_evaluation_map.find(cut)->second * (1 - part);
    cout << "Player B evaluates the second piece of cake as: "<< second_ev <<  endl;
    if(first_ev > second_ev)
      m_chosen = 1;
    else
      m_chosen = 2;
}
