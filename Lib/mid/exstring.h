#ifndef EXSTRING_H_
#define EXSTRING_H_

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include "mem.h"

#ifndef EXSTRING_INCLUDE_ARRAY
  #define EXSTRING_INCLUDE_ARRAY 1
#endif

#ifndef EXSTRING_INCLUDE_CUT
  #define EXSTRING_INCLUDE_CUT 0
#endif

#ifndef EXSTRING_INCLUDE_TRIM
  #define EXSTRING_INCLUDE_TRIM 1
#endif

#ifndef EXSTRING_INCLUDE_REPLACE_STR
  #define EXSTRING_INCLUDE_REPLACE_STR 0
#endif

//-------------------------------------------------------------------------------------------------

typedef enum {
  EXSTRING_Cut_StartLeft_GetLeft,
  EXSTRING_Cut_StartRight_GetLeft,
  EXSTRING_Cut_StartLeft_GetRight,
  EXSTRING_Cut_StartRight_GetRight
} EXSTRING_Cut_e;

//-------------------------------------------------------------------------------------------------

uint32_t hash(char *str);
char *substr(char *string, int16_t start, int16_t length);
char *joinstr(unsigned int count, const char *str, ...);
char *strcopy(char *str);
char *strtolower_this(char *str);
char *strtolower(char *str);
char *strtoupper_this(char *str);
char *strtoupper(char *str);
uint8_t itoa_base(int32_t nbr, char *str, uint8_t base, bool sign, uint8_t fill_zero, uint8_t fill_space);
char *itoa_int(int32_t nbr, uint8_t base, bool sign, uint8_t fill_zero, uint8_t fill_spac);
char *itoa_dec(int32_t nbr);
char *itoa_udec(uint32_t nbr);
char *itoa_hex8(uint32_t nbr);
char *itoa_hex16(uint32_t nbr);
char *itoa_hex32(uint32_t nbr);
char *itoa_array(uint8_t *ary, uint16_t len);
double uatof(char *str);
uint32_t str2nbr(const char *str);
bool str2nbr_valid(const char *str);
char *reverse_this_string(char *str);
char *reverse_string(char *str_in);
int find(char *str, char *pattern);
int find_right(char *str, char *pattern);
#if(EXSTRING_INCLUDE_ARRAY)
  void reverse_this_array(uint8_t *array, uint16_t length);
  uint8_t *reverse_array(uint8_t *array, uint16_t length);
  int find_array(uint8_t *array, uint16_t length, char *pattern);
#endif
#if(EXSTRING_INCLUDE_CUT)
  char *cut_this(char *str, char *pattern, CUT_e option);
  char *cut(char *str_in, char *pattern, CUT_e option);
#endif
#if(EXSTRING_INCLUDE_TRIM)
  char *ltrim_list(char *str, char *list);
  char *ltrim(char *str);
  char *rtrim_list(char *str, char *list);
  char *rtrim(char *str);
  char *trim_list(char *str, char *list);
  char *trim(char *str);
#endif
uint16_t count_of_char(char *str, char val);
char *extraction(char *str, char delimiter, int position);
int explode(char ***arr_ptr, char *str, char delimiter);
char *replace_this_char(char *pattern, char replacement, char *str);
char *replace_char(char *pattern, char replacement, char *original);
#if(EXSTRING_INCLUDE_REPLACE_STR)
  char* replace_string(char *pattern, char *replacement, char *original);
#endif

//-------------------------------------------------------------------------------------------------

extern const char lowercase[];
extern const char uppercase[];

//-------------------------------------------------------------------------------------------------

#endif
