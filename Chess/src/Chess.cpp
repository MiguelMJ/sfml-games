#include "Chess.hpp"

Equipo operator ! (Equipo e){
    return e == BLANCAS ? NEGRAS : BLANCAS;
}
std::string equipo2str(Equipo e){
    return e == BLANCAS ? "BLANCAS" : "NEGRAS";
}
std::string modo2str(Modo m){
    switch(m){
        case LOCAL:
            return "PARTIDA LOCAL";
        case IA:
            return "CONTRA IA";
        case HOST:
        case ONLINE:
            return "ONLINE";
    }
}
std::string Posicion_st::to_string(){
    if(validar()){
        return std::to_string(r) + std::to_string(c);
    }else{
        return "";
    }
}
void Posicion_st::from_string(const std::string& str){   
    r = str[0] - '0';
    c = str[1] - '0';
}
Posicion_st::Posicion_st(){
    r=-1;c=-1;
}
Posicion_st::Posicion_st(const std::string& str){
    from_string(str);
}
Posicion_st::Posicion_st(unsigned rr, unsigned cc){
    r = rr;
    c = cc;
}
bool Posicion_st::validar(){
    return r < 8 && c < 8;
}
Movimiento_st::Movimiento_st(const Movimiento_st& m){
    origen=m.origen;
    destino=m.destino;
}
std::string Movimiento_st::to_string(){
    return origen.to_string() + destino.to_string();
}
Movimiento_st::Movimiento_st(const std::string& str):
    origen(str.substr(0,2)), 
    destino(str.substr(2,2)) {}
Movimiento_st::Movimiento_st(Posicion o, Posicion d):
    origen(o.r, o.c),
    destino(d.r, d.c)
    {}
bool Movimiento_st::validar(){
    return origen.validar() && destino.validar();
}
sf::Texture Pieza_st::texture;
std::string Pieza_st::to_string(){
    static const std::map<TipoPieza, std::string> nombreTipo = {
        {PEON,"PEON"},
        {TORRE,"TORRE"},
        {CABALLO,"CABALLO"},
        {ALFIL,"ALFIL"},
        {REINA,"REINA"},
        {REY,"REY"}
    };
    return nombreTipo.at(tipo) + " " + equipo2str(equipo);
}

Pieza_st::Pieza_st_(TipoPieza t, Equipo e, Posicion p, bool j):
    posicion(p.r, p.c){
    tipo = t;
    equipo = e;
    jugando=j;
    unsigned top, left;
    auto texSize = texture.getSize();
    top = equipo == BLANCAS ? 0 : texSize.y/2;
    switch(tipo){
        case PEON:
            left = 5;
            break;
        case TORRE:
            left = 4;
            break;
        case CABALLO:
            left = 3;
            break;
        case ALFIL:
            left = 2;
            break;
        case REINA:
            left = 1;
            break;
        case REY:
            left = 0;
            break;
    }
    int w = texSize.x/6;
    left *= w;
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(left,top,texSize.x/6,texSize.y/2));
    // std::cout << "Creada ficha: " << to_string() << std::endl;
}
Evento::Evento_st(Movimiento m, Pieza p):
    movimiento(m),
    comida(p){}
bool TableroAjedrez::insertarSiValido(ListaMovimientos &l, Movimiento m){
    bool dummy=false;
    return insertarSiValido(l, m, dummy);
}
bool TableroAjedrez::insertarSiValido(ListaMovimientos &l, Movimiento m, bool& ha_comido_antes){
    bool valido = false;
    if(m.validar() && !ha_comido_antes){
        Pieza p = piezaEn(m.origen);
        Pieza d = piezaEn(m.destino);
        bool come = p != nullptr && 
                    d != nullptr && 
                    d->equipo != p->equipo;
        /*if(come){
            std::cout << p->to_string() << " come " << d->to_string() << std::endl;
        }*/
        valido = p != nullptr && (d == nullptr || come);
        ha_comido_antes = ha_comido_antes || come;
        if(valido){
            l.push_back(m);
        }
    }
    return valido;
}

