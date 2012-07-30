#include <iostream>
#include "player.h"
#include "cake.h"
int main(int argc, char **argv) 
{
    Player player_a, player_b;
    Cake *cake = new Cake();
    
    //building cake
    cake->build_cake();
    
    cake->print_sectors();
    
    //building players' evaluations
    
    player_a.build_evaluation_map();
    player_b.build_evaluation_map();
    
    //evaluation
    player_a.calculate_total_evaluation(cake);
    
    player_a.print_total_evaluation();
    
    //cut
    player_a.cut(cake);
    
    //choose
    player_b.choose(cake);
    
    //print
    
    
    
    return 0;
}
