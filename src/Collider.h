#ifndef COLLIDER_H
#define COLLIDER_H
#include <vector>
#include <iterator>
#include "Sprite.h"

class Collider{
    static constexpr float PI = 3.14159265f;
    static std::vector<Collider> colliderList;
    float x,y;
    float width,height;
    float speed;
    float directionX, directionY; //normalized vector, values between 0 and 1
    int img_id;//purely for tests before switching to server-client architecture
    Collider(float x,float y,float width,float height);
    inline void UpdatePos(void);
    inline void setRandomDirection(void);
    inline void changeDirectionX(void);
    inline void changeDirectionY(void);
    inline void setPosZero(void);

    public:
    static std::string parseAll(void);
    static void Initialize(void);
    static bool Collision();
    static void setDirectionAt(float x, float y,int i);
    static void setRandomDirectionAt(int i);
    
};
#endif