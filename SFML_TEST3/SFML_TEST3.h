#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <fstream>
#include "API.h"


const int FPS = 60;
const std::chrono::milliseconds frame_delay(1000 / FPS);


const double GRAVITY = 0.000425;
const double FRICTION = 0.005;
const double windowWidth = 400;
const double windowHeight = 400;

double blockSizeX = 100;
double blockSizeY = 100;

double playerSizeX = 25;
double playerSizeY = 25;

class Block;

class Base
{

protected:
    double cordX;
    double cordY;


public:
    Base();
    ~Base();
};


class Player : protected Base
{
protected:
    double speed;      
    double vertSpeed = 0.0;  
    double jumpForce;
    double accel;
    bool grounded = false;

public:
    Player(double x, double y, double s, double j, double a);

    Player();

    void PrintState();


    bool IsGrounded();
    void SetGrounded(bool val);

    bool UpdatePos();
    
    double GetVertSpeed();
    void SetVertSpeed(double v);
    void Accelerate(int x);
    

    void Jump();
    

    double X();
    
    double Y();
    
    double X2();

        double Y2();
   

    void ResetPlayer();
    

    void SetX(double newX);
   

    void SetY(double newY);
    

    ~Player();
    
    friend void collision(Player& gracz, Block* bloki, int n);
};





class Block : protected Base
{


public:
    bool solid;
    Block();
    

    Block(double x, double y, double state);
   

    ~Block();
    

    double X();
    
    double Y();
    


    double X2();
    

    double Y2();
    

    friend void collision(Player& gracz, Block* bloki, int n);
};



void collision(Player& gracz, Block* bloki, int n);




bool readValuesFromFile(const std::string& filename, int values[16]);