void TableroAjedrez::draw(sf::RenderTarget& target, sf::RenderStates states)const{
    states.transform *= getTransform();
    for(int i=0; i < 64; i++){
        target.draw(celdas[i], states);
    }
    for(auto p : blancas){
        if(p->jugando){
            target.draw(p->sprite, states);   
        }
    }
    for(auto p : negras){
        if(p->jugando){
            target.draw(p->sprite, states);   
        }
    }
    for(auto m : marcas){
        target.draw(m, states);
    }
}
void TableroAjedrez::situarSprites(){
    std::vector<std::vector<Pieza>*> eq = {&blancas, &negras};
    for(auto pl : eq){
        for(auto p : *pl){
            poner(p->posicion, p);
            auto y = (float)lado * p->posicion.r + lado*0.1;
            auto x = (float)lado * p->posicion.c + lado*0.1;
            p->sprite.setPosition(x,y);
            auto lb = p->sprite.getLocalBounds();
            sf::Vector2f sc(lb.width, lb.height);
            float tam = (float)lado*0.8;
            sf::Vector2f fac(tam/sc.x, tam/sc.y);
            p->sprite.scale(fac);
        }
    }
}

TableroAjedrez::TableroAjedrez(int size){
    blancas.push_back(Pieza(new Pieza_st(PEON, BLANCAS, Posicion(6,0))));
    blancas.push_back(Pieza(new Pieza_st(PEON, BLANCAS, Posicion(6,1))));
    blancas.push_back(Pieza(new Pieza_st(PEON, BLANCAS, Posicion(6,2))));
    blancas.push_back(Pieza(new Pieza_st(PEON, BLANCAS, Posicion(6,3))));
    blancas.push_back(Pieza(new Pieza_st(PEON, BLANCAS, Posicion(6,4))));
    blancas.push_back(Pieza(new Pieza_st(PEON, BLANCAS, Posicion(6,5))));
    blancas.push_back(Pieza(new Pieza_st(PEON, BLANCAS, Posicion(6,6))));
    blancas.push_back(Pieza(new Pieza_st(PEON, BLANCAS, Posicion(6,7))));
    blancas.push_back(Pieza(new Pieza_st(TORRE, BLANCAS, Posicion(7,0))));
    blancas.push_back(Pieza(new Pieza_st(TORRE, BLANCAS, Posicion(7,7))));
    blancas.push_back(Pieza(new Pieza_st(CABALLO, BLANCAS, Posicion(7,1))));
    blancas.push_back(Pieza(new Pieza_st(CABALLO, BLANCAS, Posicion(7,6))));
    blancas.push_back(Pieza(new Pieza_st(ALFIL, BLANCAS, Posicion(7,2))));
    blancas.push_back(Pieza(new Pieza_st(ALFIL, BLANCAS, Posicion(7,5))));
    blancas.push_back(Pieza(new Pieza_st(REINA, BLANCAS, Posicion(7,3))));
    blancas.push_back(Pieza(new Pieza_st(REY, BLANCAS, Posicion(7,4))));
    
    negras.push_back(Pieza(new Pieza_st(PEON, NEGRAS, Posicion(1,0))));
    negras.push_back(Pieza(new Pieza_st(PEON, NEGRAS, Posicion(1,1))));
    negras.push_back(Pieza(new Pieza_st(PEON, NEGRAS, Posicion(1,2))));
    negras.push_back(Pieza(new Pieza_st(PEON, NEGRAS, Posicion(1,3))));
    negras.push_back(Pieza(new Pieza_st(PEON, NEGRAS, Posicion(1,4))));
    negras.push_back(Pieza(new Pieza_st(PEON, NEGRAS, Posicion(1,5))));
    negras.push_back(Pieza(new Pieza_st(PEON, NEGRAS, Posicion(1,6))));
    negras.push_back(Pieza(new Pieza_st(PEON, NEGRAS, Posicion(1,7))));
    negras.push_back(Pieza(new Pieza_st(TORRE, NEGRAS, Posicion(0,0))));
    negras.push_back(Pieza(new Pieza_st(TORRE, NEGRAS, Posicion(0,7))));
    negras.push_back(Pieza(new Pieza_st(CABALLO, NEGRAS, Posicion(0,1))));
    negras.push_back(Pieza(new Pieza_st(CABALLO, NEGRAS, Posicion(0,6))));
    negras.push_back(Pieza(new Pieza_st(ALFIL, NEGRAS, Posicion(0,2))));
    negras.push_back(Pieza(new Pieza_st(ALFIL, NEGRAS, Posicion(0,5))));
    negras.push_back(Pieza(new Pieza_st(REINA, NEGRAS, Posicion(0,3))));
    negras.push_back(Pieza(new Pieza_st(REY, NEGRAS, Posicion(0,4))));
    
    lado = size;
    situarSprites();
    
    sf::Color color1(204, 102, 0);
    sf::Color color2(255, 204, 102);
    for(int r = 0; r < 8; r++){
        for(int c = 0; c < 8; c++){
            sf::RectangleShape celda(sf::Vector2f(lado,lado));
            celda.setPosition(getPosition()+sf::Vector2f(lado*r,lado*c));
            celda.setFillColor(
                (r+c) % 2 == 0 ?
                color1 : color2
            );
            celdas[r*8+c] = celda;
        }
    }
}

