#pragma once
#include "Jugador.h"
#include "MapaNivel1.h"

class ICameraBehavior {
public:
    virtual ~ICameraBehavior() = default;
    virtual void moverDerecha(Jugador& jugador, MapaNivel1& mapa,
        int zonaCamaraX, int anchoPantalla, int altoTotal) = 0;
    virtual void moverIzquierda(Jugador& jugador, MapaNivel1& mapa,
        int zonaCamaraX, int anchoPantalla, int altoTotal) = 0;
};
