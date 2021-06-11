#ifndef __BOARD_HPP__
#define __BOARD_HPP__

#include <SFML/Graphics.hpp>

#include <vector>
#include <exception>
#include "util.hpp"

namespace mtb{

template<unsigned R, unsigned C>
class Board: public sf::Drawable, public sf::Transformable{
    private:
        sf::VertexArray cell_borders[R*C];
        sf::VertexArray cell_content[R*C];
        bool draw_borders;
        int size;
    public:
        Board(int s=10){
            size=s;
            draw_borders = true;
            for(int r = 0; r < R; r++){
                for(int c = 0; c < C; c++){
                    auto tr = sf::Vector2f(r*size,c*size);
                    auto tl = sf::Vector2f(r*size,c*size+size);
                    auto bl = sf::Vector2f(r*size+size,c*size+size);
                    auto br = sf::Vector2f(r*size+size,c*size);
                    unsigned p = r*C+c;
                    
                    sf::VertexArray border(sf::LineStrip, 5);
                    
                    border[0].position = tr;
                    border[1].position = tl;
                    border[2].position = bl;
                    border[3].position = br;
                    border[4].position = tr;
                    cell_borders[p] = border;
                    
                    sf::VertexArray content(sf::Quads, 4);
                    content[0].position = tr;
                    content[1].position = tl;
                    content[2].position = bl;
                    content[3].position = br;
                    cell_content[p] = content;
                    msf::setColor(cell_content[p], sf::Color::Black);
                }
            }
        }
        void setBorders(bool b){
            draw_borders = b;
        }
        void cellColor(unsigned r, unsigned c, const sf::Color& col){
            if(r < R && c < C){
                msf::setColor(cell_content[r*C+c], col);
            }
        }
        void columnCellColor(unsigned c, const sf::Color& col){
            for(int r = 0; r < R; r++){
                cellColor(r,c,col);
            }
        }
        void rowCellColor(unsigned r, const sf::Color& col){
            for(int c = 0; c < R; c++){
                cellColor(r,c,col);
            }
        }
        void borderColor(unsigned c, unsigned r, const sf::Color& col){
            if(r < R && c < C){
                msf::setColor(cell_borders[r*C+c], col);
            }
        }
        void columnBorderColor(unsigned c, const sf::Color& col){
            for(int r = 0; r < R; r++){
                borderColor(r,c,col);
            }
        }
        void rowBorderCollor(unsigned r, const sf::Color& col){
            for(int c = 0; c < R; c++){
                borderColor(r,c,col);
            }
        }
        sf::Vector2i getCell(sf::Vector2f pos){
            try{
                auto rel = pos - getPosition();
                unsigned r = (unsigned)rel.y / size;
                unsigned c = (unsigned)rel.x / size;
                return sf::Vector2i(r,c);
            }catch(std::exception&){
                return sf::Vector2i(-1,-1);
            }
        }
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states)const{
            states.transform *= getTransform();
            for(int r = 0; r < R; r++){
                for(int c = 0; c < C; c++){
                    target.draw(cell_content[r*C+c], states);
                    if(draw_borders){
                        target.draw(cell_borders[r*C+c], states);
                    }
                }
            }
        }
};
    
}

#endif
