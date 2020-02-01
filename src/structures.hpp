#pragma once

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include "zmq.h"

typedef struct PP_data{
    float x;
    float y;
    pair <int,int>spr;
    bool CLOSE = false;
}PP_data;

typedef struct Flags{
	bool WORK;
	bool MOVING;
}Flags;

typedef struct Thread_data{
    zmq_msg_t msg_type;
    void* socket_type;
    Flags flags;
   	PP_data pos;
}thread_data;
