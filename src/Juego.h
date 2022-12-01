#ifndef TP_JUEGODEPALABRAS_JUEGO_H
#define TP_JUEGODEPALABRAS_JUEGO_H

#include "Variante.h"


class Juego {
public:
    const Nat numJugadores() const;
    Juego(Nat numJugadores, const Variante& var, Repositorio rep);
    multiset<Letra> ubicar(Ocurrencia o);
    const Variante obtenerVariante() const;
    const Nat obtenerTurno() const;
    const Nat obtenerPuntaje(Nat j);
    const bool EsjugadaValida(Ocurrencia o);
    const Letra fichaEnPosicion(Nat fila , Nat columna) const;
    const bool hayFicha(Nat fila, Nat columna) const;
    const Nat cuantasDeEstaTiene(Letra l, Nat idCliente) const;
    const Nat tamanoTab() const;
    const bool tieneFichas(Ocurrencia o, Nat idCliente) const;
    const Nat cantidadLetrasOcurrencia(Letra l, Ocurrencia o) const;
    const Nat puntajeAnterior(Nat id)const;
    multiset<Letra> reponerN(Nat n,Nat id);
    const vector<multiset<Letra>> damePrimeraMano() const;
private:
    Repositorio _rep;
    struct Casillero{
        Letra letra;
        Nat   turnoJugado;
        bool  ocupado;
    };
    using Tablero = vector<vector<Casillero>>;
    Tablero tablero;
    const Variante& v;
    Nat numJug;
    Nat turnoActual;
    struct jugador{
        using Mano = vector<int>;
        Mano mano;
        Nat puntaje;
        Ocurrencia fichasQuePusoDesde;

    };
    vector<multiset<Letra>> _primeraMano;
    using Jugadores = vector<jugador>;
    Jugadores jugadores;
    Jugadores repartirFichas(Nat cantJugadores);
    Tablero crearTablero(Nat tamano);
    void agregarPalabraHorizontal(list<Palabra> &palabras, pair<Nat, Nat> prim, pair<Nat, Nat> ult);
    void agregarPalabraVertical(list<Palabra> &palabras, pair<Nat, Nat> arr, pair<Nat, Nat> abj);
    Nat puntajeTotal(set<Palabra> palabras);
    const bool enRango(Nat i, Nat j);
    const bool Horizontal(Ocurrencia o) const;
    const bool Vertical(Ocurrencia o) const;
    const bool sonContiguas(pair<Nat, Nat> prim, pair<Nat, Nat> ult, bool horizontal);
};

#endif //TP_JUEGODEPALABRAS_JUEGO_H