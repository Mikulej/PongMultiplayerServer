#ifndef COLLIDER_H
#define COLLIDER_H
#include <vector>
#include <iterator>
#include "Sprite.h"

class Collider{
    static std::vector<Collider> colliderList;
    float x,y;
    float width,height;
    int img_id;//purely for tests before switching to server-client architecture
    Collider(float x,float y,float width,float height);
    void Move(float x,float y);
    bool Collision();
    public:
    static void Initialize(void);
};
#endif