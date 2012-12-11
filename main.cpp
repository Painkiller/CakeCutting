#include <iostream>

#include "player.h"
#include "referee.h"
#include "cake.h"
#include "stats.h"

void cutAndChoose(Player *player_a, Player *player_b, Stats *stats_a, Stats *stats_b, int j);

void surplusProcedure(Player *player_a, Player *player_b, Referee *referee, Stats *stats_a, Stats *stats_b, int j);
void equitabilityProcedure(Player *player_a, Player *player_b, Player *player_c, Referee *referee, Stats *stats_a, Stats *stats_b, Stats *stats_c, int j);


int main(int argc, char **argv) 
{    
    srand(time(0));
    util::enableLog(false);
    
    int problem_id;
    ProblemType problem;
    bool skip = false;
    int iter = 10000;
    Cake *cake = new Cake();
   
    Player *player_a = new Player("A", cake, CHEATER);
    Player *player_b = new Player("B", cake, CHEATER);
    Player *player_c = new Player("C", cake, CHEATER);

    
    Referee *referee = new Referee(cake);
    
    Stats *stats_a = new Stats();
    Stats *stats_b = new Stats();
    Stats *stats_c = new Stats();
    while(!skip)
    {
	cout <<"Select initialization of the problem: (0: Random, 1: Bosnia)" << endl;
	cin >> problem_id;
    
	switch(problem_id)
	{	
	  case RANDOM:
	  {
	      skip = true;
	      problem = RANDOM;
	      break;
	  }
	  case BOSNIA:
	  {
	      skip = true;
	      problem = BOSNIA;
	      break;
	  }
	  default:
	    break;
	}
    }
    //building cake
//     cake->buildCake(problem);
//     if(isLogEnabled())
// 	cake->printSectors();
    
    int n = 10;
    int m = 2;
    for(int k = 0; k < 1; k++)
    {
	stats_a->reset_stats();
	stats_b->reset_stats();
	stats_c->reset_stats();
	for(int i = 0; i < iter; i++)
	{	
	    bool no_strategy = false;
	    cake->buildCake(problem, n, m);
	    //building players' evaluations
	    if(isLogEnabled() || i == 1)
		cake->printSectors();
	    for(int j = 0; j < 2; j++)
	    {
		if(j == 1)
		{
		    no_strategy = true;
		}
		
		player_a->buildEvaluationMap(problem, SWAP, no_strategy);
		player_b->buildEvaluationMap(problem, SWAP, no_strategy);
		
		referee->assignPlayer(player_a);
		referee->assignPlayer(player_b);
		
		//***Cut & Choose Method***
		if(isLogEnabled())
		{
		    cout << endl;
		    cout <<"*************CUT AND CHOOSE************" << endl;
		    cout << endl;
		}
		cutAndChoose(player_a, player_b, stats_a, stats_b, j);
		
		//Clear previous cuts
		cake->clear_cuts();
		if(isLogEnabled())
		{
		    cout << endl;
		    cout <<"*************SURPLUS PROCEDURE************" << endl;
		    cout << endl;
		}
		//***Surplus Procedure**
		surplusProcedure(player_a, player_b, referee, stats_a, stats_b, j);
		
		cake->clear_cuts();
		referee->clear_pieces();
		
		if(isLogEnabled())
		{
		    cout << endl;
		    cout <<"*************EQUITABILITY PROCEDURE************" << endl;
		    cout << endl;
		}
		
		player_c->buildEvaluationMap(problem, SWAP, no_strategy);
		referee->assignPlayer(player_c);
			
        	equitabilityProcedure(player_a, player_b, player_c, referee, stats_a, stats_b, stats_c, j);
		
		cake->clear_cuts();
		referee->clear_pieces();
		referee->clear_result_map();

		referee->unassign_players();
	    }
	    if(i % 100 == 0)
		cout << i << endl;
	    player_a->clear_maps();
	    player_b->clear_maps();
	    player_c->clear_maps();
	}
	cout <<"Sectors " << n << " Type " << m << ":"<<endl;
    cout << "########### PLAYER A STATS ###########" << endl;
    cout << endl;
    stats_a->print_all_stats(iter);
    cout << "########### PLAYER B STATS ###########" << endl;
    cout << endl;
    stats_b->print_all_stats(iter);
    cout << "########### PLAYER C STATS ###########" << endl;
    cout << endl;
    stats_c->print_all_stats(iter);
//     n+=10;
    m++;
    }
    return 0;
}

