#ifndef SHARED_H
#define SHARED_H

#define N_SECTOR_TYPE 4
#define N_SECTORS 10
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
#endif // SHARED_H
