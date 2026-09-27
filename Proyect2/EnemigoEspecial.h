#pragma once
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

//Hola soy Rafael

class EnemigoEspecial {
private:
    int x, y;
    int anchoPantalla;
    bool activo;

public:
    EnemigoEspecial(int ancho) {
        anchoPantalla = ancho;
        x = anchoPantalla;
        y = 6;
        activo = false;
    }

    int getX() const { return x; }
    int getY() const { return y; }
    bool isActivo() const { return activo; }

    void avanzar() {
        if (!activo) {
            // NUEVO: 16% de probabilidad de aparecer en cada ciclo (Extremadamente rápido)
            if ((rand() % 6) == 0) {
                activo = true;
                x = anchoPantalla;
                y = 6 + (rand() % 4) * 3;
            }
            return;
        }

        x -= 2; // NUEVO: Avanza 2 espacios por ciclo

        if (x < -12) {
            activo = false;
        }
    }



    void resetearPosicion() {
        activo = false;
        x = anchoPantalla;
    }

    bool verificaColision(int jugadorX, int jugadorY) {
        if (activo && y == jugadorY) {
            if (jugadorX + 3 >= x && jugadorX <= x + 7) {
                return true;
            }
        }
        return false;
    }

    string getLineaSprite(int filaActual) {
        if (!activo) return "";

        int parteEnemigo = filaActual - y;
        if (parteEnemigo < 0 || parteEnemigo >= 3) return "";

        if (parteEnemigo == 0) return "  \\_X_/ ";
        if (parteEnemigo == 1) return " [###]";
        if (parteEnemigo == 2) return "   _/ \\_ ";
        return "";
    }
};
