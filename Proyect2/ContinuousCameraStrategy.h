#pragma once
#include "ICameraBehavior.h"

class ContinuousCameraStrategy : public ICameraBehavior {
public:
    void moverDerecha(Jugador& jugador, MapaNivel1& mapa,
        int zonaCamaraX, int anchoPantalla, int altoTotal) override {
        if (jugador.getX() < zonaCamaraX)
            jugador.mover(2, 0, 1, anchoPantalla, altoTotal);
        else {
            mapa.avanzar(2);
            jugador.mover(0, 0, 1, anchoPantalla, altoTotal);
        }
    }

    void moverIzquierda(Jugador& jugador, MapaNivel1& mapa,
        int zonaCamaraX, int anchoPantalla, int altoTotal) override {
        if (mapa.getPosicionMundo() > 0 && jugador.getX() >= zonaCamaraX) {
            mapa.retroceder(2);
            jugador.mover(0, 0, -1, anchoPantalla, altoTotal);
        }
        else if (jugador.getX() - 2 >= 0)
            jugador.mover(-2, 0, -1, anchoPantalla, altoTotal);
    }
};
