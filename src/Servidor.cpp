#include "Servidor.h"

Servidor::Servidor(Nat cantJug,const Variante &v, Repositorio rep):_variante(v), juego(cantJug,v,rep){
    _clientes.esperados = cantJug;
    _clientes.conectados = 0;
    vector<tuple<Notificacion,Nat>> vacio = {};
    vector<vector<tuple<Notificacion,Nat>>> porJugador(cantJug, vacio) ;
    _notifs.notificacionesPorJugador = porJugador;
    _notifs.nTodes.notificacionesServidor = {};
    vector<Nat> leyoHasta(cantJug,0);
    _notifs.nTodes.leyoHasta = leyoHasta;
    _idNotif = 0;

}

IdCliente Servidor::conectarCliente() { //TODO : mandar fichas como en el mail


    _notifs.notificacionesPorJugador[_clientes.conectados].push_back(make_tuple(Notificacion ::nuevaIdCliente(_clientes.conectados),_idNotif));
    _idNotif++;
    if (++_clientes.conectados == _clientes.esperados){
        _notifs.nTodes.notificacionesServidor.push_back(make_tuple(Notificacion ::nuevaEmpezar(juego.tamanoTab()),_idNotif));
        _notifs.nTodes.notificacionesServidor.push_back(make_tuple(Notificacion::nuevaTurnoDe(0), ++_idNotif));
        vector<multiset<Letra>> primera = juego.damePrimeraMano();
        for (int i = 0; i < _clientes.esperados; ++i) {
            _notifs.notificacionesPorJugador[i].push_back(make_tuple(Notificacion::nuevaReponer(primera[i]),++_idNotif));
        }

        _idNotif++;
    }
    _clientes.conectados;
    return _clientes.conectados - 1;


}

void Servidor::recibirMensaje(Ocurrencia o, Nat idCliente) {

    bool valida  = juego.EsjugadaValida(o);
    bool tieneFichas = juego.tieneFichas(o,idCliente);
    bool esTurno = juego.obtenerTurno() == idCliente;
    bool comenzo = numeroClientesEsperados() == numeroClientesConectados();
    if(valida and tieneFichas and esTurno and comenzo){
        Nat viejoPuntaje = juego.puntajeAnterior(idCliente);
        multiset<Letra> Repo = juego.ubicar(o);
        _notifs.nTodes.notificacionesServidor.push_back(make_tuple(Notificacion::nuevaUbicar(idCliente,o),_idNotif));
        _notifs.nTodes.notificacionesServidor.push_back(
                make_tuple(Notificacion ::nuevaSumaPuntos(idCliente,
                                                                   juego.obtenerPuntaje(idCliente) - viejoPuntaje )
                                                                   ,++_idNotif));
        _notifs.notificacionesPorJugador[idCliente].push_back(make_tuple(Notificacion::nuevaReponer(Repo),++ _idNotif));
        _notifs.nTodes.notificacionesServidor.push_back(make_tuple(Notificacion ::nuevaTurnoDe(idCliente + 1 ),++ _idNotif));
        _idNotif++;


    }
    else{
        _notifs.notificacionesPorJugador[idCliente].push_back(make_tuple(Notificacion :: nuevaMal(),_idNotif ++));
    }
}

const Nat Servidor::numeroClientesEsperados() const {
    return _clientes.esperados;
}

const Nat Servidor::numeroClientesConectados() const {
    return _clientes.conectados;
}

list<Notificacion> Servidor::consultarNotificaciones(Nat idCliente) {
list<Notificacion> res = {};
Nat desde = _notifs.nTodes.leyoHasta[idCliente];
vector<tuple<Notificacion,Nat>> porJugador = _notifs.notificacionesPorJugador[idCliente];
vector<tuple<Notificacion,Nat>> todasNotis = _notifs.nTodes.notificacionesServidor;
int i = 0;

int len  = porJugador.size();
int len2 = todasNotis.size();
    while (i < len && desde < len2){
        if(get<1>(porJugador[i]) < get<1>(todasNotis[desde])){
            res.push_back(get<0>(porJugador[i]));
            i++;
        }
        else{
            res.push_back(get<0>(todasNotis[desde]));
        desde++;
        }

    }
    while (i < len ){
        res.push_back(get<0>(porJugador[i]));
        i++;
    }
    while (desde < len2){
        res.push_back(get<0>(todasNotis[desde]));
        desde++;
    }
    _notifs.nTodes.leyoHasta[idCliente] = len2;
    _notifs.notificacionesPorJugador[idCliente] = {};
    return res;
}