void TableroAjedrez::mover(Movimiento m){
    Evento e(m, piezaEn(m.destino));
    if(e.comida != nullptr){
        e.comida->jugando = false;
    }
    
    auto p = piezaEn(m.origen);
    poner(m.origen, nullptr);
    poner(m.destino, p);
    p->posicion = m.destino;
    historial.push_front(e);
}
void TableroAjedrez::deshacer(){
    if(!historial.empty()){
        auto e = historial.front();
        poner(e.movimiento.origen, piezaEn(e.movimiento.destino));
        poner(e.movimiento.destino, e.comida);
        if(e.comida != nullptr){
            e.comida->jugando = true;
        }
            
        historial.pop_front();
    }
}
bool TableroAjedrez::soloProbar(Movimiento m, Equipo e){
    bool ret = false;
    if(m.validar()){
        mover(m);
        ret = jaque(e);
        deshacer();
    }
    return ret;
}
bool TableroAjedrez::probar(Movimiento m, Equipo e){
    bool contemplado = false;
    bool produceJaque = false;
    if(m.validar()){
        auto p = piezaEn(m.origen);
        if(p != nullptr && p->equipo == e){
            auto pm = posiblesMovimientos(p);
            auto it = pm.begin();
            while(!contemplado && it != pm.end()){
                contemplado = it->destino.r == m.destino.r &&
                              it->destino.c == m.destino.c;
                it++;
            }
        }
    }
    if(contemplado){
        produceJaque = soloProbar(m, e);
    }
    return contemplado && !produceJaque;
}

void TableroAjedrez::poner(Posicion p, Pieza f){
    tablero[p.r][p.c] = f;
    if(f != nullptr){
        f->posicion = p;
        auto pos = getPosition() + sf::Vector2f(p.c * lado + lado*0.1 , p.r * lado + lado * 0.1);
        f->sprite.setPosition(pos);
    }
}

Pieza TableroAjedrez::piezaEn(Posicion p){
    return tablero[p.r][p.c];
}

