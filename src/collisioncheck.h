#ifndef COLLISIONCHECK_H
#define COLLISIONCHECK_H
#include "precomp.h"
#include "cbobject.h"

class CollisionCheck
{
public:
    CollisionCheck();
    void setObjects(CBObject *a,CBObject *b);
};

#endif // COLLISIONCHECK_H
