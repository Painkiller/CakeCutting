#include <iostream>
#include "player.h"
#include "cake.h"
int main(int argc, char **argv) 
{    
    Cake *cake = new Cake();
   
    Player *player_a = new Player("A", cake);
    Player *player_b = new Player("B", cake);
    
    //building cake
    cake->build_cake();
    
    cake->print_sectors();
    
    //building players' evaluations
    
    player_a->build_evaluation_map();
    player_b->build_evaluation_map();
    
    //evaluation
    player_a->calculate_total_evaluation();
    
    player_a->print_total_evaluation();
    
    //cut
    player_a->cut();
    
    //choose
    player_b->choose();
    
    //print
    
    
    return 0;
}
