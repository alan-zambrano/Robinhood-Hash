#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <vector>
#include <list>

/*
 *Need this format for Robin Hood Hashing. PSL is used to determine the hashed
 * object's place in the table.
 *Using the following guide: https://programming.guide/robin-hood-hashing.html
 */
template <typename T>
struct hashedObj{
	std::string key;
	T val;
	int PSL;
};

template <typename T>
class HashTable{
	private:
		std::vector<int> primesList;
		std::vector<hashedObj<T>> table;
		std::vector<int>::iterator currSizeIter;
		int numElems = 0;

		/*Hashing methods*/
		unsigned int keygen(std::string p);
		int rehash();
		int hashHelper(std::string key, T value);
	public:
		/*Table info*/
		const int MAX_TABLE_SIZE = 1000000;
		const float HASH_LOAD_FACTOR = .75;

		/*Error catching*/
		const int ERR_ITEM_NOT_FOUND = -3;
		const int ERR_MAX_TABLE_LOAD = -2;
		const int ERR_TABLE_MAX_SIZE = -1;
		const int SUCCESS_HASH = 0;

		/*Table creation*/
		HashTable();
		HashTable(size_t length);
		int hash(std::string key, T value);
		/*Getters*/
		int getNumElems();
		int length();

		/*User functions*/
		void printAll();
		void printRange(int i, int j);
		int find(std::string key, T value);
		int erase(std::string key, T value);
};

#endif
