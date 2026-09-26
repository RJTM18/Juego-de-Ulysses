#include "pch.h"
#include <iostream>
#include <vector>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <chrono>
#ifdef _WIN32
#include <Windows.h>
#pragma comment(lib, "User32.lib")
#endif
#include "Jugador.h"
#include "Carro.h"
#include "EnemigoEspecial.h"
#include "MapaNivel1.h"
#include "GameManager.h"
using namespace std;

const int ANCHO_PANTALLA = 90;
const int FILAS_FONDO = 6;
const int FILAS_JUEGO = 12;
const int ALTO_TOTAL = FILAS_FONDO + FILAS_JUEGO;

// Cuando el jugador llega aqui, deja de avanzar visualmente hacia la derecha.
// Desde ese punto, la flecha DERECHA desplaza el mundo.
const int ZONA_CAMARA_X = 35;

// ==========================================================
// TEXTOS DE TRANSICION: ESCRIBE LO QUE QUIERAS ENTRE " "
// ==========================================================
const string TEXTO_FIN_MAPA_1 = "";
const string TEXTO_FIN_MAPA_2 = "";
const string TEXTO_FIN_MAPA_3 = "";

void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Dibuja una pausa SIN carros ni enemigo, pero mantiene jugador + mapa.
void esperarX(const string& texto, Jugador& jugador, MapaNivel1& mapa) {
    limpiarPantalla();

    cout << string(ANCHO_PANTALLA, '=') << "\n";
    cout << " ESCENARIO: " << mapa.getNombreMapa() << "   -   PAUSA\n";
    cout << string(ANCHO_PANTALLA, '=') << "\n";

    for (int fila = 0; fila < ALTO_TOTAL; fila++) {
        string linea(ANCHO_PANTALLA, ' ');

        for (int x = 0; x < ANCHO_PANTALLA; x++)
            linea[x] = mapa.getPixelFondo(x, fila);

        string sj = jugador.getLineaSprite(fila);
        for (size_t c = 0; c < sj.size(); c++) {
            int px = jugador.getX() + (int)c;
            if (px >= 0 && px < ANCHO_PANTALLA)
                linea[px] = sj[c];
        }

        cout << "|" << linea << "|\n";
    }

    mapa.dibujarBordeInferior();
    cout << "\n\"" << texto << "\"\n\n";
    cout << "Presiona X para continuar...";

    while (true) {
        int tecla = _getch();
        if (tecla == 'x' || tecla == 'X') break;
    }
}

