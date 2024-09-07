#include "Collider.h"
using namespace std;
extern float deltaTime;
std::vector<Collider> Collider::colliderList;
Collider::Collider(float x,float y,float scaleX,float scaleY):x(x),y(y){
    img_id = Sprite::Add("box",x,y);
    Sprite::get(img_id).setScale(scaleX,scaleY);
    width = Sprite::get(img_id).getScaleX() / 16.0f;
    height = Sprite::get(img_id).getScaleY() / 9.0f;
    speed = 1.0f;
}
std::string Collider::parseAll(void){
    std::stringstream ss;
    for(const auto& c : colliderList){    
        ss << c.x << " " << c.y << " ";
    }
    return ss.str();
}
void Collider::Initialize(){
    //add all elements to colliderlist here
    //colliderlist[0] is ball [1] is player1 [2] is player2
    colliderList.push_back(Collider(0,0,0.2f,0.2f));
    colliderList.push_back(Collider(0.8f,0,0.5f,2.5f));
    colliderList.push_back(Collider(-0.8f,0,0.5f,2.5f));
}
void Collider::Collision(){
    auto ball = colliderList.begin();
    const float ballStepX = ball->speed*ball->directionX*deltaTime;
    const float ballStepY = ball->speed*ball->directionY*deltaTime;

    auto player1 = colliderList.begin() + 1;
    const float player1StepY = player1->speed*player1->directionY*deltaTime;

    auto player2 = colliderList.begin() + 2;
    const float player2StepY = player2->speed*player2->directionY*deltaTime;
    
    
    //check if collides with window border
    if(ball->y + (ball->height/2.0f) + (ballStepY)>= 1.0){   
        ball->y = 1.0 - (ball->height/2.0f);  
        ball->changeDirectionY();  
    }
    else if(ball->y - (ball->height/2.0f) + (ballStepY)<= -1.0){
        ball->y = -1.0 + (ball->height/2.0f);
        ball->changeDirectionY();  
    }
    if(ball->x + (ball->width/2.0f) + (ballStepX)>= 1.0){   
        ball->x = 1.0 - (ball->width/2.0f);
        ball->changeDirectionX();   
    }
    else if(ball->x - (ball->width/2.0f) + (ballStepX)<= -1.0){
        ball->x = -1.0 + (ball->width/2.0f);
        ball->changeDirectionX(); 
    }
    if(
    (
        (ball->y - (ball->height/2.0f) + (ballStepY) <= player1->y + (player1->height/2.0f) + (player1StepY)) &&
        (ball->y - (ball->height/2.0f) + (ballStepY) >= player1->y - (player1->height/2.0f) + (player1StepY))
    )
    ||
    (
        (ball->y + (ball->height/2.0f) + (ballStepY) <= player1->y + (player1->height/2.0f) + (player1StepY)) &&
        (ball->y + (ball->height/2.0f) + (ballStepY) >= player1->y - (player1->height/2.0f) + (player1StepY))
    )  
    ){
        if((ball->x + (ball->width/2.0f) < player1->x - (player1->width/2.0f))&& (ball->x + (ball->width/2.0f) + (ballStepX)>= player1->x - (player1->width/2.0f))){   
            ball->x = player1->x - (player1->width/2.0f) - (ball->width/2.0f);
            ball->changeDirectionX();   
        }
        
    }
    if(
    (
        (ball->y - (ball->height/2.0f) + (ballStepY) <= player2->y + (player2->height/2.0f) + (player2StepY)) &&
        (ball->y - (ball->height/2.0f) + (ballStepY) >= player2->y - (player2->height/2.0f) + (player2StepY))
    )
    ||
    (
        (ball->y + (ball->height/2.0f) + (ballStepY) <= player2->y + (player2->height/2.0f) + (player2StepY)) &&
        (ball->y + (ball->height/2.0f) + (ballStepY) >= player2->y - (player2->height/2.0f) + (player2StepY))
    )  
    ){
        if((ball->x - (ball->width/2.0f) > player2->x + (player2->width/2.0f))&&(ball->x - (ball->width/2.0f) + (ballStepX)<= player2->x + (player2->width/2.0f))){   
            ball->x = player2->x + (player2->width/2.0f) + (ball->width/2.0f);
            ball->changeDirectionX();   
        }
    }
    
    ball->x +=  ball->speed*ball->directionX*deltaTime;
    ball->y +=  ball->speed*ball->directionY*deltaTime;
   
    
    //check if collides with window border
    if(player1->y + (player1->height/2.0f) + (player1StepY)>= 1.0){   
        player1->y = 1.0 - (player1->height/2.0f);       
    }
    else if(player1->y - (player1->height/2.0f) + (player1StepY)<= -1.0){
        player1->y = -1.0 + (player1->height/2.0f);
    }
    else{
        player1->y += player1StepY;
    }
    
    if(player2->y + (player2->height/2.0f) + (player2StepY)>= 1.0){   
        player2->y = 1.0 - (player2->height/2.0f);       
    }
    else if(player2->y - (player2->height/2.0f) + (player2StepY)<= -1.0){
        player2->y = -1.0 + (player2->height/2.0f);
    }
    else{
        player2->y += player2StepY;
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
}
void Collider::changeDirectionX(){
    directionX *= -1;
    const float x = directionX;
    const float y = directionY;
    const float angle = PI * ((rand() % 31) - 15)/ 180.0f;
    const float rawX = (x * cos(angle)) - (y * sin(angle));
    const float rawY = (x * sin(angle)) + (y * cos(angle));
    const float norm = sqrt((rawX*rawX)+(rawY*rawY));
    directionX = rawX / norm;
    directionY = rawY / norm;
}
void Collider::changeDirectionY(){
    constexpr float cutOffAngle = 30.0f;
    directionY *= -1;
    if(directionY > 0){
        constexpr float cufOffLeft = 90.0f + cutOffAngle;
        constexpr float cufOffRight = 90.0f - cutOffAngle;
        const float oldAngle = atan2(directionY, directionX)*180.0f/PI;
        if(oldAngle < cufOffLeft && oldAngle >= 90.0f){//if angle of vector is close to stright Y-line change it to  45 +- 10
            const float newAngle = (PI * ((rand() % 21) - 10 + 45.0f + 90.0f)/ 180.0f) ;
            const float rawX = cos(newAngle);
            const float rawY = sin(newAngle);
            const float norm = sqrt((rawX*rawX)+(rawY*rawY));
            directionX = rawX / norm;
            directionY = rawY / norm;
            return;
        }
        else if(oldAngle > cufOffRight && oldAngle < 90.0f){
            const float newAngle = (PI * ((rand() % 21) - 10 - 45.0f + 90.0f)/ 180.0f) ;
            const float rawX = cos(newAngle);
            const float rawY = sin(newAngle);
            const float norm = sqrt((rawX*rawX)+(rawY*rawY));
            directionX = rawX / norm;
            directionY = rawY / norm;
            return;
        }
    }
    else{
        constexpr float cufOffLeft = 270.0f - cutOffAngle;
        constexpr float cufOffRight = 270.0f + cutOffAngle;
        float oldAngle = atan2(directionY, directionX);
        oldAngle = (2*PI + oldAngle) * 360 / (2*PI);

        if(oldAngle > cufOffLeft && oldAngle < 270.0f){//if angle of vector is close to stright Y-line change it to  45 +- 10
            const float newAngle = (PI * ((rand() % 21) - 10 - 45.0f + 270.0f)/ 180.0f) ;
            const float rawX = cos(newAngle);
            const float rawY = sin(newAngle);
            const float norm = sqrt((rawX*rawX)+(rawY*rawY));
            directionX = rawX / norm;
            directionY = rawY / norm;
            return;
        }
        else if(oldAngle < cufOffRight && oldAngle >= 270.0f){
            const float newAngle = (PI * ((rand() % 21) - 10 + 45.0f + 270.0f)/ 180.0f) ;
            const float rawX = cos(newAngle);
            const float rawY = sin(newAngle);
            const float norm = sqrt((rawX*rawX)+(rawY*rawY));
            directionX = rawX / norm;
            directionY = rawY / norm;
            return;
        }
    }
}
void Collider::setPosZero(){
    x=0; y=0;
}
void Collider::setDirectionAt(float x, float y,int i){ 
    colliderList.at(i).directionX = x;
    colliderList.at(i).directionY = y;
}