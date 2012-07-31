#ifndef REFEREE_H
#define REFEREE_H
#include "entity.h"

class Referee : public Entity
{

public:
    Referee(Cake *cake);
    virtual ~Referee();
};

#endif // REFEREE_H
