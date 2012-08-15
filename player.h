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
    void buildEvaluationMap();
    void calculateTotalEvaluation();
    void calculatePieceEvaluation();
    void printTotalEvaluation();
    void printPieceInfo();
    void cut();
    void choose();
    
    inline void set_piece(Piece *piece){m_piece_assigned = piece;}
    inline string& get_id(){return m_id;}
    inline map<int, float> get_evaluation_map(){return m_norm_evaluation_map;}
    
private:
    float m_result;
    float m_piece_result;
    float m_halfpoint;
    
    int m_chosen;

    string m_id;
    
    map<int, float> m_evaluation_map;
    map<int, float> m_norm_evaluation_map;

    Piece *m_piece_assigned;
    
    
    void calculateCut();
};

#endif // PLAYER_H
