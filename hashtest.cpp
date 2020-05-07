#include <iostream>
#include <fstream>
#include <string>
#include <chrono> //std::chrono
#include "hashTable.h"
#include <unordered_map>

int main(){
	HashTable table(512009);
	std::fstream fword;
	std::string word;
	fword.open("dictionary.txt");

	/********************Robin Hood Hashing********************/
	std::cout << "Hashing using robin hood hashing" << std::endl;
	auto t1 = std::chrono::high_resolution_clock::now();
	while(std::getline(fword, word, '\n')){
		if(table.hash(word, word) != table.SUCCESS_HASH){
			std::cerr << "something went wrong hashing " << word << std::endl;
			std::cerr << "table capacity: " << table.getNumElems() << std::endl;
			std::cerr << "table size: " << table.length() << std::endl;
			exit(0);
		}
	}
	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
	std::cout << "successfully hashed " << table.getNumElems() << " words" << std::endl;
	std::cout << duration << " milliseconds elapsed" << std::endl;
	std::cout << std::endl;

	fword.clear();
	fword.seekg(0, fword.beg);
	std::cout << "deleting the first 100000 words from the hash table" << std::endl;
	t1 = std::chrono::high_resolution_clock::now();
	for(int i = 0; i < 100000; i++){
		std::getline(fword, word, '\n');
		if(table.erase(word, word) != table.SUCCESS_HASH){
			std::cerr << "something went wrong deleting " << word << std::endl;
			std::cerr << "table capacity: " << table.getNumElems() << std::endl;
			std::cerr << "table size: " << table.length() << std::endl;
			exit(0);
		}
	}
	std::cout << "successfully deleted 100000 objects" << std::endl;
	t2 = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
	std::cout << duration << " milliseconds elapsed" << std::endl;
	std::cout << std::endl;

	std::cout << "rehashing first 100000 objects" << std::endl;
	fword.clear();
	fword.seekg(0, fword.beg);
	t1 = std::chrono::high_resolution_clock::now();
	for(int i = 0; i < 100000; i++){
		std::getline(fword, word, '\n');
		if(table.hash(word, word) != table.SUCCESS_HASH){
			std::cerr << "something went wrong hashing " << word << std::endl;
			std::cerr << "table capacity: " << table.getNumElems() << std::endl;
			std::cerr << "table size: " << table.length() << std::endl;
			exit(0);
		}
	}
	std::cout << "successfully rehashed 100000 objects" << std::endl;
	t2 = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
	std::cout << duration << " milliseconds elapsed" << std::endl;
	std::cout << std::endl;

	/***********************Unordered Map***********************/
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "hashing using unordered map" << std::endl;
	fword.clear();
	fword.seekg(0, fword.beg);
	std::unordered_map<std::string, std::string> umap;
	t1 = std::chrono::high_resolution_clock::now();
	while(std::getline(fword, word, '\n')){
		umap.insert({word, word});
	}
	t2 = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
	std::cout << duration << " milliseconds elapsed" << std::endl;
	std::cout << std::endl;

	std::cout << "deleting first 100000 objects" << std::endl;
	fword.clear();
	fword.seekg(0, fword.beg);
	t1 = std::chrono::high_resolution_clock::now();
	for(int i = 0; i < 100000; i++){
		std::getline(fword, word, '\n');
		umap.erase(word);
	}
	t2 = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
	std::cout << duration << " milliseconds elapsed" << std::endl;
	std::cout << std::endl;

	std::cout << "rehashing first 100000 objects" << std::endl;
	fword.clear();
	fword.seekg(0, fword.beg);
	t1 = std::chrono::high_resolution_clock::now();
	for(int i = 0; i < 100000; i++){
		std::getline(fword, word, '\n');
		umap.insert({word, word});
	}
	t2 = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
	std::cout << duration << " milliseconds elapsed" << std::endl;
	std::cout << std::endl;
}
