
#include<iostream>
using namespace std;


class String
{
private:

	int length;
	int capacity;
	char* Characters;

public:

	String() {
		length = 0;
		capacity = 1;
		Characters = new char[capacity];
		Characters[0] = '\0';
	}
	String(const String& s) {
		length = s.length;
		capacity = s.capacity;
		Characters = new char[capacity];
		for (int i = 0; i < length; i++) {
			Characters[i] = s.Characters[i];
		}
		Characters[length] = '\0';
	}
	String& operator=(const String& s) {
		if (this != &s) {
			delete[] Characters;

			length = s.length;
			capacity = s.capacity;
			Characters = new char[capacity];

			for (int i = 0; i < length; i++) {
				Characters[i] = s.Characters[i];
			}
			Characters[length] = '\0';
		}
		return *this;
	}


	String(const char* c)
	{
		length = 0;
		while (c[length] != '\0') length++;
		capacity = length + 1;
		Characters = new char[capacity];
		for (int i = 0; i < length; i++) {
			Characters[i] = c[i];
		}
		Characters[length] = '\0';
	}

	void replace_first(char c)
	{
		if (length > 0) Characters[0] = c;
	}



	void print() const
	{
		for (int i = 0; i < length; i++) {
			cout << Characters[i];
		}
		cout << endl;
	}
	char* regrow_string()
	{
		capacity = (capacity * 2) + 1;
		char* temp = new char[capacity];

		for (int i = 0; i < length; i++)
		{
			temp[i] = Characters[i];
		}
		temp[length] = '\0';

		delete[] Characters;
		return temp;
	}
	int stoi() const {
		int num = 0;
		int sign = 1;
		int i = 0;

		if (Characters[0] == '-') {
			sign = -1;
			i++;
		}

		for (; i < length; i++) {
			if (Characters[i] < '0' || Characters[i] > '9') break;
			num = num * 10 + (Characters[i] - '0');
		}

		return num * sign;
	}
	String itos(int num) {
		if (num == 0) return String("0");

		int temp = num, count = 0;
		bool isNegative = false;

		if (num < 0) {
			isNegative = true;
			num = -num;
		}

		while (temp != 0) {
			count++;
			temp /= 10;
		}

		if (isNegative) count++;

		capacity = count + 1;
		Characters = new char[capacity];
		Characters[count] = '\0';

		for (int i = count - 1; i >= 0; i--) {
			Characters[i] = (num % 10) + '0';
			num /= 10;
		}

		if (isNegative) Characters[0] = '-';

		length = count;
		return *this;
	}
	String& trim() {
		int start = 0;
		while (Characters[start] == ' ' || Characters[start] == '\t' || Characters[start] == '\n') {
			start++;
		}

		int end = length - 1;
		while (end > start && (Characters[end] == ' ' || Characters[end] == '\t' || Characters[end] == '\n')) {
			end--;
		}

		int newLength = end - start + 1;
		char* trimmed = new char[newLength + 1];

		for (int i = 0; i < newLength; i++) {
			trimmed[i] = Characters[start + i];
		}
		trimmed[newLength] = '\0';

		delete[] Characters;
		Characters = trimmed;
		length = newLength;
		capacity = newLength + 1;

		return *this;
	}
	static String trim(const char* str) {
		int start = 0;
		while (str[start] == ' ' || str[start] == '\t' || str[start] == '\n') {
			start++;
		}

		int end = 0;
		while (str[end] != '\0') {
			end++;
		}
		end--;

		while (end > start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n')) {
			end--;
		}

		int newLength = end - start + 1;
		char* trimmed = new char[newLength + 1];

		for (int i = 0; i < newLength; i++) {
			trimmed[i] = str[start + i];
		}
		trimmed[newLength] = '\0';

		String result(trimmed);
		delete[] trimmed;
		return result;
	}
	bool operator==(const String& M) const {
		if (length != M.length) return false;

		for (int i = 0; i < length; i++) {
			if (Characters[i] != M.Characters[i]) {
				return false;
			}
		}
		return true;
	}
	bool is_less(const String& M) {
		int minLength = (length < M.length) ? length : M.length;

		for (int i = 0; i < minLength; i++) {
			if (Characters[i] < M.Characters[i]) {
				return true;
			}
			else if (Characters[i] > M.Characters[i]) {
				return false;
			}
		}

		return length < M.length;
	}
	bool is_greater(const String& M) {
		int minLength = (length < M.length) ? length : M.length;

		for (int i = 0; i < minLength; i++) {
			if (Characters[i] > M.Characters[i]) {
				return true;
			}
			else if (Characters[i] < M.Characters[i]) {
				return false;
			}
		}

		return length > M.length;
	}
	char operator[](int i) const {

		return Characters[i];
	}
	char& operator[](int i) {

		return Characters[i];
	}

	String concat(const String& s2) const {
		int newLength = this->length + s2.length;
		char* newChars = new char[newLength + 1];

		for (int i = 0; i < this->length; i++) {
			newChars[i] = this->Characters[i];
		}

		for (int i = 0; i < s2.length; i++) {
			newChars[this->length + i] = s2.Characters[i];
		}

		newChars[newLength] = '\0';

		String result(newChars);
		delete[] newChars;

		return result;
	}
	String& append(const String& s2) {
		int newLength = this->length + s2.length;

		if (newLength >= this->capacity) {
			this->Characters = regrow_string();
		}

		for (int i = 0; i < s2.length; i++) {
			this->Characters[this->length + i] = s2.Characters[i];
		}

		this->length = newLength;
		this->Characters[this->length] = '\0';

		return *this;
	}
	int* all_sub_strings(const char* substring, int& count) const {
		int* indices = new int[this->length]; // Allocate memory for storing indices
		count = 0; // Initialize count

		for (int i = 0; i <= this->length - strlen(substring); i++) {
			bool found = true;

			for (int j = 0; j < strlen(substring); j++) {
				if (this->Characters[i + j] != substring[j]) {
					found = false;
					break;
				}
			}

			if (found) {
				indices[count++] = i; // Store index and increment count
			}
		}

		return indices; // Return array of indices
	}

