#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <conio.h>
#include "Jugador.h"
#include "Carro.h"
#include "EnemigoEspecial.h"
using namespace std;

class GameManager {
private:
    bool jugadorGolpeado;
public:
    GameManager() { jugadorGolpeado = false; }

    void verificarEventos(Jugador& jugador, vector<Carro>& carros, EnemigoEspecial& enemigoEsp) {
        jugadorGolpeado = false;
        for (auto& carro : carros) {
            if (carro.verificaColision(jugador.getX(), jugador.getY())) {
                jugador.perderVida();
                carro.resetearPosicion();
                jugadorGolpeado = true;
                return;
            }
        }
        if (enemigoEsp.verificaColision(jugador.getX(), jugador.getY())) {
            jugador.perderVida();
            enemigoEsp.resetearPosicion();
            jugadorGolpeado = true;
        }
    }

    void dibujarVidasSuperiores(const Jugador& jugador) const {
        cout << " VIDAS: ";
        for (int v = 0; v < jugador.getVidas(); v++) cout << "<3 ";
        cout << "\n";
    }

    void dibujarInterfazInferior(int anchoPantalla) const {
        cout << string(anchoPantalla, '-') << "\n";
        if (jugadorGolpeado) cout << "                 >>> AUCH! PERDISTE UNA VIDA! <<<\n";
        else cout << "                 ESQUIVA EL TRAFICO Y SIGUE CORRIENDO!\n";
    }

    void regularVelocidad(const Jugador& jugador) const {
        if (jugadorGolpeado && jugador.getVidas() > 0) {
            this_thread::sleep_for(chrono::milliseconds(500));
            while (_kbhit()) _getch();
        } else {
            this_thread::sleep_for(chrono::milliseconds(45));
        }
    }
};
