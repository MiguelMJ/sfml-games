#include <iostream>
#include <sstream>
#include <random>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "GameOfLife.hpp"

sf::RenderWindow win;

int main(int argc, char **argv){
    // constants
    const unsigned R = 50;
    const unsigned C = 50;
    const unsigned n = 5;
    // window and view
    win.create(sf::VideoMode(800,600), "TBM",sf::Style::Titlebar | sf::Style::Close);
    win.setFramerateLimit(45);
    
    sf::View view(sf::FloatRect(-1,-1,C*n+2,R*n+2));
    sf::View viewInfo(sf::FloatRect(1000,0,220,800));
    
    
    view.setViewport(sf::FloatRect(0,0,600.0/800,1));
    viewInfo.setViewport(sf::FloatRect(600.0/800,0,200.0/800,1));
    
    
    // Board
    mtb::GameOfLife<R,C> board(n);
    board.setWindowHandler(&win);
    // Clock
    sf::Clock clock;
    sf::Time wait = sf::milliseconds(500);
    // Text
    sf::Font font;
    if(!font.loadFromFile("fonts/GOTHIC.TTF")){
        exit(1);
    }
    sf::Text info;
    info.setFont(font);
    info.setPosition(1010,10);
    info.setCharacterSize(24);
    
    sf::Text info2;
    info2.setFont(font);
    info2.setPosition(1010,100);
    info2.setCharacterSize(20);
    info2.setString("[+] Speed up\n[-] Speed down\n[Enter] Pause/Play\
    \n[C] Clear\n[R] Random\n\n[Left Click] Paint\n[Left Click] Erase");
    
    sf::Text promo;
    promo.setFont(font);
    promo.setPosition(1010,780);
    promo.setCharacterSize(10);
    promo.setString("github.com/MiguelMJ/GameOfLife");
    
    
    while(win.isOpen()){
        sf::Event e;
        while(win.pollEvent(e)){
            switch(e.type){
                case sf::Event::Closed:
                    win.close();
                    break;
                case sf::Event::KeyPressed:
                    switch(e.key.code){
                    case sf::Keyboard::Key::Add:
                        board.setPause(board.getPause()-50);
                        break;
                    case sf::Keyboard::Key::Subtract:
                        board.setPause(board.getPause()+50);
                        break;
                    }
                    break;
            }
            board.handle(e);
        }
        
        std::stringstream ss;
        if(board.isOn()){
            ss <<  "Running\n";
        }else{
            ss << "Paused\n";
        }
        ss << board.getPause() << " ms";
        info.setString(ss.str());
        
        unsigned dt = clock.restart().asMilliseconds();
        board.update(dt);
        win.clear();
        win.setView(viewInfo);
        win.draw(info);
        win.draw(info2);
        win.draw(promo);
        win.setView(view);
        win.draw(board);
        win.display();
    }
    
    return 0;
}
