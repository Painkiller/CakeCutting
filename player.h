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
    string m_id;
    map<int, float> m_evaluation_map;
    float m_result;
    int m_chosen;
};

#endif // PLAYER_H
