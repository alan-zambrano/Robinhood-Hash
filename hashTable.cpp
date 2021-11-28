#include <iostream>
#include <fstream>
#include <time.h> //time
#include <string>
#include "hashTable.h"

/*
 *Hash table defaults to the smallest size in @primeslist
 *@primesList- Used to expand the size of the table. Sequentially calculating
 * prime numbers is cumbersome, slow and unnecessary. We only need the
 * following numbers because we're always doubling the size of the table.
 * taken from https://www.mathsisfun.com/numbers/prime-number-lists.html
 */
template <typename T>
HashTable<T>::HashTable(){
	primesList = {
		67,     127,    257,
		503,    1009,   2003,
		4001,   8009,   16001,
		32003,  64007,  128021,
		256019, 512009, 999983};
	currSizeIter = primesList.begin();
	table.resize(*(currSizeIter), (hashedObj<T>){"", T(), -1});
}

/*
 *Initialize the hash table to at least the size @length. The actual length is
 * the next closest prime number from @primeslist. The largest the length will
 * actually be is very close to 2*@length.
 */
template <typename T>
HashTable<T>::HashTable(size_t length){
	primesList = {
		67,     127,    257,
		503,    1009,   2003,
		4001,   8009,   16001,
		32003,  64007,  128021,
		256019, 512009, 999983};
	currSizeIter = primesList.begin();
	for(;currSizeIter != primesList.end() &&
		(*currSizeIter) < length;
		currSizeIter++){}
	std::cout << "initialized to size " << (*currSizeIter) << std::endl;
	table.resize(*(currSizeIter), (hashedObj<T>){"", T(), -1});
}


/*
 *HASH FUNCTION
 *One-at-a-Time hash function taken from
 * https://eternallyconfuzzled.com/hashing-c-introduction-to-hashing
 *
 *Returns:
 * generated key.
 */
template <typename T>
unsigned int HashTable<T>::keygen(std::string p){
    unsigned int h = 0;
    int i;
    int len = p.length();

    for (i = 0; i < len; i++){
        h += p[i];
        h += (h << 10);
        h ^= (h >> 6);
    }

    h += (h << 3);
    h ^= (h >> 11);
    h += (h << 15);

    return h%table.size();
}

/*
 *Creates an empty vector @tablecpy at least twice the size of the current
 * @table-- the next size is determined from the subsequent prime number in the
 * vector @primeslist. We then rehash every element from the smaller @table.
 *
 *Returns:
 * ERR_TABLE_MAX_SIZE- hash table has exceeded size limits. Table is unchanged
 * SUCCESS_HASH- New table has been created and all elements rehashed
 */
template <typename T>
int HashTable<T>::rehash(){
	//moves the iterator over to get the next prime == 2 * current table size
	if(currSizeIter == primesList.end() ||
		*currSizeIter > MAX_TABLE_SIZE ||
		++currSizeIter == primesList.end()){

		std::cerr << "Failed to resize: table size exceeded" << std::endl;
		return ERR_TABLE_MAX_SIZE;
	}
	std::cout << "resizing to " << *currSizeIter << "..." << std::endl;
	std::vector<hashedObj<T>> tablecpy(*currSizeIter, (hashedObj<T>){"", T(), -1});
	//swap here so we can use hashHelper because it uses @table not @tablecpy
	swap(table, tablecpy);
	numElems = 0;
	for(auto it : tablecpy){
		if(hashHelper(it.key, it.val) != SUCCESS_HASH){
			std::cerr << "Something went horribly wrong" << std::endl;
			exit(0);
		};
	}
	return SUCCESS_HASH;
}

/*
 *Hashes @key @value pair to @table. Keep separate from hash() because this
 * function can have infinite recursion with rehash()
 *
 *Collisions
 * using Robin Hood Hashing from
 * https://programming.guide/robin-hood-hashing.html
 *
 *Returns:
 * ERR_MAX_TABLE_LOAD- when load factor > 75%
 * SUCCESS_HASH- when value has been added to hash table
 */
