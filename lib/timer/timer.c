#include "stm32f1xx.h"
#include "timer.h"
#include "soporte_placa.h"

#define PREESCALER 46UL

typedef enum TIM_OCM{
    OCM_FROZEN            = 0b000,
    OCM_ACTIVE_ON_MATCH   = 0b001,
    OCM_INACTIVE_ON_MATCH = 0b010,
    OCM_TOGGLE_ON_MATCH   = 0b011,
    OCM_FORCE_INACTIVE    = 0b100,
    OCM_FORCE_ACTIVE      = 0b101,
    OCM_PWM_1             = 0b110,
    OCM_PWM_2             = 0b111
}TIM_OCM;

static void TIM4_setOC1M(TIM_OCM modo);

/**
 * @brief Reinicio la configuracion del timer 4
 * 
 */
static void TIM4_reset () {
    RCC->APB1RSTR = RCC_APB1RSTR_TIM4RST;
    RCC->APB1RSTR = 0; 
}
    
static void pinConfig (void) {
    SP_Pin_setModo(SP_PB6,SP_PIN_SALIDA);
    SP_Pin_setModo(SP_PB7,SP_PIN_ENTRADA);
}

static void TIM4_CC2S_config (void) {
    TIM4->CCMR1 = (TIM4->CCMR1 & ~(TIM_CCMR1_CC2S)) | (0b01 << TIM_CCMR1_CC2S_Pos); //Configuro el canal 2 del timer 4 como entrada, y lo conecto a la entrada TI1
}

void TIM4_init (){
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;     //Activo el clock del timer 4  
    TIM4_reset(); 
    TIM4->PSC = PREESCALER;                 //Configuro el prescaler del timer
    TIM4->CR1 |= TIM_CR1_CEN;               //Habilito el contador
    TIM4_setOC1M(OCM_FORCE_INACTIVE);
    TIM4_CC2S_config();                    
    TIM4->CCER |= TIM_CCER_CC1E;            //Habilito modo Capture/Compare en el canal 1 del timer 4
    TIM4->CCER |= TIM_CCER_CC2E;            //Habilito modo Capture/Compare en el canal 2 del timer 4                         
    pinConfig ();
}

static void TIM4_setFlancoADetectar (const flancos flanco) {
    if (flanco == FLANCO_DESCENDENTE) {         
        TIM4->CCER |= TIM_CCER_CC2P;                                    //Captura por flanco descendente
    }
    else {
        TIM4->CCER &= ~(TIM_CCER_CC2P);                                 //Captura por flanco ascendente
    }  
    TIM4->SR &= ~TIM_SR_CC2IF;
}

uint32_t TIM4_detectarFlanco (flancos flanco) {
    uint32_t valor_capturado;  
    TIM4_setFlancoADetectar (flanco);
    
    while (!(TIM4->SR & TIM_SR_CC2IF));

    valor_capturado = TIM4->CCR2;
    return(valor_capturado);
}


static void TIM4_setOC1M(TIM_OCM modo){
    TIM4->CCMR1 = (TIM4->CCMR1 & ~(TIM_CCMR1_OC1M)) | (modo << TIM_CCMR1_OC1M_Pos);
}
static void TIM4_setCCR1(uint16_t valor){
    TIM4->CCR1 = valor;
}
static void TIM4_resetCounterAndUpdate(void){
    TIM4->EGR |= TIM_EGR_UG;
}
static void TIM4_esperaFinReset(void){
    while(TIM4->EGR & TIM_EGR_UG);
}
void TIM4_generaPulso (uint32_t ciclos){
    TIM4_resetCounterAndUpdate(); // reinicia contador y prescaler
    TIM4_setOC1M(OCM_FORCE_ACTIVE);
    TIM4_esperaFinReset(); // Asegura que se haya reiniciado el contador
    TIM4_setCCR1(TIM4->CNT + ciclos);
    TIM4_setOC1M(OCM_INACTIVE_ON_MATCH);
}

void TIM4_deInit (){
    TIM4_reset();   
    RCC->APB1ENR &= ~(RCC_APB1ENR_TIM4EN);  //Deshabilito el clock del Timer 2
}

void TIM4_IRQHandler (void) {

}
