#include "Header.h"
#include <iostream>
#include <cstring>
#include<Windows.h>
#include<string>
void printHex(const unsigned char* data, size_t length) {
	for (size_t i = 0; i < length; ++i) {
		std::cout << std::hex << (int)data[i] << " ";
	}
	std::cout << std::dec << std::endl; // Возврат в десятичный формат
}
void Anubis_XOR(uint8_t* a, uint8_t* b, uint8_t* c)
{
	for (int i = 0; i < 8; i++)
		c[i] = a[i] ^ b[i];
}

void HASH_Function(BYTE* mess, int size, NESSIEstruct* RaphaelTheBest)
{
	int length = 0;
	if (size % 16 != 0) length = size + (16 - size % 16);
	else length = size;
	std::cout << "Длина дополненного сообщения:" << std::endl;
	std::cout << length << std::endl;
	uint8_t* new_mess = new uint8_t[length];
	uint8_t* hash_mess = new uint8_t[length];
	for (int i = 0; i < size; i++)
	{
		new_mess[i] = mess[i];
		hash_mess[i] = mess[i];
	}
	//Заполняем нулевыми байтами последний блок
	if (length > size)
	{
		new_mess[size] = 0;
		hash_mess[size] = 32768;
		for (int i = size + 1; i < length; i++)
		{
			hash_mess[i] = 0;
			new_mess[i] = 0;
		}
	}
	int block_num = length / 16;
	uint8_t* enc_mess = new uint8_t[length];
	uint8_t* dec_mess = new uint8_t[length];
	uint8_t hash_value[16] = {};
	for (int i = 0; i < block_num; i++)
	{
		uint8_t blk[16];
		uint8_t blk_hash[16];
		for (int j = 0; j < 16; j++)
		{
			blk[j] = new_mess[j + 16 * i];
			blk_hash[j] = hash_mess[j + 16 * i];
		}
		if (i == 0) NESSIEencrypt(RaphaelTheBest, mess, hash_value);
		else
		{
			Anubis_XOR(hash_value, blk_hash, blk_hash);
			NESSIEdecrypt(RaphaelTheBest, blk_hash, hash_value);
		}
		uint8_t out_blk[16];
		NESSIEencrypt(RaphaelTheBest, blk, out_blk);
		for (int k = 0; k < 16; k++)
		{
			enc_mess[k + 16 * i] = out_blk[k];
		}
		NESSIEdecrypt(RaphaelTheBest, out_blk, out_blk);
		for (int k = 0; k < 16; k++)
		{
			dec_mess[k + 16 * i] = out_blk[k];
		}
	}
	std::cout << "Зашифрованное сообщение:" << std::endl;
	printf("%.*s\n", size, enc_mess);
	std::cout << "Расшифрованное сообщение:" << std::endl;
	printf("%s\n", dec_mess);
	std::cout << "Полученное хэш-значение:" << std::endl;
	for (int i = 0; i < 16; i++) printf("%02x", hash_value[i]);
	printf("\n");
}
int main()
{
	NESSIEstruct      NessieStruct;

	memset(&NessieStruct, 0, sizeof(NESSIEstruct));

	NessieStruct.keyBits = 16;

	NESSIEkeysetup((BYTE*)"ABRACADABRA12345", &NessieStruct);

	BYTE     pPlainText[MAX_PATH];
	BYTE     pCipherText[MAX_PATH];

	memset(pPlainText, 0, MAX_PATH);
	memset(pCipherText, 0, MAX_PATH);
	memcpy((char*)pPlainText, "Hello! My name is Yusufi Saddam!, and now you're watching the way my code works. It works great!", 97);
	std::cout << "Сообщение:" << std::endl;
	printf("%s\n", pPlainText);
	HASH_Function(pPlainText, 97, &NessieStruct);
	memset(pPlainText, 0, MAX_PATH);
	memset(pCipherText, 0, MAX_PATH);
	return 0;
}