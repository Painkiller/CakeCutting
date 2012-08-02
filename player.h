#ifndef PLAYER_H
#define PLAYER_H
#include <map>
#include <stdlib.h>
#include <string>

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
    void calculate_piece_evaluation();
    void print_total_evaluation();
    void print_piece_info();
    void cut();
    void choose();
    inline void set_piece(Piece piece){m_piece_assigned = piece;}
    string& get_id(){return m_id;}

    
private:
    void calculate_cut();
  
  
    float m_result;
    float m_piece_result;
    float m_halfpoint;
    
    int m_chosen;

    string m_id;
    
    map<int, float> m_evaluation_map;
    
    Piece m_piece_assigned;
};

#endif // PLAYER_H
