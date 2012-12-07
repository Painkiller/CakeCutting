#include <iostream>

#include "player.h"
#include "referee.h"
#include "cake.h"
#include "stats.h"

void cutAndChoose(Player *player_a, Player *player_b, Stats *stats_a, Stats *stats_b);

void surplusProcedure(Player *player_a, Player *player_b, Referee *referee, Stats *stats_a, Stats *stats_b);
void equitabilityProcedure(Player *player_a, Player *player_b, Player *player_c, Referee *referee, Stats *stats_a, Stats *stats_b, Stats *stats_c);


int main(int argc, char **argv) 
{    
    srand(time(0));
    util::enableLog(false);
    
    int problem_id;
    ProblemType problem;
    bool skip = false;
    int iter = 1000000;
    Cake *cake = new Cake();
   
    Player *player_a = new Player("A", cake, CHEATER);
    Player *player_b = new Player("B", cake, HONEST);
    Player *player_c = new Player("C", cake, HONEST);

    
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
    for(int k = 0; k < 12; k++)
    {
	stats_a->reset_stats();
	stats_b->reset_stats();
	stats_c->reset_stats();
	for(int i = 0; i < iter; i++)
	{	
	    cake->buildCake(problem, n, m);
	    //building players' evaluations
	    if(isLogEnabled() || i == 1)
		cake->printSectors();
	    player_a->buildEvaluationMap(problem, ADDMAX_SUBMIN);
	    player_b->buildEvaluationMap(problem, ADDMAX_SUBMIN);
	    
	    referee->assignPlayer(player_a);
	    referee->assignPlayer(player_b);
	    
	    //***Cut & Choose Method***
	    if(isLogEnabled())
	    {
		cout << endl;
		cout <<"*************CUT AND CHOOSE************" << endl;
		cout << endl;
	    }
	    cutAndChoose(player_a, player_b, stats_a, stats_b);
	    
	    //Clear previous cuts
	    cake->clear_cuts();
	    if(isLogEnabled())
	    {
		cout << endl;
		cout <<"*************SURPLUS PROCEDURE************" << endl;
		cout << endl;
	    }
	    //***Surplus Procedure**
	    surplusProcedure(player_a, player_b, referee, stats_a, stats_b);
	    
	    cake->clear_cuts();
	    referee->clear_pieces();
	    
	    if(isLogEnabled())
	    {
		cout << endl;
		cout <<"*************EQUITABILITY PROCEDURE************" << endl;
		cout << endl;
	    }
	    
	    player_c->buildEvaluationMap(problem, ADDMAX_SUBMIN);
	    referee->assignPlayer(player_c);
		    
//     	equitabilityProcedure(player_a, player_b, player_c, referee, stats_a, stats_b, stats_c);
	    
	    cake->clear_cuts();
	    referee->clear_pieces();
	    referee->clear_result_map();
	    player_a->clear_maps();
	    player_b->clear_maps();
	    player_c->clear_maps();
	    
	    referee->unassign_players();
	    if(i % 100000 == 0)
	    cout << i << endl;
	}
    cout <<"Sectors " << n << " Type " << m << ":"<<endl;
    stats_a->print_all_stats(iter);
    stats_b->print_all_stats(iter);
    stats_c->print_all_stats(iter);
//     n+=10;
    m++;
    }
    return 0;
}

void cutAndChoose(Player *player_a, Player *player_b, Stats *stats_a, Stats *stats_b)
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
    
    float gap = player_a->getRealPieceEvaluation() - player_a->getPieceEvaluation();
    if(gap > 0)
	stats_a->cc_inc(true, gap);
    else
	stats_a->cc_inc(false, gap);
    
    gap = player_b->getRealPieceEvaluation() - player_b->getPieceEvaluation();
    if(gap > 0)
	stats_b->cc_inc(true, gap);
    else
	stats_b->cc_inc(false, gap);
}

void surplusProcedure(Player *player_a, Player *player_b, Referee *referee, Stats *stats_a, Stats *stats_b)
{
    player_a->calculateTotalEvaluation();
    player_b->calculateTotalEvaluation();
    
    player_a->cut();
    player_b->cut();
    
    referee->handleHalfpoints();
    
    player_a->printPieceInfo();
    player_b->printPieceInfo();
    
    referee->handleMiddle();
    
    float gap = player_a->getRealPieceEvaluation() - player_a->getPieceEvaluation();
    if(gap > 0)
	stats_a->sp_inc(true, gap);
    else
	stats_a->sp_inc(false, gap);
    
    gap = player_b->getRealPieceEvaluation() - player_b->getPieceEvaluation();
    if(gap > 0)
	stats_b->sp_inc(true, gap);
    else
	stats_b->sp_inc(false, gap);
}

void equitabilityProcedure(Player *player_a, Player *player_b, Player *player_c, Referee *referee, Stats *stats_a, Stats *stats_b, Stats *stats_c)
{
    referee->handleEquitability();
   
    float gap = player_a->getRealPieceEvaluation() - player_a->getPieceEvaluation();
    if(gap > 0)
	stats_a->ep_inc(true, gap);
    else
	stats_a->ep_inc(false, gap);
    
    gap = player_b->getRealPieceEvaluation() - player_b->getPieceEvaluation();
    if(gap > 0)
	stats_b->ep_inc(true, gap);
    else
	stats_b->ep_inc(false, gap);
        
    gap = player_c->getRealPieceEvaluation() - player_c->getPieceEvaluation();
    if(gap > 0)
	stats_c->ep_inc(true, gap);
    else
	stats_c->ep_inc(false, gap);

}