#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>

using namespace std;
using namespace sf;

#include "include/config.hpp"
#include "include/player.hpp"
#include "include/connection.hpp"

PP_data my_pos = {32,32,emp,false};
PP_data *enemy_pos;

string push_port = "4040"; //tcp://localhost:
string pull_port = "4041"; //tcp://*:
bool MOVING = false;

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

    RenderWindow window(sf::VideoMode(RES_WIDTH, RES_HEIGHT), "Game"/*, Style::Fullscreen*/);
    cout << "let's go..." << endl;
    
    pair <int,int>s;
    //Spr_data s;

    Image map_image;
    map_image.loadFromFile("sprites/map.png");
    Texture map;
    map.loadFromImage(map_image);
    Sprite s_map;
    s_map.setTexture(map);

    Player player("bee.png",0,0,w,h);
    player.setPosition(my_pos.x, my_pos.y);
    
    Player enemy("bat.png",0,0,w,h);
    enemy.setPosition(enemy_data.pos.x, enemy_data.pos.y);

    Clock clock;
    my_data.pos.CLOSE = false;
    while (window.isOpen())
    {
        enemy.setTextureRect(enemy_data.pos.spr);
        enemy.setPosition(enemy_data.pos.x, enemy_data.pos.y);

        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time/800;
        //cout << time << endl;

        Event event;
        while (window.pollEvent(event)){
            if (event.type == Event::Closed){
                conn.flags.WORK = false;
                my_data.flags.WORK = false;
                window.close();
                system("clear");
                cout << "quit..." << endl;
                my_data.pos.CLOSE = true;
                conn.pushPos(my_data.pos);
            }
        }

        if(Keyboard::isKeyPressed(Keyboard::LControl) && Keyboard::isKeyPressed(Keyboard::C))
        {
            conn.flags.WORK = false;
            my_data.flags.WORK = false;
            window.close();
            system("clear");
            cout << "quit..." << endl;
            my_data.pos.CLOSE = true;
            conn.pushPos(my_data.pos);
        }
/*
        if(Keyboard::isKeyPressed(Keyboard::Left)){
            if(s.x0 == l1.x0 && s.y0 == l1.y0)         {s = l2;}
            else if(s.x0 == l2.x0 && s.y0 == l2.y0)    {s = l3;}
            else if(s.x0 == l3.x0 && s.y0 == l3.y0)    {s = l1;}
            else                                        {s = l1;}

            player.dir = 1; player.speed = speed;
            MOVING = true;
        }

        if(Keyboard::isKeyPressed(Keyboard::Right)){
            if(s.x0 == r1.x0 && s.y0 == r1.y0)         {s = r2;}
            else if(s.x0 == r2.x0 && s.y0 == r2.y0)    {s = r3;}
            else if(s.x0 == r3.x0 && s.y0 == r3.y0)    {s = r1;}
            else                                        {s = r1;}

            player.dir = 0; player.speed = speed;
            MOVING = true;
        }

        if(Keyboard::isKeyPressed(Keyboard::Up)){
            if(s.x0 == u1.x0 && s.y0 == u1.y0)         {s = u2;}
            else if(s.x0 == u2.x0 && s.y0 == u2.y0)    {s = u3;}
            else if(s.x0 == u3.x0 && s.y0 == u3.y0)    {s = u1;}
            else                                        {s = u1;}

            player.dir = 3; player.speed = speed;
            MOVING = true;
        }

        if(Keyboard::isKeyPressed(Keyboard::Down)){
            if(s.x0 == d1.x0 && s.y0 == d1.y0)         {s = d2;}
            else if(s.x0 == d2.x0 && s.y0 == d2.y0)    {s = d3;}
            else if(s.x0 == d3.x0 && s.y0 == d3.y0)    {s = d1;}
            else                                        {s = d1;}

            player.dir = 2; player.speed = speed;
            MOVING = true;
        }*/
        if(Keyboard::isKeyPressed(Keyboard::Left)){
            if(s == l1)         {s = l2;}
            else if(s == l2)    {s = l3;}
            else if(s == l3)    {s = l1;}
            else                {s = l1;}

            player.dir = 1; player.speed = speed;
            MOVING = true;
        }

        else if(Keyboard::isKeyPressed(Keyboard::Right)){
            if(s == r1)         {s = r2;}
            else if(s == r2)    {s = r3;}
            else if(s == r3)    {s = r1;}
            else                {s = r1;}

            player.dir = 0; player.speed = speed;
            MOVING = true;
        }

        else if(Keyboard::isKeyPressed(Keyboard::Up)){
            if(s == u1)         {s = u2;}
            else if(s == u2)    {s = u3;}
            else if(s == u3)    {s = u1;}
            else                {s = u1;}

            player.dir = 3; player.speed = speed;
            MOVING = true;
        }

        else if(Keyboard::isKeyPressed(Keyboard::Down)){
            if(s == d1)         {s = d2;}
            else if(s == d2)    {s = d3;}
            else if(s == d3)    {s = d1;}
            else                {s = d1;}

            player.dir = 2; player.speed = speed;
            MOVING = true;
        }
        player.setTextureRect(s);
        player.update(time, enemy);
        if(player.Y() > RES_HEIGHT-h-32 || player.X() > RES_WIDTH-w-32 || player.X() < 32 || player.Y() < 32)
            player.setPosition(my_pos.x,my_pos.y);
        my_pos.spr = s;
        my_pos.x = player.X();
        my_pos.y = player.Y();
        my_data.pos = my_pos;
        if(MOVING){
            my_data.flags.MOVING = true;
        }

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

        MOVING = false;
        my_data.flags.MOVING = false;
    }
    return 0;
}
