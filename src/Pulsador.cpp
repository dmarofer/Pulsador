/* 

# Pulsador.h V 1.0

Libreria de clase Pulsador para implementar la lectura de un pulsador o switch en entornos Arduino con configuracion de tiempo de debounce
Author: Diego Maroto - BilbaoMakers 2020 - info@bilbaomakers.org - dmarofer@diegomaroto.net
https://bilbaomakers.org/

Licencia: GNU General Public License v3.0 - https://www.gnu.org/licenses/gpl-3.0.html

*/

#include <Pulsador.h>
#include "Arduino.h"


Pulsador::Pulsador(int pin, int modo, unsigned long DebounceTimeMS, unsigned long HoldTimeMS, bool invert){
   	
    pinswitch = pin;
	pinMode(pinswitch, modo);
	Estado_Interno = SWITCH_OFF;
	Evento_Pulsador = PUL_IDLE;
	millis_debounce = 0;
	debouncetime = DebounceTimeMS;
	holdtime = HoldTimeMS;
	invertir=invert;

}


void Pulsador::SetCallbackEventos(EVENTOS_CALLBACK_SIGNATURE){

	this->MiCallbackEventoPulsador = MiCallbackEventoPulsador;

}


void Pulsador::Run(){

   	// Leo fisicamente el switch. Le aplico el invertir para que en el software la logica siempre sea afirmativa
	int l_lecturareal = digitalRead(pinswitch) ^ invertir;

	// Si el estado leido es distinto del interno y no estamos en Debouncing, entramos a modo debouncing, la lectura real no nos interesa aun
	if (l_lecturareal != Estado_Interno && Estado_Interno != SWITCH_DEBOUNCING){

		Estado_Interno = SWITCH_DEBOUNCING;
		millis_debounce = millis();

	}

	// Si estamos en estado debouncing y ha pasado el tiempo, ya podemos hacer caso a lo que diga la lectura
	// AQUI ESTAMOS DETECTANDO YA UN CAMBIO DE ESTADO, SOLO SE DISPARA AL CAMBIO REAL DE LA ENTRADA
	if (Estado_Interno == SWITCH_DEBOUNCING && (millis() - millis_debounce) >= debouncetime){

		// Leemos un Cambio a 1 REAL
		if (l_lecturareal == HIGH) {

			Estado_Interno = SWITCH_ON;

			// Ahora cuando leemos un CAMBIO A 1 habra que hacer cosas depende de donde este la maquina de estado del boton
			// Si todo esta correcto forzosamente estaremos en IDLE asi que no hay que mirar mucho, pasamos a PULSACION_DETECTADA
			// Este estado es INMEDIATO y sirve para Switches de HOME por ejemplo que requieren respuesta inmediata

			Evento_Pulsador = PUL_PULSACION_DETECTADA;	// Pongo el estado interno
			MiCallbackEventoPulsador(Evento_Pulsador);	// Lo lanzo con la funcion Callback


		}

		// Leemos un cambio a 0 real
		else {
			
			Estado_Interno = SWITCH_OFF;

			// Ahora cuando leemos un 0 habra que hacer cosas depende de donde este la maquina de estado del boton
			// Cambio a cero podemos detectar en 3 de los estados (Pulsacion_Detectada, Pulsacion_simple y HOLD)
			
			switch (Evento_Pulsador){

				// Si suelto en estado pulsacion detectada, envio evento Pulsacion_Simple
				// Este evento es util, junto con HOLD para botones de usuario que tienen varias funciones
				// Tambien se puede implementar en el futuro pulsaciones Multiples
				case PUL_PULSACION_DETECTADA:
					
					Evento_Pulsador = PUL_PULSACION_SIMPLE;
					MiCallbackEventoPulsador(Evento_Pulsador);	// Lo lanzo con la funcion Callback

				break;
				
				// En el resto de estados, al soltar vuelvo a IDLE
				default:
					
					Evento_Pulsador = PUL_IDLE;
					MiCallbackEventoPulsador(Evento_Pulsador);	// Lo lanzo con la funcion Callback

				break;

			}

		}
		
	}

	// Y aqui ya fuera del debouncin porque esto ya no depende del cambio de estado del boton fisico ...

	// Si llevamos en PULSACION_DETECTADA mas del tiempo de HOLD
	if (Evento_Pulsador == PUL_PULSACION_DETECTADA && (millis()-millis_debounce)>= holdtime){

		Evento_Pulsador = PUL_HOLD;
		MiCallbackEventoPulsador(Evento_Pulsador);	// Lo lanzo con la funcion Callback

	}

	// Si estamos en Pulsacion Simple, como este estado ya ha sido por cambio a 0, hay que cambairlo a IDLE a mano.
	if (Evento_Pulsador == PUL_PULSACION_SIMPLE){

		Evento_Pulsador = PUL_IDLE;
		MiCallbackEventoPulsador(Evento_Pulsador);	// Lo lanzo con la funcion Callback

	}
	
}