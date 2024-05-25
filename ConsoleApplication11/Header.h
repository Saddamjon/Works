#ifndef HEADER_H
#define HEADER_H

#include <limits.h>
#include <cstdint>

// Типы данных
typedef int8_t s8;
typedef uint8_t u8;

// Определение типов данных в зависимости от размера int
#if UINT_MAX >= 4294967295UL // Если int имеет размер 32 бита или более

typedef int16_t s16;
typedef int32_t s32;
typedef uint16_t u16;
typedef uint32_t u32;

#define ONE32 0xffffffffU // Маска для 32 битов
#else
// В противном случае, если int имеет размер менее 32 бит
typedef int16_t s16;
typedef int32_t s32;
typedef uint16_t u16;
typedef uint32_t u32;

#define ONE32 0xffffffffUL // Маска для 32 битов
#endif

#define ONE8 0xffU  // Маска для 8 битов
#define ONE16 0xffffU // Маска для 16 битов

// Макросы для получения байтов из 32-битных значений
#define T8(x) ((x) & ONE8)
#define T16(x) ((x) & ONE16)
#define T32(x) ((x) & ONE32)

// Макросы для преобразования массивов из 8 байтов в 32-битные значения и обратно
#define U8TO32_BIG(c) (((u32)T8(*(c)) << 24) | ((u32)T8(*((c) + 1)) << 16) | \
                       ((u32)T8(*((c) + 2)) << 8) | ((u32)T8(*((c) + 3))))

#define U8TO32_LITTLE(c) (((u32)T8(*(c))) | ((u32)T8(*((c) + 1)) << 8) | \
                          ((u32)T8(*((c) + 2)) << 16) | ((u32)T8(*((c) + 3)) << 24))

#define U32TO8_BIG(c, v) do { \
    u32 x = (v); \
    u8 *d = (c); \
    d[0] = T8(x >> 24); \
    d[1] = T8(x >> 16); \
    d[2] = T8(x >> 8); \
    d[3] = T8(x); \
} while (0)

#define U32TO8_LITTLE(c, v) do { \
    u32 x = (v); \
    u8 *d = (c); \
    d[0] = T8(x); \
    d[1] = T8(x >> 8); \
    d[2] = T8(x >> 16); \
    d[3] = T8(x >> 24); \
} while (0)

// Макрос для циклического сдвига влево на n битов
#define ROTL32(v, n) (T32((v) << (n)) | ((v) >> (32 - (n))))

// Ограничения и константы для шифра
#define MIN_N 4
#define MAX_N 10
#define MIN_ROUNDS (8 + MIN_N)
#define MAX_ROUNDS (8 + MAX_N)
#define MIN_KEYSIZEB (4 * MIN_N)
#define MAX_KEYSIZEB (4 * MAX_N)
#define BLOCKSIZE 128
#define BLOCKSIZEB (BLOCKSIZE / 8)
#define KEYSIZEB 16

// Структура для хранения состояния шифра
struct NESSIEstruct {
	int keyBits; // Размер ключа в битах
	int R; // Количество раундов
	u32 roundKeyEnc[MAX_ROUNDS + 1][4]; // Ключи для шифрования
	u32 roundKeyDec[MAX_ROUNDS + 1][4]; // Ключи для дешифрования
};

// Таблицы замены (S-boxes) и раундовые константы
extern const u32 T0[256];
extern const u32 T1[256];
extern const u32 T2[256];
extern const u32 T3[256];
extern const u32 T4[256];
extern const u32 T5[256];
extern const u32 rc[32]; // Пример для 32 раундовых констант

// Прототипы функций
void NESSIEkeysetup(const unsigned char * const key,
	NESSIEstruct * const structpointer);

void NESSIEencrypt(const NESSIEstruct * const structpointer,
	const unsigned char * const plaintext,
	unsigned char * const ciphertext);

void NESSIEdecrypt(const NESSIEstruct * const structpointer,
	const unsigned char * const ciphertext,
	unsigned char * const plaintext);

#endif