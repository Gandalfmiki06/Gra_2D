#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <chrono>
#include <thread>
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

/*
class Player: protected Base
{
protected:
    double speed;
    double jumpForce;
    double accel;




public:
    Player(double x, double y, double s, double j, double a)
    {
        cordX = x;
        cordY = y;
        speed = s;
        jumpForce = j;
        accel = a;

        cout << "Created player object at x = " << cordX << " y = " << cordY <<endl<< "With speed = " << speed << " jumpForce = " << jumpForce << " and acceleration = " << accel<<endl;
    }

    Player()
    {
        cout << "Created empty player object" << endl;
    }


    void PrintState()
    {
        cout << "Player is at x = " << cordX << " y = " << cordY << endl << "With speed = " << speed << " jumpForce = " << jumpForce << " and acceleration = " << accel << endl;
    }

    bool UpdatePos()
    {
        bool changed = false;
        if (speed > 1)
        {
            speed = 1;
        }
        else if (speed < -1)
        {
            speed = -1;
        }

        cordX += speed;

        if (speed > 0)
        {
            if ((speed - FRICTION) < 0)
            {
                speed = 0.0;
                changed = true;
            }
            else
            {
                speed -= FRICTION;
                changed = true;
            }
        }
        else if (speed < 0)
        {
            if ((speed + FRICTION) > 0)
            {
                speed = 0.0;
                changed = true;
            }
            else
            {
                speed += FRICTION;
                changed = true;
            }
        }

        if (playerSizeY + cordY + GRAVITY < windowHeight)
        {
            cordY += GRAVITY;
            changed = true;
        }
        else if (cordY < GRAVITY && cordY != 0)
        {
            cordY = 0.0;
            changed = true;
        }



        return changed;
    }

    void Accelerate(int x)
    {
        if (x == 1)
        {
            speed = speed + accel;
        }

        else if (x == -1)     
        {
            speed = speed - accel;
        }
        else
        {
            cout << "Accelerate shouldnt be called now" << endl;
        }
    }

    void Jump()
    {
        cordY -= jumpForce;
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
        cordY = 0.;
        speed = 0;
        jumpForce = 0.1;
        accel = 0.01;
    }

    void SetX(double newX)
    {
        cordX = newX;
        speed = 0;
    }
    
    void SetY(double newY)
    {
        cordY = newY;
    }
    
    
    
    
    
    ~Player()
    {
        cout << "Player object destroyed" << endl;
    }





    friend void collision(Player& gracz, Block* bloki, int n);
};
*/

class Player : protected Base
{
protected:
    double speed;      // horizontal velocity
    double vertSpeed=0.0;  // vertical velocity
    double jumpForce;
    double accel;
    bool grounded;

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

        if (!grounded) // only apply gravity if not on ground
        {
            vertSpeed += GRAVITY;
            cordY += vertSpeed;
            changed = true;
        }
        else
        {
            vertSpeed = 0; // reset vertical speed when grounded
        }

        // Prevent falling below window bottom
        if (cordY + playerSizeY > windowHeight)
        {
            cordY = windowHeight - playerSizeY;
            vertSpeed = 0;
            grounded = true;
            changed = true;
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
        // Only jump if standing on something (vertSpeed == 0)
        if (vertSpeed == 0)
            vertSpeed = -jumpForce;
    }

    double X() { return cordX; }
    double Y() { return cordY; }
    double X2() { return cordX + playerSizeX; }
    double Y2() { return cordY + playerSizeY; } // bottom = top + height

    void ResetPlayer()
    {
        cordX = 0.;
        cordY = 0.;
        speed = 0;
        vertSpeed = 0;
        jumpForce = 0.1;
        accel = 0.01;
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
protected:
    bool solid;
public:

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





int main()
{


    using clock = std::chrono::steady_clock;


    //x,y,speed,jump,accel
    Player gracz1(0, 200, 0, 0.5, 0.01);
    bool change;
   
    int n = 16;
    Block* Bloki;
    Bloki = (Block*)malloc(sizeof(Block) *n);


    for (int i = 0; i < n-2; i++) // Dwa ostatnie block tylko chce testowaæ na ten moment
    {
        Bloki[i] = Block(1000, 1000, false);
    }

    Bloki[14] = Block(100, 200, true);
    Bloki[15] = Block(200, 300, false);


    sf::RectangleShape TrueTest((sf::Vector2f(blockSizeX, blockSizeY)));
    sf::RectangleShape FalseTest((sf::Vector2f(blockSizeX, blockSizeY)));

    TrueTest.setFillColor(sf::Color::Blue);
    FalseTest.setFillColor(sf::Color::Red);


    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML works!");

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);


    sf::RectangleShape gracz(sf::Vector2f(playerSizeX, playerSizeY));
    gracz.setPosition(100, 100);
    gracz.setFillColor(sf::Color::White);


    //
    TrueTest.setPosition(Bloki[14].X(), Bloki[14].Y());
    FalseTest.setPosition(Bloki[15].X(), Bloki[15].Y());
    //

    sf::RectangleShape debugHitbox(sf::Vector2f(playerSizeX, playerSizeY));
    debugHitbox.setPosition(gracz1.X(), gracz1.Y());
    debugHitbox.setFillColor(sf::Color(255, 255, 255, 100)); // semi-transparent



    while (window.isOpen())
    {
        auto frame_start = clock::now();

        



        change = false;
        if (GetKeyState('A') & 0x8000)
        {
            gracz1.Accelerate(-1);
            change = true;
            gracz1.UpdatePos();
            collision(gracz1, Bloki, n);
            gracz.setPosition(gracz1.X(), gracz1.Y());
        }
        if (GetKeyState('D') & 0x8000)
        {
            gracz1.Accelerate(1);
            change = true;
            gracz1.UpdatePos();
            collision(gracz1, Bloki, n);
            gracz.setPosition(gracz1.X(), gracz1.Y());

        }
        if (GetKeyState('W') & 0x8000)
        {
            gracz1.Jump();
            change = true;
            gracz1.UpdatePos();
            collision(gracz1, Bloki, n);
            gracz.setPosition(gracz1.X(), gracz1.Y());

        }
        
        if (GetKeyState('R') & 0x8000)
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
        window.draw(shape);
        window.draw(gracz);
        window.draw(TrueTest);
        window.draw(FalseTest);

        debugHitbox.setPosition(gracz1.X(), gracz1.Y());
        window.draw(debugHitbox);


        window.display();


        auto frame_end = clock::now();
        auto frame_duration = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end - frame_start);
        
        
        
        /*
        if (frame_duration < frame_delay) {
            std::this_thread::sleep_for(frame_delay - frame_duration);
        }
        */

    }
    
    return 0;
}

