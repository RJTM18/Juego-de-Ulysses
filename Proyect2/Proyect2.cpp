#include "pch.h"
#include <iostream>
#include <vector>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <cctype>
#ifdef _WIN32
#include <Windows.h>
#endif
#include "Jugador.h"
#include "Carro.h"
#include "EnemigoEspecial.h"
#include "MapaNivel1.h"
#include "GameManager.h"
#include "PersonajeElegante.h"
#include "ContinuousCameraStrategy.h"
#include "MapaNivel2.h"
#include "SegmentedCameraStrategy.h"
using namespace std;

const int ANCHO_PANTALLA = 90;
const int FILAS_FONDO = 6; //cambiar esto
const int FILAS_JUEGO = 12;
const int ALTO_TOTAL = FILAS_FONDO + FILAS_JUEGO;

// Cuando el jugador llega aqui, deja de avanzar visualmente hacia la derecha.
// Desde ese punto, la flecha DERECHA desplaza el mundo.
const int ZONA_CAMARA_X = 35;

// ==========================================================
// TEXTOS DE TRANSICION: ESCRIBE LO QUE QUIERAS ENTRE " "
// ==========================================================
const string TEXTO_FIN_MAPA_1 = "Bloom: Ese hombre vendra, Cierto?. Vendra a visitar a Molly. Sera mejor que camine";
const string TEXTO_FIN_MAPA_2 = "Bloom: Que recuerde, en tal biblioteca, un chico llamado Stephen dara un ensayo acerca de las obras de Shakespeare";
const string TEXTO_FIN_MAPA_3 = "Bloom: He escuchado que Sandymount Strand se encuentran mujeres bellas, podria echarlas un vistaso";

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


// ==========================================================
// CHARLA FINAL: TODO ESTO QUEDA LIBRE PARA QUE LO ESCRIBAS.
// Cada opcion del jugador tiene una respuesta propia del personaje.
// ==========================================================
const string DIALOGO_1 = "   ";
const string OPCION_1A = "   ";
const string OPCION_1B = "   ";
const string OPCION_1C = "   ";
const string RESPUESTA_1A = "   ";
const string RESPUESTA_1B = "   ";
const string RESPUESTA_1C = "   ";

const string DIALOGO_2 = "   ";
const string OPCION_2A = "   ";
const string OPCION_2B = "   ";
const string OPCION_2C = "   ";
const string RESPUESTA_2A = "   ";
const string RESPUESTA_2B = "   ";
const string RESPUESTA_2C = "   ";

const string DIALOGO_3 = "   ";
const string OPCION_3A = "   ";
const string OPCION_3B = "   ";
const string OPCION_3C = "   ";
const string RESPUESTA_3A = "   ";
const string RESPUESTA_3B = "   ";
const string RESPUESTA_3C = "   ";

void turnoDialogo(const string& frase, const string& a, const string& b, const string& c,
                  const string& ra, const string& rb, const string& rc) {
    limpiarPantalla();
    cout << "=============================================\n";
    cout << "                 CONVERSACION                \n";
    cout << "=============================================\n\n";
    cout << "        _=|=_\n";
    cout << "         (|) \n";
    cout << "         / \\\n\n";
    cout << "Personaje: " << frase << "\n\n";
    cout << "A) " << a << "\n";
    cout << "B) " << b << "\n";
    cout << "C) " << c << "\n\n";
    cout << "Tu respuesta: ";

    char e;
    while (true) {
        e = (char)_getch();
        e = (char)toupper(e);
        if (e == 'A' || e == 'B' || e == 'C') break;
    }
    cout << e << "\n\n";
    cout << "Personaje: ";
    if (e == 'A') cout << ra;
    else if (e == 'B') cout << rb;
    else cout << rc;
    cout << "\n\nPresiona cualquier tecla para continuar...";
    _getch();
}

void conversacionFinal() {
    turnoDialogo(DIALOGO_1, OPCION_1A, OPCION_1B, OPCION_1C, RESPUESTA_1A, RESPUESTA_1B, RESPUESTA_1C);
    turnoDialogo(DIALOGO_2, OPCION_2A, OPCION_2B, OPCION_2C, RESPUESTA_2A, RESPUESTA_2B, RESPUESTA_2C);
    turnoDialogo(DIALOGO_3, OPCION_3A, OPCION_3B, OPCION_3C, RESPUESTA_3A, RESPUESTA_3B, RESPUESTA_3C);
}


