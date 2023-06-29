#ifndef COMUNICADOR_SERIE_H
#define COMUNIADOR_SERIE_H
#include <stdbool.h>
#include <stdint.h>
/**
 * @brief Inicializa el puerto USART
 * 
 */
void USART_init (unsigned long const baudrate);

/**
 * @brief Lee el valor del buffer de entrada del USART
 * 
 * @param palabra Palabra leida
 * @return true: Lectura exitosa
 * @return false: Error de lectura
 */
bool USART_read (uint32_t *palabra);

/**
 * @brief Escribe un caracter en el buffer de salida del puerto USART
 * 
 * @param c Caracter a escribir (1 byte = 8 bits)
 * @return true: Escritura exitosa
 * @return false: Error de escritura 
 */
bool USART_write_byte (uint8_t c);

/**
 * @brief Escribe un caracter por pantalla y se asegura de que haya sido transmitido correctamente
 * 
 * @param c Caracter a escribir
 */

void esperarYTransmitir(char c);

/**
 * @brief Desconfigura el puerto USART
 * 
 */
void USART_deInit ();



#endif