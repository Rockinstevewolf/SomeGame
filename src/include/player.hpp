#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "map.hpp"

using namespace sf;


class Player{
private:
    float x, y;
    bool ENEMYCOL;
public:
    float w, h;
    float dx, dy, speed = 0;
    int dir = 0;
    String File;
    Image image;
    Texture texture;
    Sprite sprite;

    Player(String F, float X, float Y, float W, float H){
        File = F;
        w = W; h = H;
        image.loadFromFile("sprites/" + File);
        image.createMaskFromColor(Color(41,33,59));
        image.createMaskFromColor(Color(255,255,255));
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        x = X; y = Y;
        //sprite.setTextureRect(IntRect(27,5,40,90));
        sprite.setTextureRect(IntRect(0,0,w,h));
    }

    void update(float time, Player enemy){
        switch(dir){
            case 0: dx = speed; dy = 0; break;
            case 1: dx = -speed; dy = 0; break;
            case 2: dx = 0; dy = speed; break;
            case 3: dx = 0; dy = -speed; break;
        }
        x += dx*time;
        y += dy*time;

        speed = 0;
        sprite.setPosition(x,y);
        interactionMap();
        interactionHero(time, enemy);
    }

    void setPosition(float X, float Y){
        x = X;
        y = Y;
        sprite.setPosition(X,Y);
    }
    void move(float dX, float dY){
        sprite.move(dX,dY);
        x += dX;
        y += dY;
    }
    void setTextureRect(Spr_data s){
        sprite.setTextureRect(IntRect(s.x0,s.y0,s.w,s.h));
        //sprite.setTextureRect(IntRect(s.first,s.second,w,h));
    }
    void setTextureRect(std::pair<int,int> s){
        //sprite.setTextureRect(IntRect(s.x0,s.y0,s.w,s.h));
        sprite.setTextureRect(IntRect(s.first,s.second,w,h));
    }
    float X(){
        return x;
    }
    float Y(){
        return y;
    }

    void interactionMap(){
        for(int j = y/32; j < (y + w)/32; j++){
            string get = m[j];
            for(int i = x/32; i < (x + h)/32; i++){
                if(get[i] == '0' || get[i] == 's'){
                    if(!ENEMYCOL){
                        if(dy > 0)
                            y = j*32 - h;
                        if(dy < 0)
                            y = j*32 + 32;
                        if(dx > 0)
                            x = i*32 - w;
                        if(dx < 0)
                            x = i*32 + 32;
                    }

                }
            }
        }
    }
    void interactionHero(float time, Player enemy){
        if(sprite.getGlobalBounds().intersects(enemy.sprite.getGlobalBounds())){
            //dx = -dx;
            //dy = -dy;
            ENEMYCOL = true;
            if(dy>0)
                y = y - dy*time;// - 5;
            if(dy<0)
                y = y - dy*time;// + 5;
            if(dx>0)
                x = x - dx*time;// - 5;
            if(dx<0)
                x = x - dx*time;// + 5;
        }
        else{
            ENEMYCOL = false;
            //dx = dx;
            //dy = dy;
        }
    }

    void interactionStructMap(){
        for(int j = y/32; j < (y + 90)/32; j++){
            string get = m[j];
            for(int i = x/32; i < (x + 40)/32; i++){
                if(get[i] == '0' || get[i] == 's'){
                    if(dy > 0)
                        y = j*32 - 90;
                    if(dy < 0)
                        y = j*32 + 32;
                }
            }
        }
        for(int j = y/32; j < (y + 60)/32; j++){
            string get = m[j];
            for(int i = x/32; i < (x + 90)/32; i++){
                if(get[i] == '0' || get[i] == 's'){
                    if(dx > 0)
                        x = i*32 - 90;
                    if(dx < 0)
                        x = i*32 + 32;
                }
            }
        }    
    }
};