// ==========================================================
// NIVEL 2 - BOCETO FUNCIONAL
// Misma base de movimiento del Nivel 1, pero SIN scroll continuo.
// El escenario cambia por pantallas completas (partes).
// Los enemigos y dialogos quedan como puntos de extension para
// definirlos despues sin inventar contenido que aun no fue acordado.
// ==========================================================
void jugarNivel2Boceto() {
    Jugador jugador(5, FILAS_FONDO);
    MapaNivel2 mapa;
    SegmentedCameraStrategy camera;
    int ultimaDirHorizontal = 1;

    if (!mapa.estaCargado()) {
        limpiarPantalla();
        cout << "ERROR: No se pudieron cargar las partes TXT del Nivel 2.\n";
        cout << "Presiona cualquier tecla para volver al menu...";
        _getch();
        return;
    }

    limpiarPantalla();
    cout << "=============================================\n";
    cout << "             NIVEL 2 - BOCETO               \n";
    cout << "=============================================\n\n";
    cout << "Este nivel funciona POR PARTES.\n";
    cout << "Al llegar al borde derecho se carga la siguiente pantalla.\n";
    cout << "Al volver por el borde izquierdo regresas a la anterior.\n\n";
    cout << "Enemigos, historia y arte final: pendientes de definir.\n\n";
    cout << "Presiona cualquier tecla para comenzar...";
    _getch();

    bool terminado = false;
    auto ultimoPaso = chrono::steady_clock::now();

    while (!terminado) {
        auto ahora = chrono::steady_clock::now();
        bool puedeDarPaso = chrono::duration_cast<chrono::milliseconds>(ahora - ultimoPaso).count() >= 80;
        bool derecha = false, izquierda = false, arriba = false, abajo = false;

        if (_kbhit()) {
            int tecla = _getch();
            if (tecla == 0 || tecla == 224) {
                tecla = _getch();
                if (tecla == 77) derecha = true;
                else if (tecla == 75) izquierda = true;
                else if (tecla == 72) arriba = true;
                else if (tecla == 80) abajo = true;
            }
            else {
                if (tecla == 'd' || tecla == 'D') derecha = true;
                else if (tecla == 'a' || tecla == 'A') izquierda = true;
                else if (tecla == 'w' || tecla == 'W') arriba = true;
                else if (tecla == 's' || tecla == 'S') abajo = true;
                else if (tecla == 'x' || tecla == 'X') terminado = true;
            }
        }

        if (puedeDarPaso && !terminado) {
            bool seMovio = false;
            int parteAntes = mapa.getParteActual();

            if (derecha && !izquierda) {
                ultimaDirHorizontal = 1;
                camera.moverDerecha(jugador, mapa, ANCHO_PANTALLA, ALTO_TOTAL);
                seMovio = true;

                // En la ultima parte, alcanzar el borde termina SOLO este boceto.
                if (mapa.esUltimaParte() && parteAntes == mapa.getParteActual() &&
                    jugador.getX() >= ANCHO_PANTALLA - 6)
                    terminado = true;
            }
            else if (izquierda && !derecha) {
                ultimaDirHorizontal = -1;
                camera.moverIzquierda(jugador, mapa, ANCHO_PANTALLA, ALTO_TOTAL);
                seMovio = true;
            }

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

            if (seMovio) ultimoPaso = ahora;
        }

        if (terminado) break;

        limpiarPantalla();
        cout << string(ANCHO_PANTALLA, '=') << "\n";
        cout << " " << mapa.getNombreMapa()
             << "   [" << mapa.getParteActual() + 1 << "/" << mapa.getCantidadPartes() << "]\n";
        cout << string(ANCHO_PANTALLA, '=') << "\n";

        for (int fila = 0; fila < ALTO_TOTAL; ++fila) {
            string linea(ANCHO_PANTALLA, ' ');
            for (int x = 0; x < ANCHO_PANTALLA; ++x)
                linea[x] = mapa.getPixelFondo(x, fila);

            string sj = jugador.getLineaSprite(fila);
            for (size_t c = 0; c < sj.size(); ++c) {
                int px = jugador.getX() + (int)c;
                if (px >= 0 && px < ANCHO_PANTALLA) linea[px] = sj[c];
            }
            cout << "|" << linea << "|\n";
        }

        cout << string(ANCHO_PANTALLA, '=') << "\n";
        cout << " Flechas/WASD: mover | X: salir del boceto | Al borde: cambia de PARTE\n";
        Sleep(16);
    }

    limpiarPantalla();
    cout << "=============================================\n";
    cout << "       FIN DEL BOCETO FUNCIONAL NIVEL 2      \n";
    cout << "=============================================\n\n";
    cout << "La estructura por partes ya funciona.\n";
    cout << "El contenido definitivo queda libre para completarlo despues.\n\n";
    cout << "Presiona cualquier tecla para volver al menu...";
    _getch();
}

