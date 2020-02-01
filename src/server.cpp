#include <iostream>
//#include <cstdlib>

using namespace std;

#include "structures.hpp"

PP_data pos1;
PP_data pos2;

Thread_data data1;
Thread_data data2;


#include "connection.hpp"


string push_port1 = "4041"; //tcp://localhost:
string pull_port1 = "4040"; //tcp://*:

string push_port2 = "8081"; //tcp://localhost:
string pull_port2 = "8080"; //tcp://*:



int main()
{
    Connection player1(push_port1, pull_port1);
    Connection player2(push_port2, pull_port2);

    data1.pos = player1.pullPos();
    cout << "there" << endl;
    
    data2.pos = player2.pullPos();
    cout << "there" << endl;

    player1.pushPos(data2.pos);
    player2.pushPos(data1.pos);
    
    player1.flags = {true,true};
    player2.flags = {true,true};



    player1.pullPosThreaded(&data1);
    player2.pullPosThreaded(&data2);

    player2.pushPosThreaded(&data1);
    player1.pushPosThreaded(&data2);


    bool WORK = true;
    string x;
    while(WORK){
        cin >> x;
        if(x == "q"){
            break;
        }
    }
    return 0;
}
