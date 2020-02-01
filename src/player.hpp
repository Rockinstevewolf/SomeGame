#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

class Player{
private:
    float x, y;
public:
    float w, h;
    String File;
    Image image;
    Texture texture;
    Sprite sprite;

    Player(String F, float X, float Y, float W, float H){
        File = F;
        w = W; h = H;
        image.loadFromFile("sprites/" + File);
        //image.createMaskFromColor(Color(44,33,59));
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        x = X; y = Y;
        sprite.setTextureRect(IntRect(0,0,w,h));
    }

    void setPosition(float X, float Y){
        sprite.setPosition(X,Y);
    }
    void move(float dX, float dY){
        sprite.move(dX,dY);
        x += dX;
        y += dY;
    }
    void setTextureRect(pair <int,int>s){
        sprite.setTextureRect(IntRect(s.first,s.second,w,h));
    }
    float X(){
        return x;
    }
    float Y(){
        return y;
    }
};
