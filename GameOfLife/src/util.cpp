#include "util.hpp"

namespace msf{
    void setColor(sf::VertexArray& va, const sf::Color& c){
        auto n = va.getVertexCount();
        for(int i = 0; i < n; i++){
            va[i].color = c;
        }
    }
}
