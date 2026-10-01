#pragma once
#include "Jugador.h"
#include "MapaNivel2.h"

// Strategy del Nivel 2: no hay scroll continuo.
// Bloom se mueve dentro de una pantalla y, al alcanzar un borde,
// se cambia de seccion completa.
class SegmentedCameraStrategy {
public:
    void moverDerecha(Jugador& jugador, MapaNivel2& mapa, int anchoPantalla, int altoTotal) {
        const int LIMITE_DERECHO = anchoPantalla - 6;
        if (jugador.getX() < LIMITE_DERECHO) {
            jugador.mover(2, 0, 1, anchoPantalla, altoTotal);
            return;
        }

        if (mapa.siguienteParte())
            jugador.colocar(2, jugador.getY());
    }

    void moverIzquierda(Jugador& jugador, MapaNivel2& mapa, int anchoPantalla, int altoTotal) {
        if (jugador.getX() > 2) {
            jugador.mover(-2, 0, -1, anchoPantalla, altoTotal);
            return;
        }

        if (mapa.parteAnterior())
            jugador.colocar(anchoPantalla - 8, jugador.getY());
    }
};
