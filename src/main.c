#include <soporte_placa.h>
#include "comunicador_serie.h"

uint8_t *caracter;

int main(void){

    SP_init();
    USART_init(9600);
    for(;;){
        bool read = USART_read (caracter);
        if(read) {
            bool write = USART_write_byte (*caracter);}
        return 0;}
}