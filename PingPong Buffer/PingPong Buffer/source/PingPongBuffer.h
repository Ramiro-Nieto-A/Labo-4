
#ifndef PINGPONGBUFFER_H_
#define PINGPONGBUFFER_H_

#include "hardware.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

//extern bool newBuffer;

void PingPongBuffer_Init(uint32_t freq,uint16_t bufferSize);
volatile uint16_t * GetBufferIn();
volatile uint16_t *  SetBufferOut();

#endif /* void PINGPONGBUFFER_H_ */
