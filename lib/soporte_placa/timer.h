#ifndef TIMER_H
#define TIMER_H
#include <stdint.h>
#include <stdbool.h>

enum flancos{
    FLANCO_ASCENDENTE,
    FLANCO_DESCENDENTE
}; 

/**
 * @brief Inicializ el timer
 * 
 */
void TIM4_init ();

/**
 * @brief Establece el canal 1 del timer 4 en modo capture
 * 
 * @param flanco Variable que indica si capturo por flanco ascendente o descendente
 * @return Valor capturado
 */
uint32_t TIM4_detectarFlanco (bool flanco);

/**
 * @brief Genera un pulso de la duracion especificada
 * 
 * @param tiempo Ciclos de timer que dura el pulso (1 ciclo = 14.7 microsegundos)
 */
void TIM4_generaPulso (uint32_t ciclos);

/**
 * @brief Desconfigura el timer
 * 
 */
void TIM4_deInit ();


#endif