ListaMovimientos TableroAjedrez::posiblesMovimientos(Pieza p){
    ListaMovimientos ret;
    if(p != nullptr){
        Posicion pos = p->posicion;
        Posicion dest;
        Pieza paux;
        bool comio;
        switch(p->tipo){
            case PEON:
                unsigned dr, start;
                if(p->equipo == NEGRAS){
                    start = 1;
                    dr = 1;
                }else{
                    start = 6;
                    dr = -1;
                }
                // attack
                dest.r = pos.r + dr;
                for(dest.c = pos.c-1; dest.c <= pos.c+1; dest.c += 2){
                    if(dest.validar()){
                        paux = piezaEn(dest);
                        if(paux != nullptr && paux->equipo != p->equipo){
                            ret.push_back(Movimiento(pos, dest));
                        }
                    }
                }
                // normal move
                dest.r = pos.r+dr; dest.c = pos.c;
                if(dest.validar() && piezaEn(dest) == nullptr){
                    ret.push_back(Movimiento(pos, dest));
                    // starting move
                    dest.r += dr;
                    if( dest.validar() && pos.r == start && 
                        piezaEn(dest) == nullptr){
                        ret.push_back(Movimiento(pos, dest));
                    }
                    dest.r -= dr;
                }
                break;
            case REINA:
            case TORRE:
                // arriba
                dest.r = pos.r-1; dest.c = pos.c;
                comio=false;
                while(insertarSiValido(ret, Movimiento(pos,dest),comio)){
                    dest.r--;
                }
                // abajo
                dest.r = pos.r+1;
                comio=false;
                while(insertarSiValido(ret, Movimiento(pos,dest), comio)){
                    dest.r++;
                }
                // izquierda
                dest.r = pos.r; dest.c = pos.c-1;
                comio=false;
                while(insertarSiValido(ret, Movimiento(pos,dest), comio)){
                    dest.c--;
                }
                // derecha
                dest.c = pos.c+1;
                comio=false;
                while(insertarSiValido(ret, Movimiento(pos,dest), comio)){
                    dest.c++;
                }
                if (p->tipo == TORRE){
                    break;
                }
            case ALFIL:
                // arriba izquierda
                dest.r = pos.r-1; dest.c = pos.c-1;
                comio=false;
                while(insertarSiValido(ret, Movimiento(pos, dest), comio)){
                    dest.r--;
                    dest.c--;
                }
                // arriba derecha
                dest.r = pos.r-1; dest.c = pos.c+1;
                comio=false;
                while(insertarSiValido(ret, Movimiento(pos, dest), comio)){
                    dest.r--;
                    dest.c++;
                }
                // abajo izquierda
                dest.r = pos.r+1; dest.c = pos.c-1;
                comio=false;
                while(insertarSiValido(ret, Movimiento(pos, dest), comio)){
                    dest.r++;
                    dest.c--;
                }
                // abajo derecha
                dest.r = pos.r+1; dest.c = pos.c+1;
                comio=false;
                while(insertarSiValido(ret, Movimiento(pos, dest), comio)){
                    dest.r++;
                    dest.c++;
                }
                break;
            case CABALLO:
                dest.r = pos.r+2; dest.c = pos.c+1;
                insertarSiValido(ret, Movimiento(pos, dest));
                dest.r = pos.r+2; dest.c = pos.c-1;
                insertarSiValido(ret, Movimiento(pos, dest));
                dest.r = pos.r-2; dest.c = pos.c+1;
                insertarSiValido(ret, Movimiento(pos, dest));
                dest.r = pos.r-2; dest.c = pos.c-1;
                insertarSiValido(ret, Movimiento(pos, dest));
                dest.r = pos.r+1; dest.c = pos.c+2;
                insertarSiValido(ret, Movimiento(pos, dest));
                dest.r = pos.r+1; dest.c = pos.c-2;
                insertarSiValido(ret, Movimiento(pos, dest));
                dest.r = pos.r-1; dest.c = pos.c+2;
                insertarSiValido(ret, Movimiento(pos, dest));
                dest.r = pos.r-1; dest.c = pos.c-2;
                insertarSiValido(ret, Movimiento(pos, dest));
                break;
            case REY:
                dest.r = pos.r+1; dest.c = pos.c+1;
                insertarSiValido(ret, Movimiento(pos, dest));
                dest.r = pos.r+1; dest.c = pos.c;
                insertarSiValido(ret, Movimiento(pos, dest));
                dest.r = pos.r+1; dest.c = pos.c-1;
                insertarSiValido(ret, Movimiento(pos, dest));
                dest.r = pos.r; dest.c = pos.c+1;
                insertarSiValido(ret, Movimiento(pos, dest));
                dest.r = pos.r; dest.c = pos.c-1;
                insertarSiValido(ret, Movimiento(pos, dest));
                dest.r = pos.r-1; dest.c = pos.c+1;
                insertarSiValido(ret, Movimiento(pos, dest));
                dest.r = pos.r-1; dest.c = pos.c;
                insertarSiValido(ret, Movimiento(pos, dest));
                dest.r = pos.r-1; dest.c = pos.c-1;
                insertarSiValido(ret, Movimiento(pos, dest));
                break;
        }
        // std::cout << "almacenado movimiento desde " << pos.to_string() << std::endl;
    }
    return ret;
}

ListaMovimientos TableroAjedrez::posiblesMovimientos(Equipo e){
    const std::vector<Pieza> &equipo = e == BLANCAS ? blancas : negras;
    ListaMovimientos ret;
    for(auto p : equipo){
        if(p->jugando){
            auto pm = posiblesMovimientos(p);
            ret.insert(ret.end(),pm.begin(), pm.end());
        }
    }
    return ret;
}

