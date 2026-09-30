#include "ContinuousCameraStrategy.h"

//declaracion de variables internas
ContinuousCameraStrategy::ContinuousCameraStrategy(int anchoP, int anchoM) 
    : 
    centroPantalla(anchoP / 2),
    anchoPantalla(anchoP),
    anchoMundo(anchoM) { }

void ContinuousCameraStrategy::updateCamera(int playerX, int playerY, int& camX, int& camY) {
    // Si el jugador no ha cruzado el umbral, la cámara no se mueve (X = 0)
    if (playerX < centroPantalla) {
        camX = 0;
    }
    // Si el jugador llego al final de nivel, la camara se detiene (camX es constante)
    else if (player > (anchoMundo - anchoPantalla)) {
        camX = anchoMundo - anchoPantalla;
    }

    //durante rango(CentroPantalla, AnchoPantalla - CentroPantalla) playerx sera variable, por lo que camX tambien
    else {
        camX = playerX - centroPantalla;
    }

    // En el Nivel 1 no hay movimiento vertical de cámara
    camY = 0;
}