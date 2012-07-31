#include <iostream>

#include "player.h"
#include "referee.h"
#include "cake.h"

void cut_and_choose(Player *player_a, Player *player_b);

void surplus_procedure(Player *player_a, Player *player_b, Referee *referee);

int main(int argc, char **argv) 
{    
    Cake *cake = new Cake();
   
    Player *player_a = new Player("A", cake);
    Player *player_b = new Player("B", cake);
    
    Referee *referee = new Referee(cake);
    
    //building cake
    cake->build_cake();
    
    cake->print_sectors();
    
    //building players' evaluations
    
    player_a->build_evaluation_map();
    player_b->build_evaluation_map();
    
    
    //***Cut & Choose Method***
    
    cut_and_choose(player_a, player_b);
    
    //Clear previous cuts
    cake->clear_cuts();
    
    //***Surplus Procedure**
    
    
    return 0;
}

void cut_and_choose(Player *player_a, Player *player_b)
{ 
    //evaluation
    player_a->calculate_total_evaluation();
    
    player_a->print_total_evaluation();
    
    //cut
    player_a->cut();
    
    //choose
    player_b->choose();
}

void surplus_procedure(Player *player_a, Player *player_b, Referee *referee)
{
    
}