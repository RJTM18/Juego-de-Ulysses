#pragma once
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

class Carro {
private:
    int x, y;
    int anchoPantalla;

public:
    Carro(int inicioX, int inicioY, int ancho) {
        x = inicioX;
        y = inicioY;
        anchoPantalla = ancho;
    }

    int getX() const { return x; }
    int getY() const { return y; }

    void avanzar() {
        x -= 2; // NUEVO: Avanza 2 espacios por ciclo (Duplica velocidad)

        // Ajustamos el límite de salida por la izquierda debido al paso doble
        if (x < -14) {
            x = anchoPantalla;
            y = 6 + (rand() % 4) * 3;
        }
    }


    void resetearPosicion() {
        x = anchoPantalla;
        y = 6 + (rand() % 4) * 3; // FIJADO
    }

    bool verificaColision(int jugadorX, int jugadorY) {
        if (y == jugadorY) {
            if (jugadorX + 3 >= x && jugadorX <= x + 10) {
                return true;
            }
        }
        return false;
    }

    string getLineaSprite(int filaActual) {
        int parteCarro = filaActual - y;
        if (parteCarro < 0 || parteCarro >= 3) return "";

        if (parteCarro == 0) return "   ______   ";
        if (parteCarro == 1) return " _/ /_/  |  ";
        if (parteCarro == 2) return "|__0____0...";
        return "";
    }
};
