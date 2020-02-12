#pragma once

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include "zmq.h"

#include <pthread.h>

using namespace std;

bool PAUSE;

void* ptp_get(void *args);

void* ptp_send(void *args);

class Connection{
private:
    bool pushPTH = false, pullPTH = false;
    pthread_t thread_recv, thread_send;

    void* context;

    void* pull;
    zmq_msg_t recv;

    void* push;
    zmq_msg_t send;

    string address_push, address_pull;

public:
    Flags flags = {false, false};

    Connection(string port_push, string port_pull){
        address_push = port_push;
        address_pull = port_pull;

        context = zmq_ctx_new();

        pull = zmq_socket(context, ZMQ_PULL);
        if(zmq_bind(pull, address_pull.c_str()) < 0){
            strerror(errno);
        }

        push = zmq_socket(context, ZMQ_PUSH);
        if(zmq_connect(push, address_push.c_str()) == 0){
            cout << address_pull.c_str() << "-connected to " << address_push.c_str() << " for PUSH" << endl;
        }
    }

    void pushPos(PP_data my_pos){
        zmq_msg_init_size(&send, sizeof(PP_data));
        memcpy(zmq_msg_data(&send), &my_pos, sizeof(PP_data));
        zmq_msg_send(&send, push, 0);
        zmq_msg_close(&send);
        //cout << "push there" << endl;
    }
    void pushPosThreaded(Thread_data *data_push){
        if(!pushPTH){
            int status_push;
            data_push->msg_type = send;
            data_push->socket_type = push;
            data_push->flags = flags;
            status_push = pthread_create(&thread_send, NULL, ptp_send, (void *) data_push);
            if(status_push != 0) {
                pushPTH = false;
                cout << "main error: can't create thread_send, status_push = " << status_push << endl;
                exit(-11);
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
    PP_data pullPos(){
        cout << "there_pull" << endl;
        zmq_msg_init(&recv);
        zmq_msg_recv(&recv, pull, 0);
        PP_data *pos_back = (PP_data *) zmq_msg_data(&recv); 
        zmq_msg_close(&recv);
        //cout << "there_pull" << endl;
        return (*pos_back);
    }
    void pullPosThreaded(Thread_data *data_recv){
        if(!pullPTH){
            int status_pull;
            data_recv->msg_type = recv;
            data_recv->socket_type = pull;
            status_pull = pthread_create(&thread_recv, NULL, ptp_get, (void *) data_recv);
            if(status_pull != 0) {
                pullPTH = false;
                cout << "main error: can't create thread_recv, status_pull = " << status_pull << endl;
                exit(-11);
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

void* ptp_get(void *args){
    Thread_data *data = (thread_data*) args;
    zmq_msg_t recv = data->msg_type;
    void* pull = data->socket_type;
    bool WORK = true;
    while(WORK){
        zmq_msg_init(&recv);
        zmq_msg_recv(&recv, pull, 0);
        PP_data *buffer = (PP_data *) zmq_msg_data(&recv);
        if(buffer->CLOSE == true){
            PAUSE = true;
        }
        else{
            PAUSE = false;
        }
        data->pos = (*buffer);
        zmq_msg_close(&recv);
        //cout << data->pos.x << endl;
    }
    cout << "-get thread quit..." << endl;
    return (void *) 1;
}

void* ptp_send(void *args){
    Thread_data *data = (thread_data*) args;
    zmq_msg_t send = data->msg_type;
    void *push = data->socket_type;
    bool WORK = true;
    while(WORK){
        if(data->flags.MOVING && PAUSE == false){
            cout << "sending...   " << "x = " << data->pos.x  << " y = " << data->pos.y << endl;
            zmq_msg_init_size(&send, sizeof(PP_data));
            memcpy(zmq_msg_data(&send), &data->pos, sizeof(PP_data));
            zmq_msg_send(&send, push, 0);
            zmq_msg_close(&send);
        }
    }
    cout << "-send thread quit..." << endl;
    return (void *) 1;
}