int main() {
    srand((unsigned int)time(0));
    int opcion = 0;

    while (true) {
        limpiarPantalla();
        cout << "=========================================\n";
        cout << "              JUEGO RETRO                \n";
        cout << "=========================================\n\n";
        cout << "1. Jugar persecucion\n";
        cout << "2. Salir\n\n";
        cout << "Elige una opcion: ";
        cin >> opcion;

        if (opcion == 2) break;
        if (opcion != 1) continue;

        Jugador jugador(5, FILAS_FONDO);
        vector<Carro> carros;
        carros.push_back(Carro(ANCHO_PANTALLA, FILAS_FONDO, ANCHO_PANTALLA));
        carros.push_back(Carro(ANCHO_PANTALLA + 30, FILAS_FONDO + 6, ANCHO_PANTALLA));
        EnemigoEspecial enemigoEsp(ANCHO_PANTALLA);
        MapaNivel1 mapa(ANCHO_PANTALLA);
        GameManager manager;
        int ultimaDirHorizontal = 1;

        bool pausaMapa1Hecha = false;
        bool pausaMapa2Hecha = false;
        bool nivelCompletado = false;

        limpiarPantalla();
        cout << "=== PERSECUCION HORIZONTAL ===\n\n";
        cout << "FLECHA DERECHA: avanzar (puedes mantenerla presionada).\n";
        cout << "Al llegar a la zona de camara, TU quedas en pantalla y el MUNDO se mueve.\n";
        cout << "Si dejas de avanzar, el mapa se queda quieto.\n\n";
        cout << "Presiona cualquier tecla para comenzar...";
        _getch();

        while (jugador.getVidas() > 0 && !nivelCompletado) {

            // =====================================================
            // CONTROLES CONTINUOS (MANTENER FLECHA)
            // El jugador/mapa se mueven a un ritmo fijo independiente
            // del ritmo de los carros y del enemigo.
            // =====================================================
            static auto ultimoPaso = chrono::steady_clock::now();
            auto ahoraMovimiento = chrono::steady_clock::now();
            const int INTERVALO_MOVIMIENTO_MS = 80;

            bool puedeDarPaso =
                chrono::duration_cast<chrono::milliseconds>(
                    ahoraMovimiento - ultimoPaso
                ).count() >= INTERVALO_MOVIMIENTO_MS;

#ifdef _WIN32
            bool derecha = (GetAsyncKeyState(VK_RIGHT) & 0x8000) != 0;
            bool izquierda = (GetAsyncKeyState(VK_LEFT) & 0x8000) != 0;
            bool arriba = (GetAsyncKeyState(VK_UP) & 0x8000) != 0;
            bool abajo = (GetAsyncKeyState(VK_DOWN) & 0x8000) != 0;
#else
            bool derecha = false;
            bool izquierda = false;
            bool arriba = false;
            bool abajo = false;
#endif

            if (puedeDarPaso) {
                bool seMovio = false;

                if (derecha && !izquierda) {
                    ultimaDirHorizontal = 1;

                    // Primero el personaje avanza hasta la zona de camara.
                    if (jugador.getX() < ZONA_CAMARA_X) {
                        jugador.mover(2, 0, 1, ANCHO_PANTALLA, ALTO_TOTAL);
                    }
                    else {
                        // Luego queda aproximadamente fijo y avanza el mundo.
                        mapa.avanzar(2);
                        jugador.mover(0, 0, 1, ANCHO_PANTALLA, ALTO_TOTAL);
                    }
                    seMovio = true;
                }
                else if (izquierda && !derecha) {
                    ultimaDirHorizontal = -1;

                    if (mapa.getPosicionMundo() > 0 && jugador.getX() >= ZONA_CAMARA_X) {
                        mapa.retroceder(2);
                        jugador.mover(0, 0, -1, ANCHO_PANTALLA, ALTO_TOTAL);
                    }
                    else if (jugador.getX() - 2 >= 0) {
                        jugador.mover(-2, 0, -1, ANCHO_PANTALLA, ALTO_TOTAL);
                    }
                    seMovio = true;
                }

                // Arriba/abajo cambian de carril con el mismo ritmo controlado.
                if (arriba && !abajo) {
                    if (jugador.getY() - 3 >= FILAS_FONDO)
                        jugador.mover(0, -1, ultimaDirHorizontal, ANCHO_PANTALLA, ALTO_TOTAL);
                    seMovio = true;
                }
                else if (abajo && !arriba) {
                    if (jugador.getY() + 3 < ALTO_TOTAL)
                        jugador.mover(0, 1, ultimaDirHorizontal, ANCHO_PANTALLA, ALTO_TOTAL);
                    seMovio = true;
                }

                if (seMovio)
                    ultimoPaso = ahoraMovimiento;
            }

            // =====================================================
            // PAUSAS POR DISTANCIA RECORRIDA, NO POR TIEMPO
            // El mapa nuevo ya ocupa toda la pantalla cuando llegamos
            // a estas posiciones.
            // =====================================================
            if (!pausaMapa1Hecha && mapa.getPosicionMundo() >= 180) {
                pausaMapa1Hecha = true;
                esperarX(TEXTO_FIN_MAPA_1, jugador, mapa);
            }

            if (!pausaMapa2Hecha && mapa.getPosicionMundo() >= 340) {
                pausaMapa2Hecha = true;
                esperarX(TEXTO_FIN_MAPA_2, jugador, mapa);
            }

            // =====================================================
            // FINAL DEL RECORRIDO NOCTURNO
            // =====================================================
            if (mapa.getPosicionMundo() >= 500) {
                esperarX(TEXTO_FIN_MAPA_3, jugador, mapa);
                nivelCompletado = true;
                break;
            }

            // =====================================================
            // ENEMIGOS (se mueven independientemente del fondo)
            // =====================================================
            for (auto& carro : carros) carro.avanzar();
            enemigoEsp.avanzar();
            manager.verificarEventos(jugador, carros, enemigoEsp);

            // =====================================================
            // RENDER
            // =====================================================
            limpiarPantalla();
            cout << string(ANCHO_PANTALLA, '=') << "\n";
            manager.dibujarVidasSuperiores(jugador);
            cout << " ESCENARIO: " << mapa.getNombreMapa()
                 << "   DISTANCIA: " << mapa.getPosicionMundo() << "\n";
            cout << string(ANCHO_PANTALLA, '=') << "\n";

            for (int fila = 0; fila < ALTO_TOTAL; fila++) {
                string linea(ANCHO_PANTALLA, ' ');

                for (int x = 0; x < ANCHO_PANTALLA; x++)
                    linea[x] = mapa.getPixelFondo(x, fila);

                string sj = jugador.getLineaSprite(fila);
                for (size_t c = 0; c < sj.size(); c++) {
                    int px = jugador.getX() + (int)c;
                    if (px >= 0 && px < ANCHO_PANTALLA) linea[px] = sj[c];
                }

                for (auto& carro : carros) {
                    string sc = carro.getLineaSprite(fila);
                    for (size_t c = 0; c < sc.size(); c++) {
                        int px = carro.getX() + (int)c;
                        if (px >= 0 && px < ANCHO_PANTALLA) linea[px] = sc[c];
                    }
                }

                string se = enemigoEsp.getLineaSprite(fila);
                for (size_t c = 0; c < se.size(); c++) {
                    int px = enemigoEsp.getX() + (int)c;
                    if (px >= 0 && px < ANCHO_PANTALLA) linea[px] = se[c];
                }

                cout << "|" << linea << "|\n";
            }

            mapa.dibujarBordeInferior();
            cout << " Flecha -> : avanzar | Flecha <- : retroceder | Arriba/Abajo: cambiar carril\n";
            manager.dibujarInterfazInferior(ANCHO_PANTALLA);
            manager.regularVelocidad(jugador);

        }

        limpiarPantalla();
        if (nivelCompletado) {
            cout << "\n=============================================\n";
            cout << "          PERSECUCION COMPLETADA             \n";
            cout << "=============================================\n";
        }
        else {
            cout << "\n=============================================\n";
            cout << "                  GAME OVER                  \n";
            cout << "=============================================\n";
            cout << "Te quedaste sin vidas.\n";
        }

        cout << "\nPresiona cualquier tecla para volver al menu...";
        _getch();
    }

    return 0;
}