template <typename T>
int HashTable<T>::hashHelper(std::string key, T value){
	if(numElems/HASH_LOAD_FACTOR > table.size()){
		return ERR_MAX_TABLE_LOAD;
	}
	unsigned int hashInd = keygen(key);
	//if hashInd is out of bounds (this should never happen)
	if(hashInd > table.size() || hashInd < 0){
		std::cout << "hashInd: " << hashInd << " is out of bounds" << std::endl;
		exit(0);
	}
	hashedObj<T> elem = {key, value, 0};
	//for empty bucket, hash object normally
	if(table[hashInd].PSL == -1){
		table[hashInd] = elem;
	}
	//collision handler
	else{
		int iter = 0;
		while(iter < table.size() && table[hashInd].PSL != -1){
			if(elem.PSL > table[hashInd].PSL){
				hashedObj<T> tmp = table[hashInd];
				table[hashInd] = elem;
				elem = tmp;
			}
			hashInd++;
			hashInd %= table.size();
			elem.PSL++;
			iter++;
		}
		if(iter >= table.size()){
			std::cerr << "Error: no available buckets" << std::endl;
			exit(0);
		}
		table[hashInd] = elem;
	}
	numElems++;
	return SUCCESS_HASH;
}

/*
 *@key: used to find hashed location in the table
 *@value: value to be stored in the table
 *
 *Used to hash @key and @value to @table. If the table has exceeded the load
 * factor @HASH_LOAD_FACTOR, we must resize the table to prevent excessive
 * collisions. However, we must also prevent the hash table from expanding beyond
 * @MAX_TABLE_SIZE buckets because of memory and integer limits.
 *
 *Returns:
 * ERR_TABLE_MAX_SIZE- (see rehash)
 * SUCCESS_HASH- element has hashed successfully
 */
template <typename T>
int HashTable<T>::hash(std::string key, T value){
	int hashRet = hashHelper(key, value);
	//resize table if exceeding load factor, return error if not possible
	if(hashRet == ERR_MAX_TABLE_LOAD){
		rehash();
		hashRet = hashHelper(key, value);
		//if we fail again, then the table has not resized. Give up.
		if(hashRet != SUCCESS_HASH){
			std::cerr << "failed to hash " << value << std::endl;
		}
	}
	return hashRet;
}

template <typename T>
void HashTable<T>::printAll(){
	for(int i = 0; i < table.size(); i++){
		std::cout << "[" << i << "] " << table[i].key << ":" << table[i].val <<
			" " << table[i].PSL << std::endl;
	}
}

template <typename T>
void HashTable<T>::printRange(int i, int j){
	for(; i <= j; i++){
		std::cout << "[" << i << "] " << table[i].key << ":" << table[i].val <<
			" " << table[i].PSL << std::endl;
	}
}
/*Number of filled buckets in the table*/
template <typename T>
int HashTable<T>::getNumElems(){
	return numElems;
}

/*Total buckets in the table*/
template <typename T>
int HashTable<T>::length(){
	return table.size();
}

/*
 *Searches the hash table starting at the location in the table generated by
 * its key, then probes linearly until we either find the correct value, or
 * until the PSL of the current value is smaller than what makes sense if the
 * object were hashed there.
 *
 *Returns:
 * int [0-table.size]- index of the item if found
 * ERR_ITEM_NOT_FOUND- item is not in the table
 */
template <typename T>
int HashTable<T>::find(std::string key, T value){
	int hashInd = keygen(key);
	int expectedPSL = 0;
	for(; table[hashInd].PSL >= expectedPSL; expectedPSL++, hashInd++){
		hashInd %= table.size();
		if(table[hashInd].key == key && table[hashInd].val == value){
			return hashInd;
		}
	}
	return ERR_ITEM_NOT_FOUND;
}

/*
 *Uses the same algorithm from find() to find the item @key, @value, and
 * then removes it from the table. We then shift over all subsequent elements
 * until we find an empty bucket or an element with a PSL of 0
 *
 *Returns:
 * ERR_ITEM_NOT_FOUND- @key, @value is not in the hash table. Table unchanged
 * SUCCESS_HASH- item has been removed from the hash table
 */
template <typename T>
int HashTable<T>::erase(std::string key, T value){
	int hashInd = keygen(key);
	int expectedPSL = 0;
	for(; table[hashInd].PSL >= expectedPSL; expectedPSL++, hashInd++){
		hashInd %= table.size();
		if(table[hashInd].key == key && table[hashInd].val == value){
			//if we found the value, shift next values up by 1 until we find an
			//empty slot or one with a PSL of 0
			while(table[(hashInd+1)%table.size()].PSL != 0 && table[(hashInd+1)%table.size()].PSL != -1){
				table[hashInd] = table[(hashInd+1)%table.size()];
				table[hashInd].PSL--;
				hashInd = (hashInd+1)%table.size();
			}
			table[hashInd] = {"", "", -1};
			numElems--;
			return SUCCESS_HASH;
		}
	}
	return ERR_ITEM_NOT_FOUND;
}