int main() {
    srand((unsigned int)time(0));
    int opcion = 0;

    while (true) {
        limpiarPantalla();
        cout << "=========================================\n";
        cout << "                 Ullyses                 \n";
        cout << "=========================================\n\n";
        cout << "1. Iniciar Nuevo Juego\n";
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
        if (!mapa.estaFondoCargado()) {
            limpiarPantalla();
            cout << "ERROR: No se pudo cargar assets/backgrounds/Fondo_de_ciudad_Nivel_1.txt\n";
            cout << "Presiona cualquier tecla para volver al menu...";
            _getch();
            continue;
        }
        GameManager manager;
        ContinuousCameraStrategy camera;
        int ultimaDirHorizontal = 1;

        bool pausaMapa1Hecha = false;
        bool pausaMapa2Hecha = false;
        bool nivelCompletado = false;

        limpiarPantalla();
        cout << "=== PERSECUCION HORIZONTAL ===\n\n";
        cout << "FLECHA DERECHA: avanzar (puedes mantenerla presionada).\n";
        cout << "Al llegar a la zona de camara, TU quedas en pantalla y el MUNDO se mueve.\n";
        cout << "Si dejas de avanzar, el mapa se queda quieto.\n\n";
        cout << "Presiona cualquier tecla para comenzar...\n\n";
        cout << "Introduccion: En un dia como cualquier otro, 16 de junio de 1904 en la ciudad de dublin, Leopold Bloom, esposo de molly, sale de su casa en direccion al trabajo. Por este dia, decide tomar la ruta mas larga...";
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

            bool derecha = false;
            bool izquierda = false;
            bool arriba = false;
            bool abajo = false;

            // Teclado con _kbhit() y _getch().
            // Flechas y WASD siguen funcionando.
            if (_kbhit()) {
                int tecla = _getch();
                if (tecla == 0 || tecla == 224) {
                    tecla = _getch();
                    if (tecla == 77) derecha = true;
                    else if (tecla == 75) izquierda = true;
                    else if (tecla == 72) arriba = true;
                    else if (tecla == 80) abajo = true;
                }
                else {
                    if (tecla == 'd' || tecla == 'D') derecha = true;
                    else if (tecla == 'a' || tecla == 'A') izquierda = true;
                    else if (tecla == 'w' || tecla == 'W') arriba = true;
                    else if (tecla == 's' || tecla == 'S') abajo = true;
                }
            }

            if (puedeDarPaso) {
                bool seMovio = false;

                if (derecha && !izquierda) {
                    ultimaDirHorizontal = 1;

                    // La estrategia de camara decide si avanza Bloom o el mundo.
                    camera.moverDerecha(jugador, mapa, ZONA_CAMARA_X, ANCHO_PANTALLA, ALTO_TOTAL);
                    seMovio = true;
                }
                else if (izquierda && !derecha) {
                    ultimaDirHorizontal = -1;

                    camera.moverIzquierda(jugador, mapa, ZONA_CAMARA_X, ANCHO_PANTALLA, ALTO_TOTAL);
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
                // Conserva el tercer texto de historia original.
                esperarX(TEXTO_FIN_MAPA_3, jugador, mapa);

                // Despues de haberlo visto acercarse en el mapa, empieza la charla.
                conversacionFinal();

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

                // El personaje elegante aparece al final del camino y se acerca
                // visualmente mientras la camara sigue avanzando.
                if (mapa.getPosicionMundo() >= 440) {
                    int npcX = 82 - (mapa.getPosicionMundo() - 440) / 2;
                    if (npcX < 48) npcX = 48;
                    PersonajeElegante personajeFinal(npcX, FILAS_FONDO + 3);
                    string sp = personajeFinal.getLineaSprite(fila);
                    for (size_t c = 0; c < sp.size(); c++) {
                        int px = personajeFinal.getX() + (int)c;
                        if (px >= 0 && px < ANCHO_PANTALLA) linea[px] = sp[c];
                    }
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
            cout << "                NIVEL COMPLETADO             \n";
            cout << "=============================================\n\n\n";
            cout << "Bloom llega a Nightown, el barrio rojo de Dublin, donde se encuentran con Stephen y pasan el rato. Despues de varios 'episodios', Stephen queda desorientado, por lo que Bloom debera guiarle el camino";
                
        }
        else {
            cout << "\n=============================================\n";
            cout << "                  GAME OVER                  \n";
            cout << "=============================================\n";
            cout << "Te quedaste sin vidas.\n";
        }

        if (nivelCompletado) {
            cout << "\nPresiona cualquier tecla para continuar al boceto del Nivel 2...";
            _getch();
            jugarNivel2Boceto();
        }
        else {
            cout << "\nPresiona cualquier tecla para volver al menu...";
            _getch();
        }
    }

    return 0;
}
