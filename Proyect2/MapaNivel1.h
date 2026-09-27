#pragma once
#include <iostream>
#include <string>
using namespace std;

class MapaNivel1 {
private:
    int anchoPantalla;
    int posicionMundo;

    // Cada escenario ocupa un tramo REAL del mundo horizontal.
    static const int INICIO_INDUSTRIAL = 180;
    static const int INICIO_NOCTURNO = 340;
    static const int FIN_RECORRIDO = 500;

    //hacer cambio

    char pista(int mundoX, int fila, char linea) const {
        if (fila == 4) return '=';
        if (fila == 5) return '-';
        if (fila >= 6 && fila < 18) {
            if (fila == 8 || fila == 11 || fila == 14)
                return (mundoX % 12 < 6) ? linea : ' ';
            return ((mundoX + fila) % 17 == 0) ? '.' : ' ';
        }
        return ' ';
    }

    char ciudad(int mundoX, int fila) const {
        int p = mundoX % 15;
        if (fila == 0) return (p == 0 || p == 9) ? '|' : ((p > 0 && p < 9) ? '_' : ' ');
        if (fila == 1 || fila == 2) {
            if (p == 0 || p == 9) return '|';
            return (p == 2 || p == 5 || p == 7) ? 'o' : ' ';
        }
        if (fila == 3) return (p == 0 || p == 9) ? '|' : ((p > 0 && p < 9) ? '_' : ' ');
        return pista(mundoX, fila, '-');
    }

    char industrial(int mundoX, int fila) const {
        int local = mundoX - INICIO_INDUSTRIAL;
        if (local < 0) local = 0;
        int p = local % 22;
        if (fila == 0) return (p == 4 || p == 5) ? '|' : (p == 16 ? '~' : ' ');
        if (fila == 1) return (p == 0 || p == 14) ? '|' : ((p > 0 && p < 14) ? '=' : ' ');
        if (fila == 2) {
            if (p == 0 || p == 14) return '|';
            return (p == 3 || p == 7 || p == 11) ? '#' : ' ';
        }
        if (fila == 3) return (p == 0 || p == 14) ? '|' : ((p > 0 && p < 14) ? '_' : ' ');
        return pista(local, fila, '=');
    }

    char nocturno(int mundoX, int fila) const {
        int local = mundoX - INICIO_NOCTURNO;
        if (local < 0) local = 0;
        int p = local % 18;
        if (fila == 0) return (local % 19 == 2) ? '*' : ((local % 19 == 11) ? '.' : ' ');
        if (fila == 1) return p == 3 ? '/' : (p == 4 ? '^' : (p == 5 ? '\\' : ' '));
        if (fila == 2) return (p == 2 || p == 3) ? '/' : (p == 4 ? '|' : ((p == 5 || p == 6) ? '\\' : ' '));
        if (fila == 3) return p == 4 ? '|' : ' ';
        return pista(local, fila, '-');
    }

public:
    MapaNivel1(int ancho) : anchoPantalla(ancho), posicionMundo(0) {}

    // El fondo SOLO se mueve cuando el main llama estas funciones.
    void avanzar(int cantidad = 2) {
        posicionMundo += cantidad;
        if (posicionMundo > FIN_RECORRIDO) posicionMundo = FIN_RECORRIDO;
    }

    void retroceder(int cantidad = 2) {
        posicionMundo -= cantidad;
        if (posicionMundo < 0) posicionMundo = 0;
    }

    int getPosicionMundo() const { return posicionMundo; }

    // Etapa que ocupa el borde IZQUIERDO de la pantalla.
    int getEtapa() const {
        if (posicionMundo >= INICIO_NOCTURNO) return 3;
        if (posicionMundo >= INICIO_INDUSTRIAL) return 2;
        return 1;
    }

    // Cada columna decide a que zona del mundo pertenece.
    // Por eso el mapa nuevo ENTRA DESDE LA DERECHA naturalmente.
    char getPixelFondo(int x, int fila) const {
        if (fila < 0 || fila >= 18 || x < 0 || x >= anchoPantalla) return ' ';

        int mundoX = posicionMundo + x;

        if (mundoX < INICIO_INDUSTRIAL)
            return ciudad(mundoX, fila);

        if (mundoX < INICIO_NOCTURNO)
            return industrial(mundoX, fila);

        return nocturno(mundoX, fila);
    }

    string getNombreMapa() const {
        int izquierda = posicionMundo;
        int derecha = posicionMundo + anchoPantalla - 1;

        if (izquierda < INICIO_INDUSTRIAL && derecha >= INICIO_INDUSTRIAL)
            return "CIUDAD -> ZONA INDUSTRIAL";
        if (izquierda < INICIO_NOCTURNO && derecha >= INICIO_NOCTURNO)
            return "ZONA INDUSTRIAL -> CARRETERA NOCTURNA";
        if (izquierda >= INICIO_NOCTURNO) return "CARRETERA NOCTURNA";
        if (izquierda >= INICIO_INDUSTRIAL) return "ZONA INDUSTRIAL";
        return "CIUDAD";
    }

    void dibujarBordeInferior() const {
        cout << string(anchoPantalla, '=') << "\n";
    }
};
