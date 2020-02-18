#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <sstream>

using namespace std;
using namespace sf;
#include "include/structures.hpp"
#include "include/config.hpp"

#include "include/player.hpp"
#include "include/healthpoints.hpp"
#include "include/connection.hpp"
#include "include/bullet.hpp"

PP_data my_pos = {false,RES_WIDTH-w-32,RES_HEIGHT-h-32,0,2,false,false};
//PP_data *enemy_pos;

string push_port = "tcp://localhost:8080"; //tcp://localhost:
string pull_port = "tcp://*:8081"; //tcp://*:
bool MOVING = false;
bool SHOOT = false;

bool PLAYER_DEAD = false;
bool ENEMY_DEAD = false;

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

    Player player("bat.png",0,0,w,h);
    player.setPosition(my_pos.x, my_pos.y);
    
    Player enemy("bee.png",0,0,w,h);
    enemy.setPosition(enemy_data.pos.x, enemy_data.pos.y);

    Bullet player_bullet("b2.png",0,0,16,16);
    Bullet enemy_bullet("b1.png",0,0,16,16);

    Health player_hp(player);
    Health enemy_hp(enemy);

     Font font;
    font.loadFromFile("sprites/19072.ttf");
    Text gameover("GAME is OVER", font, 48);
    gameover.setFillColor(sf::Color::White);
    gameover.setStyle(sf::Text::Bold);
    gameover.setPosition(64,RES_HEIGHT/2 - 64);

    Text winner("", font, 32);
    winner.setFillColor(sf::Color::White);
    winner.setPosition(64,RES_HEIGHT/2);

    Text waiter("Another player disconnected\nwaiting for enemy reconnect\n time left: ", font, 24);
    waiter.setFillColor(sf::Color::White);
    waiter.setPosition(64,RES_HEIGHT/2 - 64);

    my_data.pos.CLOSE = false;
    my_data.pos.SHOOT = false;
    Clock clock;
    Clock pause_clock;

    int animStay = 0;
    while (window.isOpen())
    {
        animStay++;

        enemy.dir = enemy_data.pos.dir;
        enemy.setTextureRect(enemy_data.pos.sprnum, enemy_data.pos.dir);
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
                my_pos.CLOSE = true;
                my_data.pos.CLOSE = true;
                my_data.pos = my_pos;
                conn.pushPos(my_data.pos);
            }
        }

        if(enemy_data.pos.PAUSE){
            pause_clock.restart();
            int pause_time = 0;
            while(pause_time != 10){
                pause_time = pause_clock.getElapsedTime().asSeconds();
                std::stringstream ss;
                ss << pause_time;
                waiter.setString("Another player disconnected\nwaiting for enemy reconnection\n time left: " + ss.str());

                window.clear();
                window.draw(waiter);
                window.display();
                while (window.pollEvent(event)){
                    if (event.type == Event::Closed){
                        conn.flags.WORK = false;
                        my_data.flags.WORK = false;
                        window.close();
                        system("clear");
                        cout << "quit..." << endl;
                        my_pos.CLOSE = true;
                        my_data.pos.CLOSE = true;
                        my_data.pos = my_pos;
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
                    my_pos.CLOSE = true;
                    my_data.pos.CLOSE = true;
                    my_data.pos = my_pos;
                    conn.pushPos(my_data.pos);
                }
                if(!enemy_data.pos.PAUSE)
                    break;
            }
            if(enemy_data.pos.PAUSE){
                window.clear();
                cout << "GAME is OVER..." << endl;
                window.draw(gameover);
                window.display();
                sleep(2);
                conn.flags.WORK = false;
                my_data.flags.WORK = false;
                window.close();
                system("clear");
                cout << "quit..." << endl;
                my_pos.CLOSE = true;
                my_data.pos.CLOSE = true;
                my_data.pos = my_pos;
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
            my_pos.CLOSE = true;
            my_data.pos.CLOSE = true;
            my_data.pos = my_pos;
            conn.pushPos(my_data.pos);
        }

        if(Keyboard::isKeyPressed(Keyboard::Left)){
            player.dir = 1; player.speed = speed;
            MOVING = true;
        }

        if(Keyboard::isKeyPressed(Keyboard::Right)){
            player.dir = 3; player.speed = speed;
            MOVING = true;
        }

        if(Keyboard::isKeyPressed(Keyboard::Up)){
            player.dir = 0; player.speed = speed;
            MOVING = true;
        }

        if(Keyboard::isKeyPressed(Keyboard::Down)){
            player.dir = 2; player.speed = speed;
            MOVING = true;
        }

        if(Keyboard::isKeyPressed(Keyboard::Space)){
            player_bullet.SHOOT = true;
            my_pos.SHOOT = true;
            player_bullet.dir = player.dir;
            my_pos.dir = player.dir;
            int x = player.X();
            int y = player.Y();
            if(player_bullet.dir == 0)
                player_bullet.setPosition(x + 9, y - 16);
            else if(player_bullet.dir == 2)
                player_bullet.setPosition(x + 9, y + 32);
            else if(player_bullet.dir == 1)
                player_bullet.setPosition(x - 16, y + 9);
            else if(player_bullet.dir == 3)
                player_bullet.setPosition(x + 32, y + 9);
        }
        if(MOVING || animStay == 10){
            player.sprnum++;
            if(player.sprnum == 3){
                player.sprnum = 0;
            }
            animStay = 0;
        }
        player.update(time, enemy);
        PLAYER_DEAD = player_hp.update(player, enemy_bullet.ENEMYHIT);
        ENEMY_DEAD = enemy_hp.update(enemy, player_bullet.ENEMYHIT);


        player_bullet.update(time,enemy);
        if(enemy_data.pos.SHOOT){
            enemy_bullet.dir = enemy_data.pos.dir;
            int x = enemy.X();
            int y = enemy.Y();
            if(enemy_bullet.dir == 0)
                enemy_bullet.setPosition(x + 9, y - 16);
            else if(enemy_bullet.dir == 2)
                enemy_bullet.setPosition(x + 9, y + 32);
            else if(enemy_bullet.dir == 1)
                enemy_bullet.setPosition(x - 16, y + 9);
            else if(enemy_bullet.dir == 3)
                enemy_bullet.setPosition(x + 32, y + 9); 
            enemy_bullet.SHOOT = true;
        }
        enemy_bullet.update(time,player);

        //player_hp.health = my_data.pos.health;
        my_pos.sprnum = player.sprnum;
        my_pos.x = player.X();
        my_pos.y = player.Y();
        //my_pos.health = player_hp.health;
        my_data.pos = my_pos;

        my_data.flags.MOVING = true;

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
        window.draw(player_hp.sprite);

        window.draw(enemy.sprite);
        window.draw(enemy_hp.sprite);

        if(player_bullet.SHOOT){
            window.draw(player_bullet.sprite);
        }
        if(enemy_bullet.SHOOT){
            window.draw(enemy_bullet.sprite);
        }
        if(PLAYER_DEAD || ENEMY_DEAD){
            cout << "GAME is OVER..." << endl;
            window.draw(gameover);
            if(PLAYER_DEAD){
                winner.setString("The winner is Bee...");
                winner.setFillColor(sf::Color(232, 107, 6));
            }
            if(ENEMY_DEAD){
                winner.setString("The winner is Bat...");
                winner.setFillColor(sf::Color(163, 73, 164));
            }
            window.draw(winner);
        }   
        window.display();

        MOVING = false;
        my_data.flags.MOVING = false;
        my_pos.SHOOT = false;
    }
    return 0;
}

