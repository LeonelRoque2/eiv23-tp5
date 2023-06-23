#include "comunicador_serie.h"
#include "timer.h"
#include "stm32f1xx.h"

void Clock_config ();

int main(void){
    uint32_t caracter;
    Clock_config ();
    USART_init(9600);
    TIM4_init();
    for(;;){
        bool read = USART_read(&caracter);
        if(read) {
            (void) USART_write_byte (caracter);
            TIM4_generaPulso(1);
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
   

/*
for(;;){
        bool read = USART_read(&caracter);
    if(read && caracter == 'M') {
        (void) USART_write_byte (caracter);
        TIM4_generaPulso(1);
        valor1 = TIM4_detectarFlanco (FLANCO_ASCENDENTE);
        valor2 = TIM4_detectarFlanco (FLANCO_DESCENDENTE);
        medida = (valor2-valor1)/4
        USART_writeByte ('\n');
        USART_writeByte (medida);
    }
        }
*/