void cutAndChoose(Player *player_a, Player *player_b, Stats *stats_a, Stats *stats_b, int j)
{ 
    float map_gap, piece_gap, player_gap, compare_gap;
    //evaluation
    player_a->calculateTotalEvaluation();
    
    player_a->printTotalEvaluation();
    
    //cut
    player_a->cut();
    
    //choose
    player_b->choose();
    player_a->take();
    if(isLogEnabled())
      cout << endl;
    
    if(j == 0)
    {
	map_gap = player_a->getRealPieceEvaluation(player_a) - player_a->getPieceEvaluation(player_a);
	if(map_gap > 0)
	    stats_a->inc(true, map_gap, MAP_STAT, CC_METHOD);
	else
	    stats_a->inc(false, map_gap, MAP_STAT, CC_METHOD);
	
	map_gap = player_b->getRealPieceEvaluation(player_b) - player_b->getPieceEvaluation(player_b);
	if(map_gap > 0)
	    stats_b->inc(true, map_gap, MAP_STAT, CC_METHOD);
	else
	    stats_b->inc(false, map_gap, MAP_STAT, CC_METHOD);
	
	piece_gap = player_a->getRealPieceEvaluation(player_a) - player_a->getRealPieceEvaluation(player_b);
	if(piece_gap > 0)
	    stats_a->inc(true, piece_gap, PIECE_STAT, CC_METHOD);
	else
	    stats_a->inc(false, piece_gap, PIECE_STAT, CC_METHOD);
	
	piece_gap = player_b->getRealPieceEvaluation(player_b) - player_b->getRealPieceEvaluation(player_a);
	if(piece_gap > 0)
	    stats_b->inc(true, piece_gap, PIECE_STAT, CC_METHOD);
	else
	    stats_b->inc(false, piece_gap, PIECE_STAT, CC_METHOD);
	
	player_gap = player_a->getRealPieceEvaluation(player_a) - player_b->getRealPieceEvaluation(player_b);
	if(player_gap > 0)
	    stats_a->inc(true, player_gap, PLAYER_STAT, CC_METHOD);
	else
	    stats_a->inc(false, player_gap, PLAYER_STAT, CC_METHOD);
	
	player_gap = player_b->getRealPieceEvaluation(player_b) - player_a->getRealPieceEvaluation(player_a);
	if(player_gap > 0)
	    stats_b->inc(true, player_gap, PLAYER_STAT, CC_METHOD);
	else
	    stats_b->inc(false, player_gap, PLAYER_STAT, CC_METHOD);
	
	stats_a->store(player_a->getRealPieceEvaluation(player_a));
	stats_b->store(player_b->getRealPieceEvaluation(player_b));
    }
    else
    {
	compare_gap = stats_a->load() - player_a->getRealPieceEvaluation(player_a);
	if(compare_gap > 0)
	    stats_a->inc(true, compare_gap, COMPARE_STAT, CC_METHOD);
	else
	    stats_a->inc(false, compare_gap, COMPARE_STAT, CC_METHOD);
	
	compare_gap = stats_b->load() - player_b->getRealPieceEvaluation(player_b);
	if(compare_gap > 0)
	    stats_b->inc(true, compare_gap, COMPARE_STAT, CC_METHOD);
	else
	    stats_b->inc(false, compare_gap, COMPARE_STAT, CC_METHOD);
    }
}

