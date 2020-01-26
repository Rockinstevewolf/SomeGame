#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>

#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include "zmq.h"

#include <pthread.h>

using namespace std;
using namespace sf;

bool MOVING = false;
bool WORK = false;
int w = 96, h = 96;
float speed = 0.3;

pair <int,int>emp(0,0);

struct Position{
    float x;
    float y;
    pair <int,int>spr;
}pos = {400,300,emp};

struct thread_data{
    zmq_msg_t msg_type;
    void* socket_type;
};

Sprite enemysprite;
Position pos_send, *pos_back;

pair <int,int>l1(0,96);    pair <int,int>l2(96,96);    pair <int,int>l3(192,96);

pair <int,int>r1(0,192);   pair <int,int>r2(96,192);   pair <int,int>r3(192,192);

pair <int,int>u1(0,288);    pair <int,int>u2(96,288);    pair <int,int>u3(192,288);

pair <int,int>d1(0,0);    pair <int,int>d2(96,0);   pair <int,int>d3(192,0);


string address_pull = "tcp://*:4040";
string address_push = "tcp://localhost:4041";


void* ptp_get(void *args){
    thread_data *data = (thread_data*) args;
    zmq_msg_t recv = data->msg_type;
    void* pull = data->socket_type;
    cout << "there_thread" << endl;
    while(WORK){
        zmq_msg_init(&recv);
        zmq_msg_recv(&recv, pull, 0);
        pos_back = (Position *) zmq_msg_data(&recv); 
        zmq_msg_close(&recv);

        enemysprite.setTextureRect(IntRect(pos_back->spr.first,pos_back->spr.second,w,h));
    }
    return (void *) 1;
}

void* ptp_send(void *args){
    thread_data *data = (thread_data*) args;
    zmq_msg_t send = data->msg_type;
    void* push = data->socket_type;
    while(WORK){
        if(MOVING){
            cout << "sending...   " << "x = " << pos.x  << "y = " << pos.y << endl;
            zmq_msg_init_size(&send, sizeof(Position));
            memcpy(zmq_msg_data(&send), &pos, sizeof(Position));
            zmq_msg_send(&send, push, 0);
            zmq_msg_close(&send);
        }
    }
    return (void *) 1;
}

