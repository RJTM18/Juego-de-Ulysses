#pragma once
#include <iostream>
#include <string>

using namespace std;

class Jugador {
private:
    int x, y;
    int vidas;
    int frameActual;
    int totalFrames;
    int direccion; // 1 = Derecha, -1 = Izquierda

public:
    Jugador(int inicioX, int inicioY) {
        x = inicioX;
        y = inicioY;
        vidas = 3;
        frameActual = 1;
        totalFrames = 3;
        direccion = 1;
    }

    int getX() const { return x; }
    int getY() const { return y; }
    int getVidas() const { return vidas; }
    void perderVida() { vidas--; }

    void mover(int desplaceX, int desplaceY, int nuevaDireccion, int limiteX, int limiteY) {
        direccion = nuevaDireccion;
        x += desplaceX;
        y += desplaceY * 3; // Mover en bloques de 3 líneas

        // Límites de la pantalla
        if (x < 0) x = 0;
        if (x > limiteX - 5) x = limiteX - 5;
        if (y < 0) y = 0;
        if (y > limiteY) y = limiteY;

        frameActual = (frameActual + 1) % totalFrames;
    }

    // Retorna la línea correspondiente del sprite según la fila que se esté dibujando
    string getLineaSprite(int filaActual) {
        int parteCuerpo = filaActual - y;
        if (parteCuerpo < 0 || parteCuerpo >= 3) return "";

        if (direccion == 1) { // Mirando a la DERECHA
            if (parteCuerpo == 0) return "  L";
            if (parteCuerpo == 1) return " (|)";
            if (parteCuerpo == 2) {
                if (frameActual == 0) return " /  L";
                if (frameActual == 1) return " / L";
                if (frameActual == 2) return "  L \\";
            }
        }
        else { // Mirando a la IZQUIERDA
            if (parteCuerpo == 0) return "  L";
            if (parteCuerpo == 1) return " (|)";
            if (parteCuerpo == 2) {
                if (frameActual == 0) return " J  \\";
                if (frameActual == 1) return " J /";
                if (frameActual == 2) return "/  J";
            }
        }
        return "";
    }
};
