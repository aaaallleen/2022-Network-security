/*Includes the implementation of operations in GF256*/
#ifndef ASSG2_GF256_H
#define ASSG2_GF256_H
#include <stdint.h>
#include <vector>
inline uint8_t GF256_add(uint8_t a,uint8_t b) { return (a^b);}
inline uint8_t GF256_mult_x(uint8_t a, const uint8_t mx) {return (a>=0x80)?((a<<1)^0x1b):(a<<1);}
uint8_t GF256_ext_gcd(uint8_t, uint8_t, uint8_t *, uint8_t *, const uint8_t);
uint8_t GF256_mult(uint8_t, const uint8_t, const uint8_t);
uint8_t GF256_inv(uint8_t, const uint8_t);
uint8_t GF256_div(uint8_t, uint8_t, uint8_t &);


#endif