#include "stm32f1xx.h"
#include "timer.h"

#define PREESCALER 470UL

/**
 * @brief Reinicio la configuracion del timer 2
 * 
 */
static void TIM2_reset () {
    RCC->APB1RSTR |= RCC_APB1RSTR_TIM2RST;
    RCC->APB2RSTR = 0; 
}
    

void TIM2_init (){
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;     //Activo el clock del timer 2
    TIM2_reset();                            
    TIM2->PSC = PREESCALER;                 //Configuro el prescaler del timer
    TIM2->CR1 |= TIM_CR1_CEN;               //Habilito el contador
}

void TIM2_setCompare (uint32_t tiempo){
    TIM2->CCMR1 = (TIM2->CCMR1 & ~(TIM_CCMR1_OC2M_Msk)) | TIM_CCMR1_OC2M_2 ;        //Fuerzo el valor de la salida en 1 
    TIM2->CCR2 =  (TIM2->CCR2 & ~(TIM_CCR2_CCR2_Msk)) | (TIM2->CNT + tiempo);       //Guardo el valor de comparacion en el registro capture/compare del canal 2
    TIM2->CCMR1 = (TIM2->CCMR1 & ~(TIM_CCMR1_OC2M_Msk)) | TIM_CCMR1_OC2M_1;          //Pongo la salida en 0 cuando haya un match
    TIM2->CCER |= TIM_CCER_CC2E;                                                    //Habilito modo Capture/Compare en el canal 2 del timer 2
    TIM2->EGR |= TIM_EGR_CC2G;                                                      //Habilito la generacion de eventos Capture/Compare en el canal 2 del timer 2
}

void TIM2_setCapture () {
    TIM2->CCMR1 |= TIM_CCMR1_CC1S_0;        //Configuro el canal 1 del timer 2 como entrada, y lo conecto a la entrada TI1         
    TIM2->CCER |= TIM_CCER_CC1E;            //Habilito modo Capture/Compare en el canal 1 del timer 2
    TIM2->DIER |= TIM_DIER_CC1IE;           //Habilito interrupciones Capture/compare en el canal 1 del timer 2
    TIM2->EGR |= TIM_EGR_CC1G;              //Habilito la generacion de eventos Capture/Compare en el canal 1 del timer 2
}

void TIM2_deInit (){
    TIM2_reset();   
    RCC->APB1ENR &= ~(RCC_APB1ENR_TIM2EN);  //Deshabilito el clock del Timer 2
}

void TIM2_IRQHandler () {

}