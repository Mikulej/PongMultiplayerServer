#include "Collider.h"
using namespace std;
extern float deltaTime;
std::vector<Collider> Collider::colliderList;
Collider::Collider(float x,float y,float scaleX,float scaleY):x(x),y(y){
    img_id = Sprite::Add("box",x,y);
    Sprite::get(img_id).setScale(scaleX,scaleY);
    width = Sprite::get(img_id).getScaleX() / 16.0f;
    height = Sprite::get(img_id).getScaleY() / 9.0f;
    speed = 0.5;
}
void Collider::Initialize(){
    //add all elements to colliderlist here
    //colliderlist[0] is ball [1] is player1 [2] is player2
    colliderList.push_back(Collider(0,0,0.2f,0.2f));
    colliderList.push_back(Collider(0.8f,0,0.5f,2.5f));
    colliderList.push_back(Collider(-0.8f,0,0.5f,2.5f));
}
bool Collider::Collision(){
    auto ball = colliderList.begin();
    ball->UpdatePos();

    auto player1 = colliderList.begin() + 1;
    //check if collides with window border
    if(player1->y + (player1->height/2.0f) + (player1->speed*player1->directionY*deltaTime)>= 1.0){   
        player1->y = 1.0 - (player1->height/2.0f);       
    }
    else if(player1->y - (player1->height/2.0f) + (player1->speed*player1->directionY*deltaTime)<= -1.0){
        player1->y = -1.0 + (player1->height/2.0f);
    }
    else{
        player1->y += player1->speed*player1->directionY*deltaTime;
    }
    player1->UpdatePos();
    auto player2 = colliderList.begin() + 2;
    if(player2->y + (player2->height/2.0f) + (player2->speed*player2->directionY*deltaTime)>= 1.0){   
        player2->y = 1.0 - (player2->height/2.0f);       
    }
    else if(player2->y - (player2->height/2.0f) + (player2->speed*player2->directionY*deltaTime)<= -1.0){
        player2->y = -1.0 + (player2->height/2.0f);
    }
    else{
        player2->y += player2->speed*player2->directionY*deltaTime;
    }
    player2->UpdatePos();
    // for (vector<Collider>::iterator i1 = colliderList.begin(), finish = colliderList.end(); i1 != finish; i1++){
    //     i1->UpdatePos();
    //     //check if collides with window border
    //         if(i1->y + (i1->height/2.0f) >= 1.0){
    //             std::cout << "Touching top border!" << std::endl;               
    //         }
    //         if(i1->y - (i1->height/2.0f) <= -1.0){
    //             std::cout << "Touching bottom border!" << std::endl;
    //         }
    //     for (vector<Collider>::iterator i2 = colliderList.begin(), finish = colliderList.end(); i2 != finish; i2++){
            
    //         //check collision between i1 and i2

    //         // // collision x-axis?
    //         // bool collisionX = (*i1).x + (*i1).width >= (*i2).x &&
    //         // (*i2).x + (*i2).width >= (*i1).x;
    //         // // collision y-axis?
    //         // bool collisionY = (*i1).y + (*i1).height >= (*i2).y &&
    //         // (*i2).y + (*i2).height >= (*i1).y;
    //         // // collision only if on both axes
    //         // return collisionX && collisionY;
    //     }
    // }
}
void Collider::UpdatePos(){
    Sprite::get(img_id).setPos(x,y);
}
void Collider::setDirectionAt(float x, float y,int i){
    colliderList.at(i).directionX = x;
    colliderList.at(i).directionY = y;
}
void Collider::Move(){
    Sprite::get(img_id).addPos(speed*directionX*deltaTime,speed*directionY*deltaTime);
}