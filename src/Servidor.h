#ifndef TP_JUEGODEPALABRAS_SERVIDOR_H
#define TP_JUEGODEPALABRAS_SERVIDOR_H

#include "Variante.h"
#include "Juego.h"
#include "Notificacion.h"

class Servidor{
public:
    Servidor(Nat cantJug, const Variante &v, Repositorio rep);
    IdCliente conectarCliente();
    list<Notificacion> consultarNotificaciones(Nat idCliente);
    void recibirMensaje(Ocurrencia o, Nat idCliente);
    const Nat numeroClientesEsperados() const;
    const Nat numeroClientesConectados() const;


private:
    struct notificacionesTodos{
        vector<tuple<Notificacion,Nat>> notificacionesServidor;
        vector<Nat> leyoHasta;
    };

    struct notificaciones{

        notificacionesTodos nTodes; /// hace falta?
        vector<vector<tuple<Notificacion,Nat>>> notificacionesPorJugador;
    };
    struct Clientes{
        Nat esperados;
        Nat conectados;
    };
    Nat _idNotif;
    notificaciones _notifs;
    Clientes _clientes;
    Juego juego;
    const Variante& _variante;
};

#endif //TP_JUEGODEPALABRAS_SERVIDOR_H