bool TableroAjedrez::jaque(Equipo e){
    bool j = false;
    const std::vector<Pieza> *equipo, *contrincante;
    // determinamos los equipos
    if(e == BLANCAS){
        equipo = &blancas;
        contrincante = &negras;
    }else{
        equipo = &negras;
        contrincante = &blancas;
    }
    // encontramos la posicion del rey
    Posicion reypos;
    for(auto p : *equipo){
        if(p->tipo == REY){
            reypos = p->posicion;
            break;
        }
    }
    // comprobamos si es atacable
    for(auto p: *contrincante){
        if(p->jugando){
            auto pm = posiblesMovimientos(p);
            for(auto mov : pm){
                j = mov.destino.r == reypos.r &&
                    mov.destino.c == reypos.c;
                if(j) break;
            }
            if(j) break;
        }
    }
    return j;
}
bool TableroAjedrez::jaquemate(Equipo e){
    bool jm=true;
    auto pm = posiblesMovimientos(e);
    auto mov = pm.begin();
    while(mov != pm.end() && jm){
        jm = soloProbar(*mov, e);
        mov++;
    }
    return jm;
}

std::string TableroAjedrez::getInfo(Posicion pos){
    std::string ret;
    if(pos.validar()){
        auto p = piezaEn(pos);
        if(p != nullptr){
            ret = p->to_string();
        }else{
            ret = "-";
        }
    }
    return ret;
}
Posicion TableroAjedrez::coord2pos(sf::Vector2f coo){
    Posicion ret;
    sf::Vector2f relPos = coo - getPosition();
    ret.r = relPos.y / lado;
    ret.c = relPos.x / lado;
    if(!ret.validar()){
        ret.r = FUERA; ret.c = FUERA;
    }/*else if(volteado){
        ret.r = 7 - ret.r;
        ret.c = 7 - ret.c;
    }*/
    return ret;
}

void TableroAjedrez::marcar(Posicion p, sf::Color c, float thickness){
    if(p.validar()){
        float grosor = (float)lado * thickness;
        float top = p.r * lado + grosor;
        float left = p.c * lado + grosor;
        float height = lado - 2 * grosor;
        float width = lado - 2 * grosor;
        sf::RectangleShape rect(sf::Vector2f(width, height));
        rect.setPosition(getPosition() + sf::Vector2f(left, top));
        rect.setFillColor(sf::Color(0,0,0,0));
        rect.setOutlineColor(c);
        rect.setOutlineThickness(grosor);
        marcas.push_back(rect);
    }
}

void TableroAjedrez::borrarMarcas(){
    marcas.clear();
}
void TableroAjedrez::voltear(){
    // volteado = !volteado;
    // auto mitadT = sf::Vector2f(lado*4, lado*4);
    float mc = lado*0.8/2;
    auto mitadP = sf::Vector2f(mc, mc);
    // rotate(180);
    // setOrigin(mitadT + mitadT - getOrigin());
    auto l = sf::Vector2f(lado*1.7,lado*1.8);
    for(auto p : blancas){
        auto spr = &(p->sprite);
        spr->rotate(180);
        spr->setOrigin(-spr->getOrigin()+l);
    }
    for (auto p : negras){
        auto spr = &(p->sprite);
        spr->rotate(180);
        spr->setOrigin(-spr->getOrigin()+l);
    }
}

Movimiento Jugador::jugada(){
    Movimiento ret(sigMov->origen, sigMov->destino);
    sigMov = nullptr;
    return ret;
};

bool JugadorCliente::listo(){
    sigMov != nullptr;
}
void JugadorCliente::notificar(Movimiento m){}
void JugadorCliente::preparar(Movimiento m){
    sigMov.reset(new Movimiento(m.origen,m.destino));
}
std::random_device rd;
bool JugadorIA::listo(){
    auto pm = tablero->posiblesMovimientos(equipo);
    auto azar = pm[rd() % pm.size()];
    sigMov.reset(new Movimiento(azar.origen, azar.destino));
    return true;
}
void JugadorIA::notificar(Movimiento m){
    // nothing
}
JugadorOnline::JugadorOnline(sf::TcpSocket &s){
    socket = &s;
    socket_sel.add(s);
}
bool JugadorOnline::listo(){
    bool ret=false;
    if(socket_sel.wait(sf::milliseconds(10)) && socket_sel.isReady(*socket)){
        sf::Packet packet;
        if (socket->receive(packet) != sf::Socket::Done){
            std::cerr << "Error al recibir paquete" << std::endl;
            std::cerr << "Es posible que el otro jugador se haya desconectado";
        }else{
            std::string msg;
            packet >> msg;
            sigMov.reset(new Movimiento(msg));
            ret=true;
        }
    }
    return ret;
}
void JugadorOnline::notificar(Movimiento m){
    sf::Packet packet;
    packet << m.to_string();
    auto st = socket->send(packet);
    while(st == sf::Socket::Partial){
        st = socket->send(packet);
    }
    if(st != sf::Socket::Done){
        std::cerr << "No se pudo notificar" << std::endl;
    }
}