	int find_first(char ch) const {
		for (int i = 0; i < this->length; i++) {
			if (this->Characters[i] == ch) {
				return i;
			}
		}
		return -1; // Not found
	}

	int find_last(char ch) const {
		for (int i = this->length - 1; i >= 0; i--) {
			if (this->Characters[i] == ch) {
				return i;
			}
		}
		return -1; // Not found
	}

	int* find_all(char ch, int& C) const {
		int* indices = new int[this->length];
		C = 0;

		for (int i = 0; i < this->length; i++) {
			if (this->Characters[i] == ch) {
				indices[C++] = i;
			}
		}

		return indices;
	}

	int find_first(const String& str) const {
		int len = str.length;
		for (int i = 0; i <= this->length - len; i++) {
			bool match = true;
			for (int j = 0; j < len; j++) {
				if (this->Characters[i + j] != str.Characters[j]) {
					match = false;
					break;
				}
			}
			if (match) return i; // Found at index i
		}
		return -1; // Not found
	}


	int find_last(const String& str) const {
		int len = str.length;
		for (int i = this->length - len; i >= 0; i--) {
			bool match = true;
			for (int j = 0; j < len; j++) {
				if (this->Characters[i + j] != str.Characters[j]) {
					match = false;
					break;
				}
			}
			if (match) return i; // Found at index i
		}
		return -1; // Not found
	}


	int* find_all(const String& str, int& C) const {
		int* indices = new int[this->length]; // Allocate memory
		C = 0;
		int len = str.length;

		for (int i = 0; i <= this->length - len; i++) {
			bool match = true;
			for (int j = 0; j < len; j++) {
				if (this->Characters[i + j] != str.Characters[j]) {
					match = false;
					break;
				}
			}
			if (match) {
				indices[C++] = i; // Store index
			}
		}
		return indices; // Return array of indices
	}
	void remove_at(int i) {
		if (i < 0 || i >= length) {
			cout << "Index out of bounds!" << endl;
			return;
		}

		for (int j = i; j < length - 1; j++) {
			Characters[j] = Characters[j + 1];  // Shift elements left
		}

		Characters[length - 1] = '\0';  // Null-terminate
		length--;  // Reduce length
	}
	void insert_at(int i, char ch) {
		if (i < 0 || i > length) {
			cout << "Index out of bounds!" << endl;
			return;
		}

		for (int j = length; j > i; j--) {
			Characters[j] = Characters[j - 1];
		}

		Characters[i] = ch;
		length++;
	}
	void insert_at(int i, const String sub) {
		if (i < 0 || i > length) {
			cout << "Index out of bounds!" << endl;
			return;
		}

		int new_length = length + sub.length;

		for (int j = new_length - 1; j >= i + sub.length; j--) {
			Characters[j] = Characters[j - sub.length];
		}

		for (int j = 0; j < sub.length; j++) {
			Characters[i + j] = sub.Characters[j];
		}

		length = new_length;
	}

	void remove_first(char ch) {
		for (int i = 0; i < length; i++) {
			if (Characters[i] == ch) {
				// Shift elements left
				for (int j = i; j < length - 1; j++) {
					Characters[j] = Characters[j + 1];
				}
				length--;  // Reduce length
				return;  // Remove only first occurrence
			}
		}
	}
	void remove_last(char ch) {
		for (int i = length - 1; i >= 0; i--) {
			if (Characters[i] == ch) {
				// Shift elements left
				for (int j = i; j < length - 1; j++) {
					Characters[j] = Characters[j + 1];
				}
				length--;  // Reduce length
				return;  // Remove only last occurrence
			}
		}
	}
	void remove_all(char ch) {
		int shift = 0;  // Number of times character found

		for (int i = 0; i < length; i++) {
			if (Characters[i] == ch) {
				shift++;  // Count occurrences
			}
			else {
				Characters[i - shift] = Characters[i];  // Shift left
			}
		}

		length -= shift;  // Reduce length
	}
	void clear() {
		delete[] Characters;
		Characters = nullptr;
		length = 0;
	}
	void ToUpper() {
		for (int i = 0; i < length; i++) {
			if (Characters[i] >= 'a' && Characters[i] <= 'z') {
				Characters[i] -= 32;
			}
		}
	}
	void ToLower() {
		for (int i = 0; i < length; i++) {
			if (Characters[i] >= 'A' && Characters[i] <= 'Z') {
				Characters[i] += 32;
			}
		}
	}
	String ToUpper() const {
		String newStr(*this);
		for (int i = 0; i < newStr.length; i++) {
			if (newStr.Characters[i] >= 'a' && newStr.Characters[i] <= 'z') {
				newStr.Characters[i] -= 32;
			}
		}
		return newStr;
	}

	String ToLower() const {
		String newStr(*this);
		for (int i = 0; i < newStr.length; i++) {
			if (newStr.Characters[i] >= 'A' && newStr.Characters[i] <= 'Z') {
				newStr.Characters[i] += 32;
			}
		}
		return newStr;
	}

	const char* C_str() const {
		return Characters;
	}


	~String()
	{
		delete[] this->Characters;
	}
};