void surplusProcedure(Player *player_a, Player *player_b, Referee *referee, Stats *stats_a, Stats *stats_b, int j)
{
    player_a->calculateTotalEvaluation();
    player_b->calculateTotalEvaluation();
    
    player_a->cut();
    player_b->cut();
    
    referee->handleHalfpoints();
    
    player_a->printPieceInfo();
    player_b->printPieceInfo();
    
    referee->handleMiddle();
    
    float map_gap, piece_gap, player_gap, compare_gap;
    if(j == 0)
    {
	map_gap = player_a->getRealPieceEvaluation(player_a) - player_a->getPieceEvaluation(player_a);
	if(map_gap > 0)
	    stats_a->inc(true, map_gap, MAP_STAT, SP_METHOD);
	else
	    stats_a->inc(false, map_gap, MAP_STAT, SP_METHOD);
	
	map_gap = player_b->getRealPieceEvaluation(player_b) - player_b->getPieceEvaluation(player_b);
	if(map_gap > 0)
	    stats_b->inc(true, map_gap, MAP_STAT, SP_METHOD);
	else
	    stats_b->inc(false, map_gap, MAP_STAT, SP_METHOD);
	
	piece_gap = player_a->getRealPieceEvaluation(player_a) - player_a->getRealPieceEvaluation(player_b);
	if(piece_gap > 0)
	    stats_a->inc(true, piece_gap, PIECE_STAT, SP_METHOD);
	else
	    stats_a->inc(false, piece_gap, PIECE_STAT, SP_METHOD);
	
	piece_gap = player_b->getRealPieceEvaluation(player_b) - player_b->getRealPieceEvaluation(player_a);
	if(piece_gap > 0)
	    stats_b->inc(true, piece_gap, PIECE_STAT, SP_METHOD);
	else
	    stats_b->inc(false, piece_gap, PIECE_STAT, SP_METHOD);
	
	player_gap = player_a->getRealPieceEvaluation(player_a) - player_b->getRealPieceEvaluation(player_b);
	if(player_gap > 0)
	    stats_a->inc(true, player_gap, PLAYER_STAT, SP_METHOD);
	else
	    stats_a->inc(false, player_gap, PLAYER_STAT, SP_METHOD);
	
	player_gap = player_b->getRealPieceEvaluation(player_b) - player_a->getRealPieceEvaluation(player_a);
	if(player_gap > 0)
	    stats_b->inc(true, player_gap, PLAYER_STAT, SP_METHOD);
	else
	    stats_b->inc(false, player_gap, PLAYER_STAT, SP_METHOD);
	    
	stats_a->store(player_a->getRealPieceEvaluation(player_a));
	stats_b->store(player_b->getRealPieceEvaluation(player_b));
    }
    else
    {
	compare_gap = stats_a->load() - player_a->getRealPieceEvaluation(player_a);
	if(compare_gap > 0)
	    stats_a->inc(true, compare_gap, COMPARE_STAT, SP_METHOD);
	else
	    stats_a->inc(false, compare_gap, COMPARE_STAT, SP_METHOD);
	
	compare_gap = stats_b->load() - player_b->getRealPieceEvaluation(player_b);
	if(compare_gap > 0)
	    stats_b->inc(true, compare_gap, COMPARE_STAT, SP_METHOD);
	else
	    stats_b->inc(false, compare_gap, COMPARE_STAT, SP_METHOD);
    }
}

void equitabilityProcedure(Player *player_a, Player *player_b, Player *player_c, Referee *referee, Stats *stats_a, Stats *stats_b, Stats *stats_c, int j)
{
    float map_gap, piece_gap, player_gap, compare_gap;
    referee->handleEquitability();
    
    if(j == 0)
    {
	map_gap = player_a->getRealPieceEvaluation(player_a) - player_a->getPieceEvaluation(player_a);
	if(map_gap > 0)
	    stats_a->inc(true, map_gap, MAP_STAT, EP_METHOD);
	else
	    stats_a->inc(false, map_gap, MAP_STAT, EP_METHOD);
	
	map_gap = player_b->getRealPieceEvaluation(player_b) - player_b->getPieceEvaluation(player_b);
	if(map_gap > 0)
	    stats_b->inc(true, map_gap, MAP_STAT, EP_METHOD);
	else
	    stats_b->inc(false, map_gap, MAP_STAT, EP_METHOD);
	    
	map_gap = player_c->getRealPieceEvaluation(player_c) - player_c->getPieceEvaluation(player_c);
	if(map_gap > 0)
	    stats_c->inc(true, map_gap, MAP_STAT, EP_METHOD);
	else
	    stats_c->inc(false, map_gap, MAP_STAT, EP_METHOD);
	
	stats_a->store(player_a->getRealPieceEvaluation(player_a));
	stats_b->store(player_b->getRealPieceEvaluation(player_b));
	stats_c->store(player_c->getRealPieceEvaluation(player_c));
    }
    else
    {
      	compare_gap = stats_a->load() - player_a->getRealPieceEvaluation(player_a);
	if(compare_gap > 0)
	    stats_a->inc(true, compare_gap, COMPARE_STAT, EP_METHOD);
	else
	    stats_a->inc(false, compare_gap, COMPARE_STAT, EP_METHOD);
	
	compare_gap = stats_b->load() - player_b->getRealPieceEvaluation(player_b);
	if(compare_gap > 0)
	    stats_b->inc(true, compare_gap, COMPARE_STAT, EP_METHOD);
	else
	    stats_b->inc(false, compare_gap, COMPARE_STAT, EP_METHOD);
	
	compare_gap = stats_c->load() - player_c->getRealPieceEvaluation(player_c);
	if(compare_gap > 0)
	    stats_c->inc(true, compare_gap, COMPARE_STAT, EP_METHOD);
	else
	    stats_c->inc(false, compare_gap, COMPARE_STAT, EP_METHOD);
    }
}