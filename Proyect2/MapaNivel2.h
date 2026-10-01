#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "BackgroundMap.h"

class MapaNivel2 {
private:
    std::vector<BackgroundMap> partes;
    int parteActual = 0;
    bool cargado = false;

    // Intenta encontrar cada parte del Nivel 2
    // desde las posibles carpetas de ejecución.
    bool cargarParte(BackgroundMap& mapa, const std::string& nombre) {

        // Ejecutando desde la raíz del repositorio
        if (mapa.loadFromFile(
            "assets/backgrounds/nivel2/" + nombre))
        {
            return true;
        }

        // Ejecutando desde la carpeta Proyect2
        if (mapa.loadFromFile(
            "../assets/backgrounds/nivel2/" + nombre))
        {
            return true;
        }

        // Ejecutando desde una carpeta más interna
        if (mapa.loadFromFile(
            "../../assets/backgrounds/nivel2/" + nombre))
        {
            return true;
        }

        // Ruta alternativa
        if (mapa.loadFromFile(
            "Proyect2/assets/backgrounds/nivel2/" + nombre))
        {
            return true;
        }

        return false;
    }

public:
    MapaNivel2() {

        const char* archivos[] = {
            "parte1.txt",
            "parte2.txt",
            "parte3.txt",
            "parte_final.txt"
        };

        cargado = true;

        for (const char* archivo : archivos) {

            BackgroundMap mapa;

            if (!cargarParte(mapa, archivo)) {
                cargado = false;

                // Nos indica exactamente qué archivo no encontró.
                std::cout
                    << "ERROR: No se pudo cargar "
                    << archivo
                    << " del Nivel 2.\n";
            }

            partes.push_back(mapa);
        }
    }

    bool estaCargado() const {
        return cargado;
    }

    int getParteActual() const {
        return parteActual;
    }

    int getCantidadPartes() const {
        return (int)partes.size();
    }

    bool siguienteParte() {

        if (parteActual + 1 >= (int)partes.size()) {
            return false;
        }

        ++parteActual;
        return true;
    }

    bool parteAnterior() {

        if (parteActual <= 0) {
            return false;
        }

        --parteActual;
        return true;
    }

    bool esUltimaParte() const {
        return parteActual == (int)partes.size() - 1;
    }

    char getPixelFondo(int x, int y) const {

        if (!cargado ||
            parteActual < 0 ||
            parteActual >= (int)partes.size())
        {
            return ' ';
        }

        return partes[parteActual].getPixel(x, y);
    }

    std::string getNombreMapa() const {

        if (esUltimaParte()) {
            return "NIVEL 2 - PARTE FINAL (BOCETO)";
        }

        return "NIVEL 2 - PARTE "
            + std::to_string(parteActual + 1)
            + " (BOCETO)";
    }
};