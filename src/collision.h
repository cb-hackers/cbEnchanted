#ifndef COLLISION_H
#define COLLISION_H
class CBObject;
struct Collision {
	Collision(CBObject *a,CBObject *b):a(a),b(b),angle(0),x(0),y(0){}
	Collision(CBObject *a,CBObject *b,float ang,float x,float y):a(a),b(b),angle(ang),x(x),y(x){}
	CBObject *a;
	CBObject *b;
	float angle;
	float x;
	float y;
};

#endif // COLLISION_H
