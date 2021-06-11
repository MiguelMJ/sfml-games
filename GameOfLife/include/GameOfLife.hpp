#ifndef __GAME_OF_LIFE_HPP__
#define __GAME_OF_LIFE_HPP__

#include "Board.hpp"
#include <SFML/Window.hpp>

namespace mtb{
    template<unsigned R, unsigned C>
class GameOfLife: public mtb::Board<R,C>{
    private:
        typedef enum {BIRTH, LIVE, DYING, DEAD} State;
        State alive[R+2][C+2];
        std::random_device rd;
        sf::RenderWindow* win_handler;
        unsigned stacked_dt;
        int pause;
        bool running;
        bool painting;
        bool erasing;
        void endUpdate(){
            for(int r=1; r<=R; r++){
                for(int c=1; c<=C; c++){
                    State s = alive[r][c];
                    if(s == DYING){
                        alive[r][c] = DEAD;
                    }else if(s == BIRTH){
                        alive[r][c] = LIVE;
                    }
                }
            }
        }
        unsigned getNeighbourCount(unsigned rb, unsigned cb){
            unsigned count = 0;
            for(int c=0; c < 3; c++){
                for(int r=0; r <= 2; r+=2){
                    State s = alive[rb+r][cb+c];
                    count += s == LIVE || s == DYING ? 1 : 0 ;
                }
            }
            State s = alive[rb+1][cb];
            count += s == LIVE || s == DYING ? 1 : 0 ;
            s = alive[rb+1][cb+2];
            count += s == LIVE || s == DYING ? 1 : 0 ;
            return count;
        }
        void set(unsigned r, unsigned c, State s){
            if(r < R && c < C){
                auto color = s == BIRTH ? sf::Color::Red : sf::Color::Black;
                alive[1+r][c+1] = s;
                mtb::Board<R,C>::cellColor(r,c,color);
            }
        }
        void update(unsigned r, unsigned c){
            State s = alive[r+1][c+1];
            unsigned n = getNeighbourCount(r,c);
            if(s == LIVE){
                switch(n){
                    case 2:
                    case 3:
                        // survives
                        break;
                    default:
                        set(r,c,DYING);
                }
            }else if(n == 3){
                set(r,c,BIRTH);
            }
        }
    public:
        GameOfLife(int n=10) : mtb::Board<R,C>(n) {
            pause = 50;
            running=false;
            painting=false;
            erasing=false;
            for(int r=0; r < R+2; r++){
                for(int c=0; c < C+2; c++){
                    alive[r][c] = DEAD;
                }
            }
        }
        void setWindowHandler(sf::RenderWindow* w){
            win_handler=w;
        }
        void reset(){
            running = false;
            for(int r=0; r < R; r++){
                for(int c=0; c < C; c++){
                    set(r,c,DEAD);
                }
            }
        }
        void random(){
            if(!running){
                for(int r=0; r < R; r++){
                    for(int c=0; c<C; c++){
                        set(r,c,rd()%2==0?BIRTH:DEAD);
                    }
                }
            }
        }
        int getPause(){
            return pause;
        }
        void setPause(int p){
            pause = p < 5 ? 5 : p;
            pause = pause > 2000 ? 2000 : pause;
        }
        bool isOn(){return running;}
        void handle(const sf::Event& e){
            switch(e.type){
                case sf::Event::MouseButtonPressed:
                    if(!running){
                        if(e.mouseButton.button == sf::Mouse::Left){
                            auto p = sf::Mouse::getPosition(*win_handler);
                            auto co = win_handler->mapPixelToCoords(p);
                            auto c = mtb::Board<R,C>::getCell(co);
                            set(c.y,c.x,BIRTH);
                            painting = true;
                            erasing = false;
                        }else{
                            painting = false;
                            erasing = true;
                        }
                    }
                    break;
                case sf::Event::MouseButtonReleased:
                    erasing = false;
                    painting = false;
                    break;
                case sf::Event::MouseMoved:
                    if(painting || erasing){
                        auto p = sf::Mouse::getPosition(*win_handler);
                        auto co = win_handler->mapPixelToCoords(p);
                        auto c = mtb::Board<R,C>::getCell(co);
                        if(painting)
                            set(c.y,c.x,BIRTH);
                        else
                            set(c.y,c.x,DEAD);
                    }
                    break;
                case sf::Event::KeyPressed:
                    if(e.key.code == sf::Keyboard::Enter){
                        running = !running;
                    }else if(e.key.code == sf::Keyboard::C){
                        reset();
                    }else if(e.key.code == sf::Keyboard::R){
                        random();
                    }
                    break;
            }
        }
        void update(unsigned dt){
            stacked_dt += dt;
            if(stacked_dt > pause && running){
                stacked_dt %= pause;
                for(int r = 0; r < R; r++){
                    for(int c = 0; c < C; c++){
                        update(r,c);
                    }
                }
                endUpdate();
            }
        }
        
};
}

#endif
