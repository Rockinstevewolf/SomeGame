#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "map.hpp"
#include "player.hpp"

using namespace sf;

class Bullet{
private:
	float x,y;
public:
	float w, h;
	float dx, dy, speed = 0.8;
	int dir = 0;
	String File;
	Image image;
	Texture texture;
	Sprite sprite;
	bool ENEMYHIT = false;
	bool MAPHIT = false;
	bool SHOOT = false;

	//Bullet();

	Bullet(String F, float X, float Y, float W, float H){
		File = F;
		w = W; h = H;
		image.loadFromFile("sprites/" + File);
        image.createMaskFromColor(Color(255,255,255));
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X; y = Y;
		sprite.setTextureRect(IntRect(0,0,w,h));
	}

	void setShot(int d, int X, int Y){
		dir = d;
		if(d == 0)
		    sprite.setPosition(X + 9, Y - 16);
		else if(d == 2)
		    sprite.setPosition(X + 9, Y + 32);
		else if(d == 1)
		    sprite.setPosition(X - 16, Y + 9);
		else if(d == 3)
		    sprite.setPosition(X + 32, Y + 9);
		SHOOT = true;
	}

	void update(float time, Player enemy){
	    switch(dir){
            case 0: dx = 0; dy = -speed; break;
            case 1: dx = -speed; dy = 0; break;
            case 2: dx = 0; dy = speed; break;
            case 3: dx = speed; dy = 0; break;
	    }
	    if((MAPHIT || ENEMYHIT)){
	    	SHOOT = false;
	    	MAPHIT = false;
	    	ENEMYHIT = false;
	    }
		else if(SHOOT){
			x += dx*time;
			y += dy*time;
			sprite.setPosition(x,y);
			interactionMap();
			interactionHero(time, enemy);
		}
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
	void setTextureRect(std::pair<int,int> s){
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
	                if(!ENEMYHIT){
	                	if(dy > 0)
	                	    y = j*32 - h;
	                	if(dy < 0)
	                	    y = j*32 + 32;
	                	if(dx > 0)
	                	    x = i*32 - w;
	                	if(dx < 0)
	                	    x = i*32 + 32;
	                	MAPHIT = true;
	                }
	                else
	                	MAPHIT = false;
	            }
	        }
	    }
	}
	void interactionHero(float time, Player enemy){
	    if(sprite.getGlobalBounds().intersects(enemy.sprite.getGlobalBounds())){
	        ENEMYHIT = true;
	        if(dy>0)
	            y = y - dy*time;
	        if(dy<0)
	            y = y - dy*time;
	        if(dx>0)
	            x = x - dx*time;
	        if(dx<0)
	            x = x - dx*time;
	    }
	    else
	    	ENEMYHIT = false;
	}
};