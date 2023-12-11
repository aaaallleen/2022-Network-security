/*Includes the handling for print and other IOs*/
#ifndef ASSG2_IO_H
#define ASSG2_IO_H
#include <vector>
#include <stdint.h>
void print(uint8_t);
void print(std::vector<uint8_t>);
std::vector<uint8_t> vectorize_input(std::string);
#endif