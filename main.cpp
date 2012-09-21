#include <iostream>

#include "player.h"
#include "referee.h"
#include "cake.h"
#include "stats.h"

void cutAndChoose(Player *player_a, Player *player_b, Stats *stats);

void surplusProcedure(Player *player_a, Player *player_b, Referee *referee, Stats *stats);
void equitabilityProcedure(Player *player_a, Player *player_b, Player *player_c, Referee *referee, Stats *stats);


int main(int argc, char **argv) 
{    
    srand(time(0));
    util::enableLog(false);
    
    int problem;
    bool skip = false;
    int iter = 1000;
    Cake *cake = new Cake();
   
    Player *player_a = new Player("A", cake, CHEATER);
    Player *player_b = new Player("B", cake, HONEST);
    Player *player_c = new Player("C", cake, HONEST);

    
    Referee *referee = new Referee(cake);
    
    Stats *stats = new Stats();
    while(!skip)
    {
	cout <<"Select initialization of the problem: (0: Random, 1: Bosnia)" << endl;
	cin >> problem;
    
	switch(problem)
	{	
	  case RANDOM:
	  {
	      skip = true;
	      break;
	  }
	  case BOSNIA:
	  {
	      skip = true;
	      break;
	  }
	  default:
	    break;
	}
    }
    //building cake
    cake->buildCake(problem);
    
    cake->printSectors();
    

    
    for(int i = 0; i < iter; i++)
    {

	//building players' evaluations
	
	player_a->buildEvaluationMap(problem);
	player_b->buildEvaluationMap(problem);
	
	referee->assignPlayer(player_a);
	referee->assignPlayer(player_b);
	
	//***Cut & Choose Method***
	if(isLogEnabled())
	{
	    cout << endl;
	    cout <<"*************CUT AND CHOOSE************" << endl;
	    cout << endl;
	}
// 	cutAndChoose(player_a, player_b, stats);
	
	//Clear previous cuts
	cake->clear_cuts();
	if(isLogEnabled())
	{
	    cout << endl;
	    cout <<"*************SURPLUS PROCEDURE************" << endl;
	    cout << endl;
	}
	//***Surplus Procedure**
// 	surplusProcedure(player_a, player_b, referee, stats);
	
	cake->clear_cuts();
	referee->clear_pieces();
	
	if(isLogEnabled())
	{
	    cout << endl;
	    cout <<"*************EQUITABILITY PROCEDURE************" << endl;
	    cout << endl;
	}
	
	player_c->buildEvaluationMap(problem);
	referee->assignPlayer(player_c);
		
	equitabilityProcedure(player_a, player_b, player_c, referee, stats);
	
	cake->clear_cuts();
	referee->clear_pieces();
	referee->clear_result_map();
	player_a->clear_maps();
	player_b->clear_maps();
	player_c->clear_maps();
	
	referee->unassign_players();
	if(i % 100 == 0)
	cout << i << endl;
    }
    
    stats->print_cc_stats(iter);
    stats->print_sp_stats(iter);
    stats->print_ep_stats(iter);
    
    return 0;
}

void cutAndChoose(Player *player_a, Player *player_b, Stats *stats)
{ 
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
    if(player_a->getPieceEvaluation() < player_a->getRealPieceEvaluation())
	stats->cc_inc();
}

void surplusProcedure(Player *player_a, Player *player_b, Referee *referee, Stats *stats)
{
    player_a->calculateTotalEvaluation();
    player_b->calculateTotalEvaluation();
    
    player_a->cut();
    player_b->cut();
    
    referee->handleHalfpoints();
    
    player_a->printPieceInfo();
    player_b->printPieceInfo();
    
    referee->handleMiddle();
    
    if(player_a->getPieceEvaluation() < player_a->getRealPieceEvaluation())
	stats->sp_inc();
}

void equitabilityProcedure(Player *player_a, Player *player_b, Player *player_c, Referee *referee, Stats *stats)
{
    referee->handleEquitability();
    if(player_a->getPieceEvaluation() < player_a->getRealPieceEvaluation())
	stats->ep_inc();
}