#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <fstream>

#pragma once

#ifdef DLLKOT_EXPORTS
#define DLLKOT_API __declspec(dllexport)
#else
#define DLLKOT_API __declspec(dllimport)
#endif

extern "C" DLLKOT_API void kot();  // Do za³¹czania w SFML_TEST3


using namespace std;


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
    Base()
    {
        cout << "Base object created" << endl;
    }
    ~Base()
    {
        cout << "Base object destroyed" << endl;
    }
};


class Player : protected Base
{
protected:
    double speed;      // horizontal velocity
    double vertSpeed = 0.0;  // vertical velocity
    double jumpForce;
    double accel;
    bool grounded = false;

public:
    Player(double x, double y, double s, double j, double a)
    {
        cordX = x;
        cordY = y;
        speed = s;
        vertSpeed = 0.0;  // initialize vertical velocity
        jumpForce = j;
        accel = a;

        cout << "Created player object at x = " << cordX << " y = " << cordY << endl
            << "With speed = " << speed << " jumpForce = " << jumpForce << " and acceleration = " << accel << endl;
    }

    Player()
    {
        cout << "Created empty player object" << endl;
        speed = 0;
        vertSpeed = 0;
        jumpForce = 0;
        accel = 0;
    }

    void PrintState()
    {
        cout << "Player is at x = " << cordX << " y = " << cordY << endl
            << "With horizontal speed = " << speed << " vertical speed = " << vertSpeed << endl
            << "Jump force = " << jumpForce << " acceleration = " << accel << endl;
    }


    bool IsGrounded() const { return grounded; }
    void SetGrounded(bool val) { grounded = val; }

    bool UpdatePos()
    {
        bool changed = false;

        // Clamp horizontal speed
        if (speed > 1) speed = 1;
        else if (speed < -1) speed = -1;

        cordX += speed;

        // Apply friction horizontally
        if (speed > 0)
        {
            speed = (speed - FRICTION < 0) ? 0 : speed - FRICTION;
            changed = true;
        }
        else if (speed < 0)
        {
            speed = (speed + FRICTION > 0) ? 0 : speed + FRICTION;
            changed = true;
        }

        if (!grounded) {
            vertSpeed += GRAVITY;
            cordY += vertSpeed;
            changed = true;
        }
        else {
            vertSpeed = 0; // standing still vertically
        }


        return changed;
    }
    double GetVertSpeed() const { return vertSpeed; }
    void SetVertSpeed(double v) { vertSpeed = v; }
    void Accelerate(int x)
    {
        if (x == 1)
            speed += accel;
        else if (x == -1)
            speed -= accel;
        else
            cout << "Accelerate should not be called with this value" << endl;
    }

    void Jump()
    {
        if (grounded) {
            vertSpeed = -jumpForce;
            grounded = false;
        }
    }

    double X()
    {


        return cordX;
    }
    double Y()
    {

        return cordY;
    }
    double X2()
    {


        return cordX + playerSizeX;
    }
    double Y2()
    {

        return cordY + playerSizeY;
    }

    void ResetPlayer()
    {
        cordX = 0.;
        cordY = windowHeight;
        speed = 0;
        vertSpeed = 0;

    }

    void SetX(double newX)
    {
        cordX = newX;
        speed = 0;
    }

    void SetY(double newY)
    {
        cordY = newY;
        vertSpeed = 0;
    }

    ~Player()
    {
        cout << "Player object destroyed" << endl;
    }

    friend void collision(Player& gracz, Block* bloki, int n);
};





class Block : protected Base
{


public:
    bool solid;
    Block()
    {
        cout << "Block created" << endl;
    }

    Block(double x, double y, double state)
    {
        cordX = x;
        cordY = y;
        solid = state;
        cout << "Block created" << endl;
    }

    ~Block()
    {
        cout << "Block destroyed" << endl;
    }

    double X()
    {
        return cordX;
    }
    double Y()
    {
        return cordY;
    }



    double X2()
    {
        return cordX + blockSizeX;
    }

    double Y2()
    {
        return cordY + blockSizeY;
    }

    friend void collision(Player& gracz, Block* bloki, int n);
};



void collision(Player& gracz, Block* bloki, int n)
{
    gracz.SetGrounded(false); // assume not grounded unless proven

    for (int i = 0; i < n; i++) {
        Block& block = bloki[i];
        if (!block.solid)
            continue;

        bool overlapping = !(gracz.X2() < block.X() || gracz.X() > block.X2() ||
            gracz.Y2() < block.Y() || gracz.Y() > block.Y2());

        if (overlapping) {
            double centerGX = (gracz.X() + gracz.X2()) / 2.0;
            double centerGY = (gracz.Y() + gracz.Y2()) / 2.0;
            double centerBX = (block.X() + block.X2()) / 2.0;
            double centerBY = (block.Y() + block.Y2()) / 2.0;

            double overlapX = std::min(gracz.X2(), block.X2()) - std::max(gracz.X(), block.X());
            double overlapY = std::min(gracz.Y2(), block.Y2()) - std::max(gracz.Y(), block.Y());

            // NEW: Prioritize vertical if falling
            if (gracz.GetVertSpeed() > 0 && centerGY < centerBY) {
                // Falling into block from above
                gracz.SetY(block.Y() - playerSizeY);
                gracz.SetGrounded(true);
                gracz.SetVertSpeed(0); // stop falling
            }
            else if (overlapX < overlapY) {
                if (centerGX < centerBX) {
                    gracz.SetX(block.X() - playerSizeX); // push left
                }
                else {
                    gracz.SetX(block.X2()); // push right
                }
            }
            else {
                if (centerGY < centerBY) {
                    gracz.SetY(block.Y() - playerSizeY); // push up
                    gracz.SetGrounded(true);
                    gracz.SetVertSpeed(0); // stop falling
                }
                else {
                    gracz.SetY(block.Y2()); // push down
                }
            }
        }
    }
}




