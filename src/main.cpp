#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>

using namespace std;
using namespace sf;

int w = 96, h = 96;
float speed = 0.3;

struct Position{
    int x;
    int y;
}pos = {50,25};

pair <int,int>l1(0,96);
pair <int,int>l2(96,96);
pair <int,int>l3(192,96);

pair <int,int>r1(0,192);
pair <int,int>r2(96,192);
pair <int,int>r3(192,192);

pair <int,int>u1(0,288);
pair <int,int>u2(96,288);
pair <int,int>u3(192,288);

pair <int,int>d1(0,0);
pair <int,int>d2(96,0);
pair <int,int>d3(192,0);



int main()
{
    pair <int,int>s;
    RenderWindow window(sf::VideoMode(640, 480), "Game"/*, Style::Fullscreen*/);
    cout << "let's go..." << endl;

    Image heroimage;
    heroimage.loadFromFile("sprites/hero.png");

    Texture herotexture;
    herotexture.loadFromImage(heroimage);

    Sprite herosprite;

    s = r1;
    herosprite.setTexture(herotexture);
    herosprite.setTextureRect(IntRect(s.first,s.second,w,h));
    herosprite.setPosition(pos.x, pos.y);

    Clock clock;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time/800;
        //cout << time << endl;

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        if(Keyboard::isKeyPressed(Keyboard::LControl) && Keyboard::isKeyPressed(Keyboard::C))
        {
            window.close();
        }

        if(Keyboard::isKeyPressed(Keyboard::Left)){
            if(s == l1)         s = l2;
            else if(s == l2)    s = l3;
            else if(s == l3)    s = l1;
            else                s = l1;

            herosprite.setTextureRect(IntRect(s.first,s.second,w,h));
            herosprite.move(-speed*time,0);
            pos.x -= speed*time;
        }

        if(Keyboard::isKeyPressed(Keyboard::Right)){
            if(s == r1)         s = r2;
            else if(s == r2)    s = r3;
            else if(s == r3)    s = r1;
            else                s = r1;

            herosprite.setTextureRect(IntRect(s.first,s.second,w,h));
            herosprite.move(speed*time,0);
            pos.x += speed*time;
        }

        if(Keyboard::isKeyPressed(Keyboard::Up)){
            if(s == u1)         s = u2;
            else if(s == u2)    s = u3;
            else if(s == u3)    s = u1;
            else                s = u1;

            herosprite.setTextureRect(IntRect(s.first,s.second,w,h));
            herosprite.move(0,-speed*time);
            pos.y += speed*time;
        }

        if(Keyboard::isKeyPressed(Keyboard::Down)){
            if(s == d1)         s = d2;
            else if(s == d2)    s = d3;
            else if(s == d3)    s = d1;
            else                s = d1;

            herosprite.setTextureRect(IntRect(s.first,s.second,w,h));
            herosprite.move(0,speed*time);
            pos.y -= speed;
        }
        window.clear();
        window.draw(herosprite);
        window.display();
    }
    return 0;
}
