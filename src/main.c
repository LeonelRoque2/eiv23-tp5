#include "comunicador_serie.h"
#include "timer.h"
#include "stm32f1xx.h"
#include <interprete.h>
#include <stdbool.h>

void Clock_config ();

static char const * const fueraDeRango = "OUT";

static void obtenerDigitosATransmitir (uint32_t numero, char digitos[]) {
    for(int i=0;i<10;i++) {
        digitos[i] = (numero % 10) + '0';
        numero = numero / 10;
    }
}

static void transmitirOutOfRange () {
    unsigned i = 0;
        while (fueraDeRango[i] != 0) {
            esperarYTransmitir (fueraDeRango[i]);
            i++;
        }
}

static void escribirPorPantalla (uint32_t numero, char digitos []) {
    obtenerDigitosATransmitir(numero,digitos);
    bool noEsCero = 0;
    for (int i=0;i<10;i++) {
        if (digitos[9-i] != '0') {
            noEsCero = 1;
        }
        if (noEsCero) {
        esperarYTransmitir (digitos[9-i]);
        }
    }
}

void Comando_dist(void){
    static char digitosATransmitir[10];
    TIM4_generaPulso(2);
    uint32_t valor1 = TIM4_detectarFlanco (FLANCO_ASCENDENTE);
    uint32_t valor2 = TIM4_detectarFlanco (FLANCO_DESCENDENTE);
    uint32_t ancho_pulso = (valor2-valor1);
    if (ancho_pulso > 4000) {
        transmitirOutOfRange();
    }
    else {
        escribirPorPantalla(ancho_pulso,digitosATransmitir);
    }
    esperarYTransmitir('\r');
    esperarYTransmitir('\n');
}

int main(void){
    uint32_t caracter;
    Clock_config ();
    USART_init(9600);
    TIM4_init();
    Interprete_init();
    for(;;){
        bool read = USART_read(&caracter);
        if(read) {
            esperarYTransmitir(caracter);
            Interprete_procesa(caracter);
        }
    }
    return 0;
}

void Clock_config () {
    RCC->CR |= RCC_CR_HSEON;
    for(unsigned i=0;(i<10000) && !(RCC->CR & RCC_CR_HSERDY);++i);
    if (RCC->CR & RCC_CR_HSERDY){
        // Arrancó HSE, cambia a HSE
        RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_SW)) | RCC_CFGR_SW_HSE;
        while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSE);
    }else{
        RCC->CR &= ~RCC_CR_HSEON; // Falló el arranque de HSE, continúa con HSI
    }
    SystemCoreClockUpdate();
}
   

