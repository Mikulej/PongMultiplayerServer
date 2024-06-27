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
    auto player1 = colliderList.begin() + 1;
    auto player2 = colliderList.begin() + 2;
    //const float ballStepX = ball->speed*ball->directionY*deltaTime;
    //check if collides with window border
    if(ball->y + (ball->height/2.0f) + (ball->speed*ball->directionY*deltaTime)>= 1.0){   
        ball->y = 1.0 - (ball->height/2.0f);  
        ball->changeDirectionY();  
    }
    else if(ball->y - (ball->height/2.0f) + (ball->speed*ball->directionY*deltaTime)<= -1.0){
        ball->y = -1.0 + (ball->height/2.0f);
        ball->changeDirectionY();  
    }
    else if(ball->x + (ball->width/2.0f) + (ball->speed*ball->directionX*deltaTime)>= 1.0){   
        ball->x = 1.0 - (ball->width/2.0f);
        ball->changeDirectionX();   
    }
    else if(ball->x - (ball->width/2.0f) + (ball->speed*ball->directionX*deltaTime)<= -1.0){
        ball->x = -1.0 + (ball->width/2.0f);
        ball->changeDirectionX(); 
    }
    else if(
    (
        (ball->y - (ball->height/2.0f) + (ball->speed*ball->directionY*deltaTime) <= player1->y + (player1->height/2.0f) + (player1->speed*player1->directionY*deltaTime)) &&
        (ball->y - (ball->height/2.0f) + (ball->speed*ball->directionY*deltaTime) >= player1->y - (player1->height/2.0f) + (player1->speed*player1->directionY*deltaTime))
    )
    ||
    (
        (ball->y + (ball->height/2.0f) + (ball->speed*ball->directionY*deltaTime) <= player1->y + (player1->height/2.0f) + (player1->speed*player1->directionY*deltaTime)) &&
        (ball->y + (ball->height/2.0f) + (ball->speed*ball->directionY*deltaTime) >= player1->y - (player1->height/2.0f) + (player1->speed*player1->directionY*deltaTime))
    )  
    ){
        if(ball->x + (ball->width/2.0f) + (ball->speed*ball->directionX*deltaTime)>= player1->x - (player1->width/2.0f)){   
            ball->x = player1->x - (player1->width/2.0f) - (ball->width/2.0f);
            ball->changeDirectionX();   
        }
        ball->x += ball->speed*ball->directionX*deltaTime;
        ball->y += ball->speed*ball->directionY*deltaTime;
    }
    else{
        ball->x += ball->speed*ball->directionX*deltaTime;
        ball->y += ball->speed*ball->directionY*deltaTime;
    }
    
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
    
    
    if(player2->y + (player2->height/2.0f) + (player2->speed*player2->directionY*deltaTime)>= 1.0){   
        player2->y = 1.0 - (player2->height/2.0f);       
    }
    else if(player2->y - (player2->height/2.0f) + (player2->speed*player2->directionY*deltaTime)<= -1.0){
        player2->y = -1.0 + (player2->height/2.0f);
    }
    else{
        player2->y += player2->speed*player2->directionY*deltaTime;
    }
    ball->UpdatePos();
    player1->UpdatePos();
    player2->UpdatePos();
}
void Collider::UpdatePos(){
    Sprite::get(img_id).setPos(x,y);
}
void Collider::setRandomDirectionAt(int i){
    float rawX = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    if(rand() % 2){rawX*=-1;}
    float rawY = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    if(rand() % 2){rawY*=-1;}
    float norm = sqrt((rawX*rawX)+(rawY*rawY));
    colliderList.at(i).directionX = rawX / norm;
    colliderList.at(i).directionY = rawY / norm;
    //std::cout<< "X: " <<colliderList.at(i).directionX << "Y: " << colliderList.at(i).directionY << std::endl;
}
void Collider::changeDirectionX(){
    directionX *= -1;
}
void Collider::changeDirectionY(){
    directionY *= -1;
}
void Collider::setPosZero(){
    x=0; y=0;
}
void Collider::setDirectionAt(float x, float y,int i){ 
    colliderList.at(i).directionX = x;
    colliderList.at(i).directionY = y;
}
void Collider::Move(){
    Sprite::get(img_id).addPos(speed*directionX*deltaTime,speed*directionY*deltaTime);
}