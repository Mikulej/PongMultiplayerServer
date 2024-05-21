#include "Collider.h"
using namespace std;
std::vector<Collider> Collider::colliderList;
Collider::Collider(float x,float y,float width,float height):x(x),y(y),width(width),height(height){
    img_id = Sprite::Add("box",x,y);
    Sprite::get(img_id).setScale(width,height);
}
void Collider::Initialize(){
    //add all elements to colliderlist here
    //colliderlist[0] is ball [1] is player1 [2] is player2
    colliderList.push_back(Collider(0,0,0.2f,0.2f));
    colliderList.push_back(Collider(0.8f,0,0.5f,2.5f));
    colliderList.push_back(Collider(-0.8f,0,0.5f,2.5f));
}
bool Collider::Collision(){
    for (vector<Collider>::iterator i1 = colliderList.begin(), finish = colliderList.end(); i1 != finish; i1++){
        for (vector<Collider>::iterator i2 = colliderList.begin(), finish = colliderList.end(); i2 != finish; i2++){
            //check collision between i1 and i2

            // // collision x-axis?
            // bool collisionX = (*i1).x + (*i1).width >= (*i2).x &&
            // (*i2).x + (*i2).width >= (*i1).x;
            // // collision y-axis?
            // bool collisionY = (*i1).y + (*i1).height >= (*i2).y &&
            // (*i2).y + (*i2).height >= (*i1).y;
            // // collision only if on both axes
            // return collisionX && collisionY;
        }
    }
}