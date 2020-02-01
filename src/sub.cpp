#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>

using namespace std;
using namespace sf;

#include "map.hpp"
#include "config.hpp"
#include "player.hpp"
#include "structures.hpp"
#include "connection.hpp"

PP_data my_pos = {400,300,emp};
PP_data *enemy_pos;

string push_port = "8080"; //tcp://localhost:
string pull_port = "8081"; //tcp://*:


int main()
{
    Connection conn(push_port, pull_port);

    Thread_data my_data;
    my_data.pos = my_pos;

    Thread_data enemy_data;

    conn.pushPos(my_data.pos);

    enemy_data.pos = conn.pullPos();

    my_data.flags.WORK = true;
    conn.flags.WORK = true;

    conn.pushPosThreaded(&my_data);
    conn.pullPosThreaded(&enemy_data);

    RenderWindow window(sf::VideoMode(RES_WIDTH, RES_HEIGHT), "Game_sub"/*, Style::Fullscreen*/);
    cout << "let's go..." << endl;
    
    pair <int,int>s;

    Image map_image;
    map_image.loadFromFile("sprites/map.png");
    Texture map;
    map.loadFromImage(map_image);
    Sprite s_map;
    s_map.setTexture(map);

    Player player("hero3.png",0,0,w,h);
    player.setPosition(my_pos.x, my_pos.y);
    
    Player enemy("hero4.png",0,0,w,h);
    enemy.setPosition(enemy_data.pos.x, enemy_data.pos.y);

    Clock clock;

    while (window.isOpen())
    {
        enemy.setTextureRect(enemy_data.pos.spr);
        enemy.setPosition(enemy_data.pos.x, enemy_data.pos.y);

        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time/800;
        //cout << time << endl;

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed){
                conn.flags.WORK = false;
                my_data.flags.WORK = false;
                window.close();
                cout << "quit..." << endl;
            }
        }

        if(Keyboard::isKeyPressed(Keyboard::LControl) && Keyboard::isKeyPressed(Keyboard::C))
        {
            conn.flags.WORK = false;
            my_data.flags.WORK = false;
            window.close();
            cout << "quit..." << endl;
        }

        if(Keyboard::isKeyPressed(Keyboard::Left)){
            if(s == l1)         {s = l2;}
            else if(s == l2)    {s = l3;}
            else if(s == l3)    {s = l1;}
            else                {s = l1;}

            my_pos.spr = s;
            my_pos.x -= speed*time;
            my_data.pos = my_pos;
            my_data.flags.MOVING = true;
            player.move(-speed*time,0);
        }

        if(Keyboard::isKeyPressed(Keyboard::Right)){
            if(s == r1)         {s = r2;}
            else if(s == r2)    {s = r3;}
            else if(s == r3)    {s = r1;}
            else                {s = r1;}

            my_pos.spr = s;
            my_pos.x += speed*time;
            my_data.pos = my_pos;
            my_data.flags.MOVING = true;
            player.move(speed*time,0);
        }

        if(Keyboard::isKeyPressed(Keyboard::Up)){
            if(s == u1)         {s = u2;}
            else if(s == u2)    {s = u3;}
            else if(s == u3)    {s = u1;}
            else                {s = u1;}

            my_pos.spr = s;
            my_pos.y -= speed*time;
            my_data.pos = my_pos;
            my_data.flags.MOVING = true;
            player.move(0,-speed*time);
        }

        if(Keyboard::isKeyPressed(Keyboard::Down)){
            if(s == d1)         {s = d2;}
            else if(s == d2)    {s = d3;}
            else if(s == d3)    {s = d1;}
            else                {s = d1;}

            my_pos.spr = s;
            my_pos.y += speed*time;
            my_data.pos = my_pos;
            my_data.flags.MOVING = true;
            player.move(0,speed*time);
        }
        player.setTextureRect(s);

        window.clear();
        for(int i = 0; i < RES_HEIGHT/32; i++){
            string get = m[i];
            for(int j = 0; j < RES_WIDTH/32; j++){
                if(get[j] == ' ')
                    s_map.setTextureRect(IntRect(0,0,32,32));
                if(get[j] == 's')
                    s_map.setTextureRect(IntRect(32,0,32,32));
                if(get[j] == '0')
                    s_map.setTextureRect(IntRect(64,0,32,32));

            s_map.setPosition(j*32,i*32);
            window.draw(s_map);
            }
        }
        window.draw(player.sprite);
        window.draw(enemy.sprite);
        window.display();
        my_data.flags.MOVING = false;
    }
    return 0;
}
