#ifndef SHARED_H
#define SHARED_H

#define N_SECTOR_TYPE 4
#define N_BOSNIA_SECTORS 104
#define MIN_ERR 0.0000001
#define MAX_ERR 0.01
#define INF RAND_MAX

enum EntityType
{
    PLAYER,
    REFEREE
};
enum Behaviour
{
    CHEATER,
    HONEST
};
enum ProblemType
{
    RANDOM = 0,
    BOSNIA = 1
};
enum PieceType
{
    ANY = 0,
    BEST = 1
};
enum StrategyType
{
    NONE_STRATEGY,
    SUBMAX_ADDMIN,
    ADDMAX_SUBMIN,
    SWAP,
    FLAT
};
#endif // SHARED_H