int main()
{
    pos.spr = l1;

    void* context = zmq_ctx_new();

    void* pull = zmq_socket(context, ZMQ_PULL);
    if(zmq_bind(pull, address_pull.c_str())<0)
        strerror(errno);

    void* push = zmq_socket(context, ZMQ_PUSH);
    if(zmq_connect(push, address_push.c_str()) == 0){
        cout <<"-connected to " << address_push.c_str() << " for PUSH" << endl;
    }

    zmq_msg_t recv, send;

    pos_send = pos;

    zmq_msg_init_size(&send, sizeof(Position));
    memcpy(zmq_msg_data(&send), &pos_send, sizeof(Position));
    zmq_msg_send(&send, push, 0);
    zmq_msg_close(&send);

    zmq_msg_init(&recv);
    zmq_msg_recv(&recv, pull, 0);
    pos_back = (Position *) zmq_msg_data(&recv); 
    zmq_msg_close(&recv);

    WORK = true;
////////////////////////////////////////////////////////////////////////////////////////
    bool PTHREAD = false;
    pthread_t thread_send, thread_recv;

    int status_send, status_send_addr;
    thread_data data_send = {send, push};
    
    status_send = pthread_create(&thread_send, NULL, ptp_send, (void *) &data_send);
    if(status_send != 0) {
        PTHREAD = false;
        printf("main error: can't create thread, status = %d\n", status_send);
        exit(-11);
        return 0;
    }
    else if(status_send == 0){
        PTHREAD = true;
    }

    int status_recv, status_recv_addr;
    thread_data data_recv = {recv, pull};

    status_recv = pthread_create(&thread_recv, NULL, ptp_get, (void *) &data_recv);
    if(status_recv != 0) {
        PTHREAD = false;
        printf("main error: can't create thread, status = %d\n", status_recv);
        exit(-11);
        return 0;
    }
    else if(status_recv == 0){
        PTHREAD = true;
    }
/////////////////////////////////////////////////////////////////////////////////////////////////

    RenderWindow window(sf::VideoMode(640, 480), "Game"/*, Style::Fullscreen*/);
    cout << "let's go..." << endl;

    pair <int,int>s;
    Image heroimage;
    heroimage.loadFromFile("sprites/hero.png");

    Texture herotexture;
    herotexture.loadFromImage(heroimage);

    Sprite herosprite;

    s = r1;
    //cout << pos_back->x << endl;
    herosprite.setTexture(herotexture);
    herosprite.setTextureRect(IntRect(s.first,s.second,w,h));
    herosprite.setPosition(pos.x, pos.y);
    
    enemysprite.setTexture(herotexture);
    enemysprite.setTextureRect(IntRect(pos_back->spr.first,pos_back->spr.second,w,h));
    enemysprite.setPosition(pos_back->x, pos_back->y);

    Clock clock;
    while (window.isOpen())
    {
        enemysprite.setPosition(pos_back->x, pos_back->y);

        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time/800;
        //cout << time << endl;

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed){
                PTHREAD = false;
                WORK = false;
                window.close();
            }
        }

        if(Keyboard::isKeyPressed(Keyboard::LControl) && Keyboard::isKeyPressed(Keyboard::C))
        {
            PTHREAD = false;
            WORK = false;
            window.close();
        }

        if(Keyboard::isKeyPressed(Keyboard::Left)){
            if(s == l1)         {s = l2; pos.spr = l2;}
            else if(s == l2)    {s = l3; pos.spr = l3;}
            else if(s == l3)    {s = l1; pos.spr = l1;}
            else                {s = l1; pos.spr = l1;}

            herosprite.setTextureRect(IntRect(s.first,s.second,w,h));
            pos.x -= speed*time;
            MOVING = true;
            herosprite.move(-speed*time,0);
        }

        if(Keyboard::isKeyPressed(Keyboard::Right)){
            if(s == r1)         {s = r2; pos.spr = r2;}
            else if(s == r2)    {s = r3; pos.spr = r3;}
            else if(s == r3)    {s = r1; pos.spr = r1;}
            else                {s = r1; pos.spr = r1;}

            herosprite.setTextureRect(IntRect(s.first,s.second,w,h));
            pos.x += speed*time;
            MOVING = true;
            herosprite.move(speed*time,0);
        }

        if(Keyboard::isKeyPressed(Keyboard::Up)){
            if(s == u1)         {s = u2; pos.spr = u2;}
            else if(s == u2)    {s = u3; pos.spr = u3;}
            else if(s == u3)    {s = u1; pos.spr = u1;}
            else                {s = u1; pos.spr = u1;}

            herosprite.setTextureRect(IntRect(s.first,s.second,w,h));
            pos.y -= speed*time;
            MOVING = true;
            herosprite.move(0,-speed*time);
        }

        if(Keyboard::isKeyPressed(Keyboard::Down)){
            if(s == d1)         {s = d2; pos.spr = d2;}
            else if(s == d2)    {s = d3; pos.spr = d3;}
            else if(s == d3)    {s = d1; pos.spr = d1;}
            else                {s = d1; pos.spr = d1;}

            herosprite.setTextureRect(IntRect(s.first,s.second,w,h));
            pos.y += speed*time;
            MOVING = true;
            herosprite.move(0,speed*time);
        }
        pos_send = pos;
        window.clear();
        window.draw(herosprite);
        window.draw(enemysprite);
        window.display();
        MOVING = false;
    }
    return 0;
    if(!PTHREAD){
        status_send = pthread_join(thread_send, (void **) &status_send_addr);
        if(status_send_addr != 1){
            printf("ERROR\n");
            return 0;
        }
        status_recv = pthread_join(thread_recv, (void **) &status_recv_addr);
        if(status_recv_addr != 1){
            printf("ERROR\n");
            return 0;
        }
        return 0;
    }
}
