#include "Juego.h"

Juego::Tablero Juego::crearTablero(Nat tamano) {
    Casillero casillero;
    casillero.letra = ' ';
    casillero.ocupado = false;
    casillero.turnoJugado = 0;
    vector<Casillero> filas(tamano, casillero);
    Tablero tablero(tamano, filas);
    return tablero;
}

Juego::Jugadores Juego::repartirFichas(Nat cantJugadores) {
    jugador::Mano mano(TAMANIO_ALFABETO,0);
    Ocurrencia fichasQuePusoDesde = {};
    Letra ficha;
    jugador jugador1;
    jugador1.mano = mano;
    jugador1.puntaje = 0;
    jugador1.fichasQuePusoDesde = fichasQuePusoDesde;
    Jugadores jugadores1(cantJugadores, jugador1);
    for (int i = 0; i < cantJugadores; ++i) {
        for (int j = 0; j < v.cantFichas(); ++j) {
            ficha = _rep.front();
            _rep.pop_front();
            jugadores1[i].mano[ord(ficha)]++;
            _primeraMano[i].insert(ficha);
        }
    }
    return jugadores1;



}
Juego ::Juego(Nat numJugadores, const Variante &var, Repositorio rep): v(var), _primeraMano(numJugadores,multiset<Letra>()){
    numJug = numJugadores;
    _rep = rep;
    turnoActual = 0;
    tablero = crearTablero(v.tamanoTablero());
    jugadores = repartirFichas(numJugadores);

}
const  Nat Juego :: numJugadores() const{
    return numJug;
};

multiset<Letra> Juego::ubicar(Ocurrencia o) {
    for (auto ficha: o) {
        Nat i = get<0>(ficha);
        Nat j = get<1>(ficha);
        Letra l = get<2>(ficha);
        tablero[i][j].letra = l;
        tablero[i][j].turnoJugado = turnoActual;
        tablero[i][j].ocupado = true;
        jugadores[obtenerTurno()].mano[ord(l)]--;
        jugadores[obtenerTurno()].fichasQuePusoDesde.insert(ficha);
    }
    multiset<Letra> repo = reponerN(o.size(),obtenerTurno());
    turnoActual++;
    return repo;
}

const Variante Juego::obtenerVariante() const {
    return v;
}

const Nat Juego::obtenerTurno() const {
    return turnoActual % numJug;
}

const Nat Juego::obtenerPuntaje(Nat j) {
    list<Palabra> palabrasV;
    list<Palabra> palabrasH;
    Ocurrencia fichasQuePusoDesde = jugadores[j].fichasQuePusoDesde;
    pair<Nat,Nat> prim;
    pair<Nat,Nat> ult;
    pair<Nat,Nat> arr;
    pair<Nat,Nat> abj;
    Nat turno;

    for (auto ficha: fichasQuePusoDesde) {
        prim = make_pair(get<0>(ficha),get<1>(ficha));
        ult = make_pair(get<0>(ficha),get<1>(ficha));
        arr = make_pair(get<0>(ficha),get<1>(ficha));
        abj = make_pair(get<0>(ficha),get<1>(ficha));
        turno = tablero[prim.first][prim.second].turnoJugado;

        while(arr.first>0 and
              tablero[arr.first - 1][arr.second].ocupado == true and
              tablero[arr.first - 1][arr.second].turnoJugado <= turno){
            arr.first--;
        }
        while(abj.first<v.tamanoTablero()-1 and
              tablero[abj.first + 1][abj.second].ocupado == true and
              tablero[abj.first + 1][abj.second].turnoJugado <= turno){
            abj.first++;
        }
        while(prim.second>0 and
              tablero[prim.first][prim.second - 1].ocupado == true and
              tablero[prim.first][prim.second - 1].turnoJugado <= turno){
            prim.second--;
        }
        while(ult.second<v.tamanoTablero()-1 and
              tablero[ult.first][ult.second + 1].ocupado == true and
              tablero[ult.first][ult.second + 1].turnoJugado <= turno){
            ult.second++;
        }
        agregarPalabraHorizontal(palabrasH, prim, ult);
        agregarPalabraVertical(palabrasV, arr, abj);
        jugadores[j].fichasQuePusoDesde.clear();

    };
    set<Palabra> pH;
    set<Palabra> pV;

    for (auto palH: palabrasH) {
        pH.insert(palH);
    }
    for (auto palV: palabrasV) {
        pV.insert(palV);
    }
    jugadores[j].puntaje = jugadores[j].puntaje + puntajeTotal(pH) + puntajeTotal(pV);

    return jugadores[j].puntaje;
}

void Juego::agregarPalabraHorizontal(list<Palabra> &palabras, pair<Nat, Nat> prim, pair<Nat, Nat> ult) {
    Palabra palabra = {};
    for (int i = prim.second; i <= ult.second; ++i) {
        palabra.push_back(tablero[prim.first][i].letra);
    }
    palabras.push_back(palabra);

}

void Juego::agregarPalabraVertical(list<Palabra> &palabras, pair<Nat, Nat> arr, pair<Nat, Nat> abj) {
    Palabra palabra = {};
    for (int i = arr.first; i <= abj.first; ++i) {
        palabra.push_back(tablero[i][arr.second].letra);
    }
    palabras.push_back(palabra);
}




Nat Juego::puntajeTotal(set<Palabra> palabras) {
    Nat res = 0;
    for (Palabra p: palabras) {
        for (Letra l: p) {
            res = res + v.puntajeLetra(l);
        }
    }
    return res;
}

