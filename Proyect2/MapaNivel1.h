#pragma once
#include <iostream>
#include <string>
#include "BackgroundMap.h"

using namespace std;

class MapaNivel1 {
private:
    int anchoPantalla;
    int posicionMundo;
    BackgroundMap fondo;
    bool fondoCargado;

    static const int INICIO_INDUSTRIAL = 180;
    static const int INICIO_NOCTURNO = 340;
    static const int FIN_RECORRIDO = 500;

public:
    MapaNivel1(int ancho)
        : anchoPantalla(ancho),
        posicionMundo(0),
        fondoCargado(false)
    {
        // Intenta cargar el fondo desde distintas ubicaciones
        // dependiendo de dónde Visual Studio ejecute el programa.

        fondoCargado = fondo.loadFromFile(
            "assets/backgrounds/Fondo_de_ciudad_Nivel_1.txt"
        );

        if (!fondoCargado) {
            fondoCargado = fondo.loadFromFile(
                "../assets/backgrounds/Fondo_de_ciudad_Nivel_1.txt"
            );
        }

        if (!fondoCargado) {
            fondoCargado = fondo.loadFromFile(
                "../../assets/backgrounds/Fondo_de_ciudad_Nivel_1.txt"
            );
        }

        if (!fondoCargado) {
            fondoCargado = fondo.loadFromFile(
                "Proyect2/assets/backgrounds/Fondo_de_ciudad_Nivel_1.txt"
            );
        }

        if (!fondoCargado) {
            cout << "ERROR: No se pudo cargar el fondo del Nivel 1.\n";
        }
    }

    bool estaFondoCargado() const {
        return fondoCargado;
    }

    void avanzar(int cantidad = 2) {
        posicionMundo += cantidad;

        if (posicionMundo > FIN_RECORRIDO) {
            posicionMundo = FIN_RECORRIDO;
        }
    }

    void retroceder(int cantidad = 2) {
        posicionMundo -= cantidad;

        if (posicionMundo < 0) {
            posicionMundo = 0;
        }
    }

    int getPosicionMundo() const {
        return posicionMundo;
    }

    int getEtapa() const {
        if (posicionMundo >= INICIO_NOCTURNO)
            return 3;

        if (posicionMundo >= INICIO_INDUSTRIAL)
            return 2;

        return 1;
    }

    char getPixelFondo(int x, int fila) const {
        if (!fondoCargado ||
            fila < 0 ||
            fila >= 18 ||
            x < 0 ||
            x >= anchoPantalla)
        {
            return ' ';
        }

        return fondo.getPixel(
            posicionMundo + x,
            fila
        );
    }

    string getNombreMapa() const {
        int izquierda = posicionMundo;
        int derecha = posicionMundo + anchoPantalla - 1;

        if (izquierda < INICIO_INDUSTRIAL &&
            derecha >= INICIO_INDUSTRIAL)
        {
            return "CIUDAD -> ZONA INDUSTRIAL";
        }

        if (izquierda < INICIO_NOCTURNO &&
            derecha >= INICIO_NOCTURNO)
        {
            return "ZONA INDUSTRIAL -> CARRETERA NOCTURNA";
        }

        if (izquierda >= INICIO_NOCTURNO)
            return "CARRETERA NOCTURNA";

        if (izquierda >= INICIO_INDUSTRIAL)
            return "ZONA INDUSTRIAL";

        return "CIUDAD";
    }

    void dibujarBordeInferior() const {
        cout << string(anchoPantalla, '=') << "\n";
    }
};