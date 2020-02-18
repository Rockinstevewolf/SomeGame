#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "player.hpp"

class Health{
private:
	float x, y;
	int x_add = 0;
	bool ZERO = false;
public:
	Image image;
	Texture texture;
	Sprite sprite;
	int hits = 0;
	int health = 3;

	Health(Player player){
		image.loadFromFile("sprites/heart.png");
        image.createMaskFromColor(Color(255,255,255));
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = player.X(); y = player.Y() - 8;
		sprite.setTextureRect(IntRect(0,0,32,8));
		sprite.setPosition(x,y);
	}

	bool update(Player player, bool HIT){ 
		if(HIT && !ZERO){
			if(hits<2 && health != 0){
				hits++;
				health--;
				x_add += 6;
				sprite.setTextureRect(IntRect(0,0,32 - hits*12,8));
			}
			else{
				health--;
				if(health < 0)
					health = 0;
				sprite.setTextureRect(IntRect(0,0,0,0));
				ZERO = true;
			}
		}
		if(health == 3)
			hits = 0;
		else if(health == 2)
			hits = 1;
		else if(health == 1)
			hits = 2;
		else if(health == 0)
			hits = 3;
		x = player.X() + x_add; y = player.Y() - 8;
		sprite.setPosition(x,y);
		if(ZERO)
			return true;
		else
			return false;
	}
}; 