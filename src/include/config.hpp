#pragma once

#include <iostream>
#include "structures.hpp"

float w = 32, h = 32;

float speed = 0.3;
/*
Spr_data emp = {0,0,0,0};

Spr_data d1 = {27,5,40,90}, d2 = {124,5,40,90}, d3 = {220,5,40,90};
Spr_data l1 = {5,135,90,60}, l2 = {101,135,90,60}, l3 = {197,135,90,60};
Spr_data r1 = {0,230,90,60}, r2 = {96,230,90,60}, r3 = {192,230,90,60};
Spr_data u1 = {27,293,40,90}, u2 = {124,293,40,90}, u3 = {220,293,40,90};
*/

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