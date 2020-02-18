#pragma once

#include <iostream>

float w = 32, h = 32;

float speed = 0.3;

pair <int,int>emp(0,0);

pair <int,int>u1(0,0);    pair <int,int>u2(32,0);    pair <int,int>u3(64,0);
pair <int,int>l1(0,32);   pair <int,int>l2(32,32);   pair <int,int>l3(64,32);
pair <int,int>d1(0,64);    pair <int,int>d2(32,64);    pair <int,int>d3(64,64);
pair <int,int>r1(0,96);    pair <int,int>r2(32,96);   pair <int,int>r3(64,96);

/*
pair <int,int>l1(0,96);    pair <int,int>l2(96,96);    pair <int,int>l3(192,96);
pair <int,int>r1(0,192);   pair <int,int>r2(96,192);   pair <int,int>r3(192,192);
pair <int,int>u1(0,288);    pair <int,int>u2(96,288);    pair <int,int>u3(192,288);
pair <int,int>d1(0,0);    pair <int,int>d2(96,0);   pair <int,int>d3(192,0);
*/