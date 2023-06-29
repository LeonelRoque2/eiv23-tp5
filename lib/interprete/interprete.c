#include <interprete.h>
#include <comunicador_serie.h>
#include <ctype.h>
#include <stdbool.h>

static char const * const comando = "dist?";

static char const * const mensaje_error = "ERR";

typedef struct Interprete {
    enum Estado {ESPERA,COINCIDIENDO,ERR} estado;
    unsigned coincidencias;
} Interprete;

static Interprete interprete;

static void transmitirMensajeDeError (void) {
    unsigned i = 0;
    while (mensaje_error[i] != 0) {
        esperarYTransmitir (mensaje_error[i]);
        i++;
    }
}

void Interprete_init(void){
    interprete.coincidencias = 0;
    interprete.estado = ESPERA;
}


/**
 * @brief Compara un caracter recibido con el caracter que se esperaba recibir
 * 
 * @param c Caracter a comparar 
 * @return true coincide
 * @return false no coincide
 */
static bool coincideChar(char c){
    // Si coincide con el caracter que espera retorna 1 y
    // avanza coincidencias, si no retorna 0
    unsigned const i = interprete.coincidencias;
    if (comando[i] != 0) {
        if (tolower(c) == comando[i]) {
            interprete.coincidencias++;
            return true;
        }
    }
    else {
        if (c == ' ' || c == '\n' || c == '\r') {
            return true;
        }
    }
    return false;
    // Si comando[i] != 0
    //  Compara tolower(c) con comando[i], si coincide avanza coincidencias y retorna true, si no retorna false
    // si comando[i] == 0
    //  Si c es espacio en blanco \n o \r retorna true, si no retorna false
}

void Interprete_procesa(char c){
    switch(interprete.estado){
    case ESPERA:
        interprete.coincidencias = 0;
        if (c == ' ' || c == '\n' || c == '\r') {
            interprete.estado = ESPERA;
        }
        else {
            if (coincideChar(c)) {
                interprete.estado = COINCIDIENDO;
            }
            else {
                interprete.estado = ERR;
            }
        }
    break;case COINCIDIENDO:
        if (coincideChar(c)) {
            if (c == '\n') {
                Comando_dist();
                interprete.estado = ESPERA;
            }
        }
        else {
            interprete.estado = ERR;
        }
    break;case ERR:
        if(c == '\n'){
            transmitirMensajeDeError();
            interprete.estado = ESPERA;
        }
    break;default:
        interprete.estado = ERR;
    }
}