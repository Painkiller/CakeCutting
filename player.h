#ifndef PLAYER_H
#define PLAYER_H
#include <map>
#include <stdlib.h>

#include "cake.h"
#include "shared.h"

using namespace std;

class Player
{

public:
    Player();
    virtual ~Player();
    void build_evaluation_map();
    void calculate_total_evaluation(Cake *cake);
    void print_total_evaluation();
    void cut(Cake* cake);
    void choose(Cake* cake);

    
private:
    map<int, float> m_evaluation_map;
    float m_result;
    int m_chosen;
};

#endif // PLAYER_H
