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
float w = 96, h = 96;

float speed = 0.3;

pair <int,int>emp(0,0);
pair <int,int>l1(0,96);    pair <int,int>l2(96,96);    pair <int,int>l3(192,96);
pair <int,int>r1(0,192);   pair <int,int>r2(96,192);   pair <int,int>r3(192,192);
pair <int,int>u1(0,288);    pair <int,int>u2(96,288);    pair <int,int>u3(192,288);
pair <int,int>d1(0,0);    pair <int,int>d2(96,0);   pair <int,int>d3(192,0);

class Player{
public:
    float x, y, w, h;
    String File;
    Image image;
    Texture texture;
    Sprite sprite;

    Player();

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
    }
    void setTextureRect(pair <int,int>s){
        sprite.setTextureRect(IntRect(s.first,s.second,w,h));
    }
};

struct Position{
    float x;
    float y;
    pair <int,int>spr;
};
Position pos = {400,300,emp};
Position *pos_back;

string push_port = "4041"; //tcp://localhost:
string pull_port = "4040"; //tcp://*:

struct thread_data{
    zmq_msg_t msg_type;
    void* socket_type;
};
void* ptp_get(void *args){
    thread_data *data = (thread_data*) args;
    zmq_msg_t recv = data->msg_type;
    void* pull = data->socket_type;
    while(WORK){
        zmq_msg_init(&recv);
        zmq_msg_recv(&recv, pull, 0);
        pos_back = (Position *) zmq_msg_data(&recv); 
        zmq_msg_close(&recv);
    }
    return (void *) 1;
}

void* ptp_send(void *args){
    thread_data *data = (thread_data*) args;
    zmq_msg_t send = data->msg_type;
    void* push = data->socket_type;
    while(WORK){
        if(MOVING){
            cout << "sending...   " << "x = " << pos.x  << " y = " << pos.y << endl;
            zmq_msg_init_size(&send, sizeof(Position));
            memcpy(zmq_msg_data(&send), &pos, sizeof(Position));
            zmq_msg_send(&send, push, 0);
            zmq_msg_close(&send);
        }
    }
    return (void *) 1;
}
class Connection{
private:
    bool pushPTH = false, pullPTH = false;
    pthread_t thread_recv, thread_send;

    void* context;
    void* pull;
    void* push;
    zmq_msg_t send, recv;

    string address_push, address_pull;
public:
    Connection(string port_push, string port_pull){
        address_push = "tcp://localhost:" + port_push;
        address_pull = "tcp://*:" + port_pull;

        context = zmq_ctx_new();

        pull = zmq_socket(context, ZMQ_PULL);
        if(zmq_bind(pull, address_pull.c_str()) < 0){
            strerror(errno);
        }

        push = zmq_socket(context, ZMQ_PUSH);
        if(zmq_connect(push, address_push.c_str()) == 0){
            cout << address_push.c_str() << "-connected to " << address_push.c_str() << " for PUSH" << endl;
        }
    }

    void pushPos(){
        zmq_msg_init_size(&send, sizeof(Position));
        memcpy(zmq_msg_data(&send), &pos, sizeof(Position));
        zmq_msg_send(&send, push, 0);
        zmq_msg_close(&send);
    }
    void pushPosThreaded(){
        if(!pushPTH){
            int status_push;
            thread_data data_pub = {send, push};

            status_push = pthread_create(&thread_send, NULL, ptp_send, (void *) &data_pub);
            if(status_push != 0) {
                pushPTH = false;
                cout << "main error: can't create thread_recv, status_push = " << status_push << endl;
                exit(-11);
                //return 0;
            }
            else if(status_push == 0){
                pushPTH = true;
                cout << "-Threaded pushing..." << endl;
            }
        }
        else{
            cout << "Threaded pushing is already running...." << endl;
        }
    }
    void pullPos(){
        zmq_msg_init(&recv);
        zmq_msg_recv(&recv, pull, 0);
        pos_back = (Position *) zmq_msg_data(&recv); 
        zmq_msg_close(&recv);
    }
    void pullPosThreaded(){
        if(!pullPTH){
            int status_pull;
            thread_data data_recv = {recv, pull};
            status_pull = pthread_create(&thread_recv, NULL, ptp_get, (void *) &data_recv);
            if(status_pull != 0) {
                pullPTH = false;
                cout << "main error: can't create thread_pull, status_pull = " << status_pull << endl;
                exit(-11);
                //return 0;
            }
            else if(status_pull == 0){
                pullPTH = true;
                cout << "-Threaded pulling..." << endl;
            }
        }
        else{
            cout << "Threaded pulling is already running...." << endl;
        }
    }

};

int main()
{
    Connection conn(push_port, pull_port);

    conn.pushPos();
    conn.pullPos();

    WORK = true;

    conn.pushPosThreaded();
    conn.pullPosThreaded();

    RenderWindow window(sf::VideoMode(640, 480), "Game_sub"/*, Style::Fullscreen*/);
    cout << "let's go..." << endl;
    
    pair <int,int>s;

    Player player("hero.png",0,0,w,h);
    player.setPosition(pos.x, pos.y);

    Player enemy("hero.png",0,0,w,h);
    enemy.setPosition(pos_back->x, pos_back->y);

    Clock clock;

    while (window.isOpen())
    {
        enemy.setTextureRect(pos_back->spr);
        enemy.setPosition(pos_back->x, pos_back->y);

        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time/800;
        //cout << time << endl;

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed){
                WORK = false;
                window.close();
            }
        }

        if(Keyboard::isKeyPressed(Keyboard::LControl) && Keyboard::isKeyPressed(Keyboard::C))
        {
            WORK = false;
            window.close();
        }

        if(Keyboard::isKeyPressed(Keyboard::Left)){
            if(s == l1)         {s = l2; pos.spr = l2;}
            else if(s == l2)    {s = l3; pos.spr = l3;}
            else if(s == l3)    {s = l1; pos.spr = l1;}
            else                {s = l1; pos.spr = l1;}

            pos.x -= speed*time;
            MOVING = true;
            player.move(-speed*time,0);
        }

        if(Keyboard::isKeyPressed(Keyboard::Right)){
            if(s == r1)         {s = r2; pos.spr = r2;}
            else if(s == r2)    {s = r3; pos.spr = r3;}
            else if(s == r3)    {s = r1; pos.spr = r1;}
            else                {s = r1; pos.spr = r1;}

            pos.x += speed*time;
            MOVING = true;
            player.move(speed*time,0);
        }

        if(Keyboard::isKeyPressed(Keyboard::Up)){
            if(s == u1)         {s = u2; pos.spr = u2;}
            else if(s == u2)    {s = u3; pos.spr = u3;}
            else if(s == u3)    {s = u1; pos.spr = u1;}
            else                {s = u1; pos.spr = u1;}

            pos.y -= speed*time;
            MOVING = true;
            player.move(0,-speed*time);
        }

        if(Keyboard::isKeyPressed(Keyboard::Down)){
            if(s == d1)         {s = d2; pos.spr = d2;}
            else if(s == d2)    {s = d3; pos.spr = d3;}
            else if(s == d3)    {s = d1; pos.spr = d1;}
            else                {s = d1; pos.spr = d1;}

            pos.y += speed*time;
            MOVING = true;
            player.move(0,speed*time);
        }
        player.setTextureRect(s);

        window.clear();
        window.draw(player.sprite);
        window.draw(enemy.sprite);
        window.display();
        MOVING = false;
    }
    return 0;
}