void Juego::girar(){
    tablero->voltear();
    camara->rotate(180);
}
void Juego::cambiaTurno(){
    if(modo == LOCAL){
        local = std::static_pointer_cast<JugadorCliente>(contrincante);
        contrincante = turno;
        turno = local;
        girar();
    }else{
        if(turno == local){
            turno = contrincante;
        }else{
            turno = local;
        }
    }
}
void Juego::manejarEnJuego(sf::Event& e){
    switch(e.type){
    case sf::Event::MouseMoved:{
        auto mousePos = ventana->mapPixelToCoords(sf::Mouse::getPosition(*ventana), *camara);
        Posicion mp = tablero->coord2pos(mousePos);
        std::string s1 = mp.to_string() + "\n";
        std::string s2 = tablero->getInfo(mp);
        info = s1+s2;
        break;
    }
    case sf::Event::MouseButtonPressed:{
        if(turno == local){
            auto mousePos = ventana->mapPixelToCoords(sf::Mouse::getPosition(*ventana), *camara);
            if(e.mouseButton.button == sf::Mouse::Left){
                /*
                * PRIMER CLICK
                */
                if(!firstClick.validar()){
                    Posicion click = tablero->coord2pos(mousePos);
                    if(click.validar()){        
                        auto p = tablero->piezaEn(click);
                        auto pm = tablero->posiblesMovimientos(p);
                        if(p != nullptr && p->equipo == turno->equipo){
                            firstClick.r = click.r; firstClick.c = click.c;
                            tablero->marcar(firstClick, sf::Color::Blue);
                            for(auto m : pm){
                                sf::Color c = 
                                    tablero->piezaEn(m.destino) == nullptr ?
                                        sf::Color::White : 
                                        sf::Color::Red ;
                                tablero->marcar(m.destino, c);
                            }
                        }
                    }
                /*
                * SEGUNDO CLICK
                */
                }else{
                    Movimiento m(firstClick, tablero->coord2pos(mousePos));
                    if(tablero->probar(m, turno->equipo)){
                        local->preparar(m);
                    }else{
                        *avisos << "Movimiento no valido" << std::endl;
                    }
                    firstClick.r = FUERA;firstClick.c = FUERA;
                    tablero->borrarMarcas();
                }
            }
        }
        break;
    }
    }
}
void Juego::manejarMenu(sf::Event& e){
    switch(e.type){
    case sf::Event::KeyPressed:
        switch(e.key.code){
            case sf::Keyboard::L:
                jugar(LOCAL);
            break;
            case sf::Keyboard::H:
                listener.listen(port);
                socket_sel.add(listener);
                estado = ESCUCHANDO;
            break;
            case sf::Keyboard::C:
                estado = ENTRADA;
            break;
            case sf::Keyboard::I:
                jugar(IA);
            break;
            case sf::Keyboard::Q:
                ventana->close();
            break;
        }
        break;    
    }
}
void Juego::manejarEntrada(sf::Event& e){
    switch(e.type){
        case sf::Event::KeyPressed:
            if(e.key.code == sf::Keyboard::Enter){
                host = info;
                jugar(ONLINE);
            }
            break;
        case sf::Event::TextEntered:
            switch(e.text.unicode){
            case 2386: // ENTER
                break;
            case 8: // BACKSPACE
                if(info.getSize() > 0){
                    info.erase(info.getSize()-1);
                }
                break;
            case 0x0030: // NUMBERS AND POINTS
            case 0x0031: 
            case 0x0032: 
            case 0x0033: 
            case 0x0034: 
            case 0x0035: 
            case 0x0036: 
            case 0x0037: 
            case 0x0038: 
            case 0x0039: 
            case 0x002E: 
                info += e.text.unicode;
                break;
            }
    }
}
Juego::Juego(sf::RenderWindow& w, sf::View& c, int size, const std::string&textureFile):
    local(new JugadorCliente),
    host("localhost"),
    port(9090)
    {
    if(!Pieza_st::texture.loadFromFile(textureFile)){
        std::cerr << "No se pudo cargar las texturas" << std::endl;
        exit(1);
    }
    Pieza_st::texture.setSmooth(true);
    tablero = std::make_shared<TableroAjedrez>(size); 
    ventana = &w;
    camara = &c;
    avisos = &std::cout;
    estado = MENU;
    }
