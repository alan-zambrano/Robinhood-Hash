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
struct hashedObj{
	std::string key;
	std::string val;
	int PSL;
};

class HashTable{
	private:
		std::vector<int> primesList;
		std::vector<hashedObj> table;
		std::vector<int>::iterator currSizeIter;
		int numElems = 0;

		/*hashing methods*/
		unsigned int keygen(std::string p);
		int rehash();
		int hashHelper(std::string key, std::string value);
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
		HashTable(unsigned int length);
		int hash(std::string key, std::string value);

		/*Getters*/
		int getNumElems();
		int length();

		/*User functions*/
		void printAll();
		void printRange(int i, int j);
		int find(std::string key, std::string value);
		int erase(std::string key, std::string value);
};

#endif
