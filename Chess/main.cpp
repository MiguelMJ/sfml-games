#include <iostream>
#include <sstream>
#include <list>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "Chess.hpp"


typedef struct{
    std::string msg;
    int ms;
} Aviso;
typedef struct {
    std::list<Aviso> lista;
    void update(int ms){
        auto i = lista.begin();
        while(i != lista.end()){
            i->ms -= ms;
            if(i->ms < 0){
                i = lista.erase(i);
            }else{
                i++;
            }
        }
    }
    void avisar(const std::string& str, unsigned m=1000){
        Aviso a;
        a.msg = str; a.ms = m;
        lista.push_back(a);
    }
    std::string to_string()const{
        std::stringstream ss;
        for(auto a : lista){
            ss << a.msg << "\n";
        }
        return ss.str();
    }
} ListaAvisos;

int main(int argc, char **argv){
    // pixels per cell
    const unsigned n = 30;
    // setup window
    sf::RenderWindow win;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    win.create(sf::VideoMode(800,600), "Chess",sf::Style::Titlebar | sf::Style::Close, settings);
    win.setFramerateLimit(45);
    // setup views
    sf::View view(sf::FloatRect(-1,-1,8*n+2,8*n+2));
    sf::View viewInfo(sf::FloatRect(1000,0,220,800));
    view.setViewport(sf::FloatRect(0,0,600.0/800,1));
    viewInfo.setViewport(sf::FloatRect(600.0/800,0,200.0/800,1));
    // clock and fonts
    sf::Clock clock;
    sf::Font font;
    if(!font.loadFromFile("assets/monoid.normal.ttf")){
        exit(1);
    }
    sf::Text info;
    info.setFont(font);
    info.setPosition(1010,10);
    info.setCharacterSize(20);
    sf::Text address;
    address.setFont(font);
    address.setPosition(1010,750);
    address.setCharacterSize(14);
    address.setString("github.com/MiguelMJ/Chess");
    
    ListaAvisos l_avisos;
    sf::Text avisos;
    avisos.setFont(font);
    avisos.setPosition(1010,750);
    avisos.setCharacterSize(16);
    // main body
    Juego juego(win, view, n);
    while(win.isOpen()){
        sf::Event e;
        // Manejo de eventos
        while(win.pollEvent(e)){
            switch(e.type){
                case sf::Event::Closed:
                    win.close();
                    break;
                default:
                    juego.manejar(e);
                    break;
            }
        }
        // fin del manejo de eventos
        
        juego.actualizar();
        
        unsigned dt = clock.restart().asMilliseconds();
        
        avisos.setString(l_avisos.to_string());
        l_avisos.update(dt);
        info.setString(juego.getInfo());
        
        win.clear();
        win.setView(viewInfo);
        win.draw(info);
        win.draw(address);
        win.draw(avisos);
        win.setView(view);
        win.draw(*(juego.tablero));
        win.display();
    }
    return 0;
}