const bool Juego::EsjugadaValida(Ocurrencia o) {
    if (o.empty()){
        return true;
    }
    Nat lmax = v.obtenerLmax();
    bool pepito = v.daUniversal().count({'a','b','b'}) == 1;
    if (o.size() > lmax){
        return false;
    }
    list<Palabra> posiblesPalabras;
    bool horizontal = Horizontal(o);
    bool vertical = Vertical(o);
    Nat n = v.tamanoTablero();
    pair<Nat, Nat> prim = make_pair(0,n-1);
    pair<Nat, Nat> ult = make_pair(0,0);
    pair<Nat, Nat> arr = make_pair(n-1,0);
    pair<Nat, Nat> abj = make_pair(0,0);
    if(not(horizontal or vertical)){
        return false;
    }
    for (auto ficha: o) {
        Nat i = get<0>(ficha);
        Nat j = get<1>(ficha);
        Letra letra = get<2>(ficha);
        if(not(enRango(i,j))){
            return false;
        }
        if(tablero[i][j].ocupado){
            return false;
        }
        tablero[i][j].letra = letra;
        tablero[i][j].ocupado = true;

        if(horizontal){
            prim = make_pair(i, min(j,prim.second));
            ult = make_pair(i, max(j,ult.second));
            arr = make_pair(i, j);
            abj = make_pair(i, j);
        }else{
            prim = make_pair(i, j);
            ult = make_pair(i, j);
            arr = make_pair(min(i,arr.first), j);
            abj = make_pair(max(i,abj.first), j);
        }
        if((ult.second - prim.second >= lmax) and (abj.first - arr.first >= lmax)){
            return false;
        }

        while(arr.first>0 and
              tablero[arr.first - 1][arr.second].ocupado == true){
            arr.first--;
        }
        while(abj.first<v.tamanoTablero()-1 and
              tablero[abj.first + 1][abj.second].ocupado == true){
            abj.first++;
        }
        while(prim.second>0 and
              tablero[prim.first][prim.second - 1].ocupado == true){
            prim.second--;
        }
        while(ult.second<v.tamanoTablero()-1 and
              tablero[ult.first][ult.second + 1].ocupado == true){
            ult.second++;
        }
        if(horizontal){
            agregarPalabraVertical(posiblesPalabras,arr,abj);
        }
        else{
            agregarPalabraHorizontal(posiblesPalabras,prim,ult);
        }
    }
    if(horizontal){
        if(sonContiguas(prim, ult, horizontal)){
            agregarPalabraHorizontal(posiblesPalabras,prim,ult);
        }else{
            return false;
        }
    }else{
        if(sonContiguas(arr, abj, horizontal)){
            agregarPalabraVertical(posiblesPalabras, arr, abj);
        }else{
            return false;
        }
    }

    for (auto ficha: o) {
        Nat i = get<0>(ficha);
        Nat j = get<1>(ficha);
        tablero[i][j].ocupado = false;
    }

    for (Palabra pal: posiblesPalabras) {
        if (not(v.daUniversal().count(pal))){
            return false;
        }
    }
    return true;
}

const bool Juego::enRango(Nat i, Nat j) {
    return (i >= 0 and i < v.tamanoTablero()) and (j >= 0 and j < v.tamanoTablero());
}

const bool Juego::Horizontal(Ocurrencia o) const {
    if (o.empty()){
        return true;
    }
    else{
        Nat i = get<0>(*(o.begin()));
        for (auto it = o.begin()++; it != o.end() ; ++it) {
            if(i  != get<0>(*(it))){
                return false;
            }
        }
    }
    return true;
}

const bool Juego::Vertical(Ocurrencia o) const {
    if (o.empty()){
        return true;
    }
    else{
        Nat i = get<1>(*(o.begin()));
        for (auto it = o.begin()++; it != o.end() ; ++it) {
            if(i  != get<1>(*(it))){
                return false;
            }
        }
    }
    return true;
}

const bool Juego::sonContiguas(pair<Nat, Nat> prim, pair<Nat, Nat> ult, bool horizontal) {
    if(horizontal){
        for (int i = prim.second; i <= ult.second; ++i) {
            if(tablero[prim.first][i].ocupado == false){
                return false;
            }
        }
    }else{
        for (int i = prim.first; i <= ult.first; ++i) {
            if(tablero[i][prim.second].ocupado == false){
                return false;
            }
        }
    }
    return true;
}

const Letra Juego::fichaEnPosicion(Nat fila, Nat columna) const {
    return tablero[fila][columna].letra;
}

const bool Juego::hayFicha(Nat fila, Nat columna) const {
    return tablero[fila][columna].ocupado;
}

const Nat Juego::cuantasDeEstaTiene(Letra l, Nat idCliente) const {
    return jugadores[idCliente].mano[ord(l)];
}

const Nat Juego::tamanoTab() const {
    return v.tamanoTablero();
}

const bool Juego::tieneFichas(Ocurrencia o,Nat idCliente) const {
    for (auto ficha:o) {
        Letra l = get<2>(ficha);
    if((jugadores[idCliente].mano[ord(l)]) < cantidadLetrasOcurrencia(l,o)){
        return false;


    }
    }
    return true;
}

const Nat Juego::cantidadLetrasOcurrencia(Letra l, Ocurrencia o) const {
    Nat res = 0;
    for (auto ficha:o) {
        if(get<2>(ficha) == l){
            res++;
        }
    }
    return res;
}

const Nat Juego::puntajeAnterior(Nat id) const {
    return jugadores[id].puntaje;
}

multiset<Letra> Juego::reponerN(Nat n,Nat id) {

    multiset<Letra> REpuestas = {};
    for (int i = 0; i < n ; ++i) {
        Letra l = _rep.front();
        _rep.pop_front();
        REpuestas.insert(l);
        jugadores[id].mano[ord(l)] ++;
    }

    return REpuestas;
}

const vector<multiset<Letra>> Juego::damePrimeraMano() const {
    return _primeraMano;
}