bool readValuesFromFile(const std::string& filename, int values[16]) {
    ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file.\n";
        return false;
    }

    int count = 0;
    char ch;
    while (count < 16 && file >> noskipws >> ch) {
        if (ch == 'X' || ch == 'x') {
            values[count++] = 1;
        }
        else if (ch == 'O' || ch == 'o') {
            values[count++] = -1;
        }
        else if (ch == 'E' || ch == 'e') {
            values[count++] = 0;
        }

    }

    if (count < 16) {
        cerr << "File does not contain enough valid letters (X, O, or E).\n";
        return false;
    }



    if (values[12] == 1)
    {
        throw invalid_argument("Spawn cannot be ocupied by a solid block");
    }
    return true;
}





void updatePlayer(Player gracz)
{

}



int main()
{
    int tab[16];
    try
    {

        readValuesFromFile("info.txt", tab);
    }
    catch (const exception& e)
    {

        cerr << "Wyjatek: " << e.what() << endl;
        tab[12] = 0;
    }

    using clock = std::chrono::steady_clock;


    //x,y,speed,jump,accel
    Player gracz1(0, 400, 0, 0.3, 0.01);
    bool change;

    int n = 32;
    Block* Bloki;
    Bloki = (Block*)malloc(sizeof(Block) * n);





    for (int i = 0; i < n - 16; i++)
    {
        int r = i / 4;
        if (tab[i] == -1)
        {
            Bloki[i + 16] = Block(100 * (i % 4), 0 + 100 * r, false);
        }
        if (tab[i] == 1)
        {
            Bloki[i + 16] = Block(100 * (i % 4), 0 + 100 * r, true);
        }
        if (tab[i] == 0)
        {
            Bloki[i + 16] = Block(1000, 1000, true);
        }
    }



    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Gra");




    sf::RectangleShape gracz(sf::Vector2f(playerSizeX, playerSizeY));
    gracz.setPosition(100, 100);
    gracz.setFillColor(sf::Color(255, 255, 255, 128));




    {
        //Ramka
        Bloki[0] = Block(0, windowHeight, true); //Ground
        Bloki[1] = Block(100, windowHeight, true);
        Bloki[2] = Block(200, windowHeight, true);
        Bloki[3] = Block(300, windowHeight, true);

        Bloki[4] = Block(0, -100, true); // Top
        Bloki[5] = Block(100, -100, true);
        Bloki[6] = Block(200, -100, true);
        Bloki[7] = Block(300, -100, true);

        Bloki[8] = Block(-100, 0, true); // Left
        Bloki[9] = Block(-100, 100, true);
        Bloki[10] = Block(-100, 200, true);
        Bloki[11] = Block(-100, 300, true);

        Bloki[12] = Block(400, 0, true); // Right
        Bloki[13] = Block(400, 100, true);
        Bloki[14] = Block(400, 200, true);
        Bloki[15] = Block(400, 300, true);

    }


    while (window.isOpen())
    {
        auto frame_start = clock::now();





        change = false;



        if ((GetKeyState('A') & 0x8000) && (GetKeyState('W') & 0x8000))
        {
            change = true;
            gracz1.Accelerate(-1);
            gracz1.Jump();
            gracz1.UpdatePos();
            collision(gracz1, Bloki, n);
            gracz.setPosition(gracz1.X(), gracz1.Y());
        }

        else if ((GetKeyState('D') & 0x8000) && (GetKeyState('W') & 0x8000))
        {
            change = true;
            gracz1.Accelerate(1);
            gracz1.Jump();
            gracz1.UpdatePos();
            collision(gracz1, Bloki, n);
            gracz.setPosition(gracz1.X(), gracz1.Y());
        }

        else if (GetKeyState('A') & 0x8000)
        {
            gracz1.Accelerate(-1);
            change = true;
            gracz1.UpdatePos();
            collision(gracz1, Bloki, n);
            gracz.setPosition(gracz1.X(), gracz1.Y());
        }
        else if (GetKeyState('D') & 0x8000)
        {
            gracz1.Accelerate(1);
            change = true;
            gracz1.UpdatePos();
            collision(gracz1, Bloki, n);
            gracz.setPosition(gracz1.X(), gracz1.Y());

        }
        else if (GetKeyState('W') & 0x8000)
        {
            gracz1.Jump();
            change = true;
            gracz1.UpdatePos();
            collision(gracz1, Bloki, n);
            gracz.setPosition(gracz1.X(), gracz1.Y());

        }

        else if (GetKeyState('R') & 0x8000)
        {
            gracz1.ResetPlayer();
            change = true;
            gracz1.UpdatePos();
            collision(gracz1, Bloki, n);
            gracz.setPosition(gracz1.X(), gracz1.Y());

        }


        if (change || gracz1.UpdatePos())
        {

            gracz1.PrintState();
        }




        collision(gracz1, Bloki, n);

        gracz.setPosition(gracz1.X(), gracz1.Y());

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();



        for (int i = 0; i < n; i++) {
            sf::RectangleShape blockRect(sf::Vector2f(blockSizeX, blockSizeY));
            blockRect.setPosition(Bloki[i].X(), Bloki[i].Y());
            blockRect.setFillColor(Bloki[i].solid ? sf::Color::Blue : sf::Color::Red);
            window.draw(blockRect);
        }






        window.draw(gracz);
        window.display();


        auto frame_end = clock::now();
        auto frame_duration = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end - frame_start);



        /*
        if (frame_duration < frame_delay) {
            std::this_thread::sleep_for(frame_delay - frame_duration);
        }
        */

    }


    kot();
    return 0;
}

