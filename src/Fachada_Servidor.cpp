#include "Fachada_Servidor.h"

Fachada_Servidor::Fachada_Servidor(Nat cantJugadores, const Fachada_Variante &variante, const Repositorio &r): servidor(cantJugadores,variante.daVariante(),r){
}

IdCliente Fachada_Servidor::conectarCliente() {
    return servidor.conectarCliente();
}

void Fachada_Servidor::recibirMensaje(IdCliente id, const Ocurrencia &o) {
servidor.recibirMensaje(o,id);
}

Nat Fachada_Servidor::jugadoresEsperados() {
    return servidor.numeroClientesEsperados();
}

Nat Fachada_Servidor::jugadoresConectados() {
    return servidor.numeroClientesConectados();
}

std::list<Notificacion> Fachada_Servidor::notificaciones(IdCliente id) {
    return servidor.consultarNotificaciones(id);
}