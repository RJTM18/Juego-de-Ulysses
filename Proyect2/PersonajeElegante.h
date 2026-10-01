#pragma once
#include <string>
using namespace std;

class PersonajeElegante {
private:
    int x, y;
public:
    PersonajeElegante(int inicioX, int inicioY) : x(inicioX), y(inicioY) {}
    int getX() const { return x; }
    int getY() const { return y; }
    string getLineaSprite(int filaActual) const {
        int parte = filaActual - y;
        if (parte == 0) return "_=|=_"; // sombrero elegante
        if (parte == 1) return " (|) ";
        if (parte == 2) return " / \\ ";
        return "";
    }
};
