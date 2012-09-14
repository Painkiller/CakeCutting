#include <iostream>

#include "player.h"
#include "referee.h"
#include "cake.h"

void cutAndChoose(Player *player_a, Player *player_b);

void surplusProcedure(Player *player_a, Player *player_b, Referee *referee);
void equitabilityProcedure(Player *player_a, Player *player_b, Player *player_c, Referee *referee);


int main(int argc, char **argv) 
{    
    srand(time(0));

    Cake *cake = new Cake();
   
    Player *player_a = new Player("A", cake, CHEATER);
    Player *player_b = new Player("B", cake, HONEST);
    
    Referee *referee = new Referee(cake);
    
    //building cake
    cake->buildCake();
    
    cake->printSectors();
    
    //building players' evaluations
    
    player_a->buildEvaluationMap();
    player_b->buildEvaluationMap();
    
    
    //***Cut & Choose Method***
    cout << endl;
    cout <<"*************CUT AND CHOOSE************" << endl;
    cout << endl;
    cutAndChoose(player_a, player_b);
    
    //Clear previous cuts
    cake->clear_cuts();
    cout << endl;
    cout <<"*************SURPLUS PROCEDURE************" << endl;
    cout << endl;
    //***Surplus Procedure**
    surplusProcedure(player_a, player_b, referee);
    
    cake->clear_cuts();
    referee->clear_pieces();
    
    cout << endl;
    cout <<"*************EQUITABILITY PROCEDURE************" << endl;
    cout << endl;
    
    Player *player_c = new Player("C", cake, HONEST);
    player_c->buildEvaluationMap();
    
    equitabilityProcedure(player_a, player_b, player_c, referee);
    
    return 0;
}

void cutAndChoose(Player *player_a, Player *player_b)
{ 
    //evaluation
    player_a->calculateTotalEvaluation();
    
    player_a->printTotalEvaluation();
    
    //cut
    player_a->cut();
    
    //choose
    player_b->choose();
    player_a->take();
    cout << endl;
    player_a->printRealEvaluation();
}

void surplusProcedure(Player *player_a, Player *player_b, Referee *referee)
{
    player_a->calculateTotalEvaluation();
    player_b->calculateTotalEvaluation();
    
    player_a->cut();
    player_b->cut();
    
    referee->assignPlayer(player_a);
    referee->assignPlayer(player_b);
    
    referee->handleHalfpoints();
    
    player_a->printPieceInfo();
    player_b->printPieceInfo();
    
    referee->handleMiddle();
    
    player_a->printRealEvaluation();

}

void equitabilityProcedure(Player *player_a, Player *player_b, Player *player_c, Referee *referee)
{
    referee->assignPlayer(player_c);
    referee->handleEquitability();
    player_a->printRealEvaluation();
}