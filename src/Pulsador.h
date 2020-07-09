/* 

# Pulsador.h V 1.0

Libreria de clase Pulsador para implementar la lectura de un pulsador o switch en entornos Arduino con configuracion de tiempo de debounce

- La entrada fisica se lee permanenetemente cada vez que se ejecuta el RUN
- Cuando se detecta un cambio en la entrada con respecto al estado LOGICO se espera un tiempo de debounce. Si tras el sigue el estado fisico  se cambia el estado logico




Author: Diego Maroto - BilbaoMakers 2020 - info@bilbaomakers.org - dmarofer@diegomaroto.net
https://bilbaomakers.org/

Licencia: GNU General Public License v3.0 - https://www.gnu.org/licenses/gpl-3.0.html

*/

#pragma once

#include "Arduino.h"

// Otra forma de pasar las funciones callback
// Esta es la forma correcta, una funcion normal sin tipo especifico mio, si no el std::bind NO Funciona
// Defino aqui esto para no volverme loco y escribir esto varias veces, que curre el compilador
#define EVENTOS_CALLBACK_SIGNATURE std::function<void(unsigned int)> MiCallbackEventoPulsador

class Pulsador{

private:
        
    // Definicion de la funcion de Callback   
    EVENTOS_CALLBACK_SIGNATURE;

    enum Tipo_Estado_Interno {

		SWITCH_OFF,
		SWITCH_ON,
        SWITCH_DEBOUNCING,
        		
	}Estado_Interno;

    // Variables internas para tiempos de debounce y hold
    unsigned long debouncetime;
    unsigned long holdtime;

    // Variable parcial para el debounce
    unsigned long millis_debounce;

    // Variable para el numero de pin entre funciones
    int pinswitch;

    // Variable para invertir la logica
    bool invertir;

public:

    enum TipoEventoPulsador {

        PUL_IDLE,
        PUL_PULSACION_DETECTADA,
        PUL_PULSACION_SIMPLE,
        PUL_HOLD,

    }Evento_Pulsador;

    // Constructor. 
    Pulsador(int pin, int modo, unsigned long DebounceTimeMS, unsigned long HoldTimeMS, bool invert);
    ~Pulsador(){};

    void SetCallbackEventos(EVENTOS_CALLBACK_SIGNATURE);          // Para pasarme la funcion de callback
    
    // Ejecutar en loop o en una task rapidito
    void Run();
    

};
