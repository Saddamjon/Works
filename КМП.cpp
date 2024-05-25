#include <iostream>
#include<string>
#include<vector>
std::vector<int> computePrefixFunction(const std::string& pattern) {
	int m = pattern.length(); /// Длина шаблона
	std::vector<int> pi(m);   /// Массив для хранения префикс-функции
	int k = 0;                /// Длина текущего совпадающего префикса и суффикса

	pi[0] = 0; /// Префикс-функция для первого символа всегда равна 0

	for (int q = 1; q < m; q++) { /// Цикл по всем символам шаблона, начиная со второго
		while (k > 0 && pattern[k] != pattern[q]) {
			k = pi[k - 1]; /// Если символы не совпадают, сдвигаем шаблон назад
		}
		if (pattern[k] == pattern[q]) {
			k++; /// Если символы совпадают, увеличиваем длину совпадающего префикса и суффикса
		}
		pi[q] = k; /// Сохраняем значение префикс-функции для текущей позиции
	}
	return pi; /// Возвращаем массив префикс-функции
}
void KMP(const std::string& text, const std::string& pattern) {
	int n = text.length();    /// Длина текста
	int m = pattern.length(); /// Длина шаблона
	std::vector<int> pi = computePrefixFunction(pattern); /// Вычисляем префикс-функцию для шаблона
	int q = 0;                /// Длина текущего совпадения в шаблоне

	for (int i = 0; i < n; i++) { /// Проходим по всем символам текста
		while (q > 0 && pattern[q] != text[i]) {
			q = pi[q - 1]; /// Если символы не совпадают, сдвигаем шаблон назад
		}
		if (pattern[q] == text[i]) {
			q++; /// Если символы совпадают, увеличиваем длину совпадения
		}
		if (q == m) { /// Если вся подстрока совпала
			std::cout << "Pattern found at index " << i - m + 1 << std::endl;
			q = pi[q - 1]; /// Сдвигаем шаблон для поиска следующего возможного совпадения
		}
	}
}
int main() {
	std::string text = "AABAACAADAABAABA";
	std::string pattern = "AABA";
	KMP(text, pattern);
	return 0;
}