std::string Juego::getInfo(){
    std::stringstream ret;
    switch(estado){
    case JUEGO:
        ret << modo2str(modo) << "\n\n"
            << "TURNO " << equipo2str(turno->equipo) << "\n" 
            << (std::string)info;
        break;
    case MENU:
        ret << "MENU\n"
            << "[L]\n" << "Partida local\n\n"
            << "[H]\n" << "Alojar partida \nonline\n\n"
            << "[C]\n" << "Unirse a online\n\n"
            << "[I]\n" << "Partida contra IA\n\n"
            << "[Q]\n" << "Quitar";
        break;
    case ESCUCHANDO:
        ret << "ESPERANDO CONEXION\n"
            << sf::IpAddress::getLocalAddress();
        break;
    case ENTRADA:
        ret << "INTRODUCIR DIRECCION IP:\n"
            << (std::string)info << "_";
        break;
    }
    return ret.str();
}
void Juego::actualizar(){
    switch(estado){
    case JUEGO:
        if(turno->listo()){
            Movimiento j(turno->jugada());
            tablero->mover(j);
            if(tablero->jaquemate(!(turno->equipo))){
                *avisos << "JAQUE MATE\n";
                terminar();
            }else{
                cambiaTurno();
                if(tablero->jaque(turno->equipo)){
                    *avisos << "JAQUE\n";
                }
            }
            turno->notificar(j);
        }
        break;
    case ESCUCHANDO:
        if(socket_sel.wait(sf::milliseconds(10)) && socket_sel.isReady(listener)){
            jugar(HOST);
        }
        break;
    }
}
void Juego::jugar(Modo m){
    switch(m){
        case LOCAL:{
            contrincante.reset(new JugadorCliente);
            local->equipo = BLANCAS;
            contrincante->equipo = NEGRAS;
            turno = local;
            estado = JUEGO;
            modo = m;
            break;
        }
        case IA:{
            auto ia = new JugadorIA;
            ia->tablero = tablero;
            contrincante.reset(ia);
            if(rd()%2 == 0){
                local->equipo = NEGRAS;
                contrincante->equipo = BLANCAS;
                turno = contrincante;
                girar();
            }else{
                local->equipo = BLANCAS;
                contrincante->equipo = NEGRAS;
                turno = local;
            }
            estado = JUEGO;
            modo = m;
            break;
        }
        case ONLINE:{
            if(socket.connect(host,port) == sf::Socket::Done){
                contrincante.reset(new JugadorOnline(socket));
                turno = contrincante;
                local->equipo = NEGRAS;
                contrincante->equipo = BLANCAS;
                girar();
                estado = JUEGO;
                modo = m;
            }else{
                std::cerr << "No se pudo conectar a:"
                << host << ":" << port << std::endl;
                estado = MENU;
            }
            break;
        }
        case HOST:{
            std::cerr << "Conexión recibida" << std::endl;
            if(listener.accept(socket) != sf::Socket::Done){
                std::cerr << "No se pudo conectar (accept)" << std::endl;
                estado = MENU;
            }else{
                contrincante.reset(new JugadorOnline(socket));
                socket_sel.clear();
                turno = local;
                local->equipo = BLANCAS;
                contrincante->equipo = NEGRAS;
                estado = JUEGO;
                modo = m;
            }
            listener.close();
            break;
        }
    }
}
void Juego::manejar(sf::Event& e){
    switch(estado){
    case JUEGO:
        manejarEnJuego(e);
        break;
    case MENU:
        manejarMenu(e);
        break;
    case ENTRADA:
        manejarEntrada(e);
        break;
    }
}
void Juego::terminar(){
    if(estado == JUEGO){
        if(modo == ONLINE || modo == HOST){
            socket.disconnect();
        }
        turno = nullptr;
        estado = MENU;
    }
}
Juego::~Juego(){
    socket.disconnect();
}
