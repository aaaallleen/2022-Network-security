/*This is the header for the AES functions, sub_byte, shift_rows, mix_columns, and others*/
#ifndef ASSG2_AES_H
#define ASSG2_AES_H
#include <vector>
#include <stdint.h>
const uint8_t mx = 0x1b;
std::vector<uint8_t> Sub_Byte(std::vector<uint8_t> , const uint8_t);
std::vector<uint8_t> Shift_Rows(std::vector<uint8_t>);
std::vector<uint8_t> Mix_Columns(std::vector<uint8_t>, const uint8_t);
std::vector<std::vector<uint8_t>> Gen_Roundkey(std::vector<uint8_t> , const uint8_t );
std::vector<uint8_t> Add_Roundkeys(std::vector<uint8_t>, std::vector<uint8_t>);
void AES_Encrypt(std::string plaintext, std::string key);
#endif