#ifndef PLAYER_H
#define PLAYER_H
#include <map>
#include <stdlib.h>

#include "entity.h"
#include "cake.h"
#include "shared.h"

using namespace std;

class Player : public Entity
{

public:
    Player(string id, Cake* cake);
    virtual ~Player();
    void build_evaluation_map();
    void calculate_total_evaluation();
    void print_total_evaluation();
    void cut();
    void choose();
    
private:
    void calculate_cut();
  
  
    float m_result;
    float m_halfpoint;
    
    int m_chosen;

    string m_id;
    
    map<int, float> m_evaluation_map;
};

#endif // PLAYER_H
