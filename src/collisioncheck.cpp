#include "collisioncheck.h"
#include <cmath>
CollisionCheck::CollisionCheck()
{
}

bool CollisionCheck::testCollision(){

        switch(mCollisionType1){
                case Box:
                        switch(mCollisionType2){
                                case Box:
                                        float left1, right1, top1, bottom1;
                                        float left2, right2, top2, bottom2;

                                        left1 = mObject1->getX() - mObject1->getObjectSizeX()/2;
                                        right1 = mObject1->getX() + mObject1->getObjectSizeX()/2;
                                        top1 = mObject1->getY() + mObject1->getObjectSizeY()/2;
                                        bottom1 = mObject1->getY() - mObject1->getObjectSizeY()/2;

                                        left2 = mObject2->getX() - mObject2->getObjectSizeX()/2;
                                        right2 = mObject2->getX() + mObject2->getObjectSizeX()/2;
                                        top2 = mObject2->getY() + mObject2->getObjectSizeY()/2;
                                        bottom2 = mObject2->getY() - mObject2->getObjectSizeY()/2;

                                        if(bottom1 < bottom2) return
                                                        false;
                                        if(top1 > top2) return
                                                        false;
                                        if(left1 < left2) return
                                                        false;
                                        if(right1 > right2)
                                                return false;
                                        return true;
                                break;
                                case Circle:

                                        float circleRad;
                                        float circleDistX;
                                        float circleDistY;
                                        float cornerDist;
                                        circleRad = 0.5*(mObject2->getObjectSizeX() + mObject2->getObjectSizeY());
                                        circleDistX = abs(mObject2->getX() - mObject1->getX()-mObject1->getObjectSizeX()/2);
                                        circleDistY = abs(mObject2->getY() - mObject1->getY()-mObject1->getObjectSizeY()/2);

                                        if (circleDistX > (mObject1->getObjectSizeX()/2 + circleRad))
                                                return false;
                                        if (circleDistY > (mObject1->getObjectSizeY()/2 + circleRad))
                                                return false;

                                        if (circleDistX <= (mObject1->getObjectSizeX()/2))
                                                return true;
                                        if (circleDistY <= (mObject1->getObjectSizeY()/2))
                                                return true;

                                        cornerDist = ((circleDistX - mObject1->getObjectSizeX()/2)*(circleDistX - mObject1->getObjectSizeX()/2)+(circleDistY - mObject1->getObjectSizeY()/2)*(circleDistY - mObject1->getObjectSizeY()/2));
                                        return (cornerDist <= circleRad*circleRad);

                                break;
                                case Map:

                                break;
                                case Pixel:

                                break;
                        }
                break;
                case Circle:
                        switch(mCollisionType2){
                                case Box:

                                break;
                                case Circle:

                                break;
                                case Map:

                                break;
                                case Pixel:

                                break;
                        }
                break;
                case Pixel:
                        switch(mCollisionType2){
                                case Box:

                                break;
                                case Circle:

                                break;
                                case Map:

                                break;
                                case Pixel:

                                break;
                        }
                break;
        }

}

void CollisionCheck::handleCollision(){

}
