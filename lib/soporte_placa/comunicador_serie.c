#include "stm32f1xx.h"
#include "comunicador_serie.h"

/**
 * @brief Reinicia el puerto USART1
 * 
 */
static void USART_reset () {
    RCC->APB2RSTR = RCC_APB2RSTR_USART1RST;                                       
    RCC->APB2RSTR = 0;                                          
}

void USART_init (unsigned long const baudrate) {
 
    uint32_t const PA9_SALIDA_50MHZ_Msk = (0b1011 << GPIO_CRH_MODE9_Pos);
    uint32_t const PA10_ENTRADA_FLOTANTE_Msk = (0b0100 << GPIO_CRH_MODE10_Pos);
    SystemCoreClockUpdate();                                            //Revisa la configuracion de clock y suelta la frecuencia del mismo  
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN;          //Activa el clock tanto del puerto A como del USART1
    GPIOA->CRH = (GPIOA->CRH & ~(0xF << GPIO_CRH_MODE9_Pos)) | PA9_SALIDA_50MHZ_Msk; //Configura el pin 9 como un pin de salida rapida
    GPIOA->CRH = (GPIOA->CRH & ~(0xF << GPIO_CRH_MODE10_Pos)) | PA10_ENTRADA_FLOTANTE_Msk;
    USART_reset();                                               //Reseteo el USART (Por defecto 8N1)
    USART1->BRR = (SystemCoreClock + baudrate/2) / baudrate;                //Establece el valor del baudrate en el puerto comunicador serie 
    USART1->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;   //Habilito el USART1 (CR1->UE = 1), el transmisor (CR1->TE = 1) y el receptor (CR1->RE = 1)                                       
}

bool USART_read (uint32_t *palabra) {
    bool lectura_exitosa=0;
    if(USART1->SR & USART_SR_RXNE_Msk) {                        //Cuando llega un caracter lo coloca en el DR, y se pone en 1 el RXNE
        *palabra = USART1->DR;                                  //La palabra leida toma el valor de DR
        lectura_exitosa=1;                                              
    }
    return lectura_exitosa;                                           
}

bool USART_write_byte (uint8_t c) {
    bool const buffer_disponible = (USART1->SR & USART_SR_TXE);  //1 buffer disponible, 0 buffer lleno
    bool escritura_exitosa = 0;
    if (buffer_disponible) {
        USART1->DR = c;                                                         //Escribo un caracter
        escritura_exitosa = 1;
    }
    return escritura_exitosa;                                                             
}

void USART_deInit () {
    while(~(USART1->SR & (USART_SR_TC_Msk)));                  //Espero a que TC = 1 en SR para asegurarse que finalizo la transmision
    USART_reset();                                             //Reinicio el USART
    RCC->APB2ENR &= ~(RCC_APB2ENR_USART1EN);                   //Desconecto el clock del USART
}


