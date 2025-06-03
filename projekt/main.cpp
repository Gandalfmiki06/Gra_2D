#include "SFML_TEST3.h"

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

    return 0;
}

