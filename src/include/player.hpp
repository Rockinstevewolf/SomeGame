#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "map.hpp"

using namespace sf;


class Player{
private:
    float x, y;
    bool ENEMYCOL;
    int sprx, spry;
public:
    float w, h;
    float dx, dy, speed = 0;
    int dir = 2; int sprnum = 0;
    String File;
    Image image;
    Texture texture;
    Sprite sprite;

    Player(String F, float X, float Y, float W, float H){
        File = F;
        w = W; h = H;
        image.loadFromFile("sprites/" + File);
        //image.createMaskFromColor(Color(41,33,59));
        //image.createMaskFromColor(Color(255,255,255));
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        x = X; y = Y;
        sprite.setPosition(x,y);
        setTextureRect(sprnum, dir);
    }

    void update(float time, Player enemy){
        switch(dir){
            case 0: dx = 0; dy = -speed; break;
            case 1: dx = -speed; dy = 0; break;
            case 2: dx = 0; dy = speed; break;
            case 3: dx = speed; dy = 0; break;
        }
        x += dx*time;
        y += dy*time;

        speed = 0;
        sprite.setPosition(x,y);
        /*if(y > RES_HEIGHT-h-32 || x > RES_WIDTH-w-32 || x < 32 || y < 32)
            sprite.setPosition(32,32);*/
        setTextureRect(sprnum, dir);
        interactionMap();
        interactionHero(time, enemy);
    }

    void setPosition(float X, float Y){
        x = X;
        y = Y;
        sprite.setPosition(x,y);
    }
    void move(float dX, float dY){
        x += dX;
        y += dY;
        sprite.move(dX,dY);
    }
    void setTextureRect(int sn, int d){
        sprnum = sn;
        dir = d;
        sprx = sprnum * 32;
        spry = dir * 32;
        sprite.setTextureRect(IntRect(sprx,spry,w,h));
    }
    void setTextureRect(int sn){
        sprnum = sn;
        sprx = sprnum * 32;
        spry = dir * 32;
        sprite.setTextureRect(IntRect(sprx,spry,w,h));
    }
    void setTextureRect(std::pair<int,int> s){
        sprite.setTextureRect(IntRect(s.first,s.second,w,h));
    }
    float X(){
        return x;
    }
    float Y(){
        return y;
    }
    void changeSprite(){

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
            ENEMYCOL = true;
            if(dy>0)
                y = y - dy*time;
            if(dy<0)
                y = y - dy*time;
            if(dx>0)
                x = x - dx*time;
            if(dx<0)
                x = x - dx*time;
        }
        else{
            ENEMYCOL = false;
        }
    }
};
