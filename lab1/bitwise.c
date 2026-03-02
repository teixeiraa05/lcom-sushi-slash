#include "bitwise.h"

#define TODO return 255

uint8_t clear(uint8_t msk, int pos) { 
  uint8_t bit = 1 << pos;
  msk &= ~bit;
  return msk;
}

uint8_t set(uint8_t msk, int pos) { 
  uint8_t bit = 1 << pos;
  msk |= bit;
  return msk; 
}

bool is_set(uint8_t msk, int pos) { 
  uint8_t bit = 1 << pos;
  uint8_t  f = msk & bit;
  return f;
}

uint8_t lsb(uint16_t wide_msk) { 
  return (uint8_t) wide_msk;
}

uint8_t msb(uint16_t wide_msk) { 
  return (uint8_t) (wide_msk >> 8); 
}

uint8_t mask(int pos, ...) { 
  TODO; 
}
