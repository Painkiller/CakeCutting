#ifndef PLAYER_H
#define PLAYER_H
#include <map>
#include <string>
#include <fstream>
#include <math.h>
#include <stdlib.h>


#include "entity.h"
#include "cake.h"
#include "shared.h"
#include "util.h";

using namespace std;
using namespace util;

class Player : public Entity
{

public:
    Player(string id, Cake* cake, int behaviour);
    virtual ~Player();
    void buildEvaluationMap(int problem);
    void calculateTotalEvaluation();
    void calculatePieceEvaluation(int sect_begin, int sect_end, float point_begin, float point_end, float& result);
    void calculateTruePieceEvaluation(int sect_begin, int sect_end, float point_begin, float point_end, float& result);
    void printTotalEvaluation();
    void printPieceInfo();
    void cut();
    void choose();
    
    inline void set_piece(Piece *piece){m_piece_assigned = piece;}
    inline void clear_piece()
    {
	
    }
    inline void clear_maps()
    {
	m_evaluation_map.clear();
	m_norm_evaluation_map.clear();
	m_true_evaluation_map.clear();
    }
    inline string& get_id(){return m_id;}
    inline map<int, float> get_evaluation_map(){return m_norm_evaluation_map;}
    CakeCut* getCakecut();
    
    float getRealPieceEvaluation();
    float getPieceEvaluation();

    void take();
    
private:
    float m_result;
    float m_piece_result;
    float m_halfpoint;
    
    int m_chosen;
    int m_behaviour;
    
    string m_id;
    
    map<int, float> m_evaluation_map;
    map<int, float> m_norm_evaluation_map;
    map<int, float> m_true_evaluation_map;

    Piece *m_piece_assigned;
    CakeCut* m_player_cut;
    
    void calculateCut();
};

#endif // PLAYER_H
