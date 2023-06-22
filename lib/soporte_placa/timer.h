#ifndef TIMER_H
#define TIMER_H
#include <stdint.h>

/**
 * @brief Inicializ el timer
 * 
 */
void TIM_init ();

/**
 * @brief Establece el timer en modo compare
 * 
 * @param tiempo Valor de tiempo con el que se compara el contador
 */
void TIM2_setCompare (uint32_t tiempo);

/**
 * @brief Establece el timer en modo capture
 * 
 */
void TIM2_setCapture ();

/**
 * @brief Desconfigura el timer
 * 
 */
void TIM2_deInit ();


#endif