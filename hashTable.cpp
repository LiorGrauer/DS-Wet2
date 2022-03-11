#include "./hashTable.h"

HashTable::HashTable() : tableSize(TABLE_SIZE), curr_size(0) {
	hash_table = new HashNode*[tableSize];
	for (int i = 0; i < tableSize; i++) {
		hash_table[i] = nullptr;
	}
}

HashTable::~HashTable() {
	HashNode* curr_next;
	for (int i = 0; i < tableSize; i++) {
		while(hash_table[i]) {
			curr_next = hash_table[i]->next;
			delete hash_table[i]->data;
			delete hash_table[i];
			hash_table[i] = curr_next;
		}
	}
	delete[] hash_table;
}

int HashTable::hash(int id) {
	return (id % tableSize);
}

void HashTable::insert(Player* data) {
	if(data == nullptr)
		return;
	if(curr_size==tableSize)
		extendHashTable();
	int key = hash(data->getID());
	HashNode* exist = hash_table[key];
	hash_table[key] = new HashNode(data);
	hash_table[key]->next = exist;
	curr_size++;
}

void HashTable::remove(int id) {
	if(find(id) == nullptr)
		return;
	int key = hash(id);
	HashNode* index = hash_table[key];
	if(hash_table[key]->data->getID() == id){
		hash_table[key] = hash_table[key]->next;
		delete index->data;
		delete index;
	} else {
		while(index->next->data->getID() != id)
			index = index->next;
		HashNode* temp = index->next;
		index->next = index->next->next;
		delete temp->data;
		delete temp;
	}
	curr_size--;
	if(curr_size == tableSize/4 && tableSize > TABLE_SIZE)
		shrinkHashTable();
}

Player* HashTable::find(int id){
	HashNode* index = hash_table[hash(id)];
	while(index && index->data->getID() != id)
		index = index->next;
	if (!index)
		return nullptr;
	return index->data;
}

void HashTable::extendHashTable() {
	int old_size = tableSize;
	HashNode *index, *iterator;
	tableSize *= 2;
	HashNode** new_hash_table = new HashNode*[tableSize];
	for (int i = 0; i < tableSize; i++) {
		new_hash_table[i] = nullptr;
	}
	for (int i = 0; i < old_size; i++) { 
		iterator = hash_table[i];
		while(iterator){
			try{
			index = new_hash_table[hash(iterator->data->getID())];
			new_hash_table[hash(iterator->data->getID())] = new HashNode(iterator->data);
			new_hash_table[hash(iterator->data->getID())]->next = index;
			iterator = iterator->next;
			}
			catch(const std::bad_alloc& ba){
				for (int j = 0; j < i ; j++) {
					while(new_hash_table[i]) {
						index = new_hash_table[i]->next;
						delete new_hash_table[i];
						new_hash_table[i] = index;
					}
				}
			delete[] new_hash_table;
			}
		}
	}
	for (int i = 0; i < old_size; i++) {
		while(hash_table[i]) {
			index = hash_table[i]->next;
			delete hash_table[i];
			hash_table[i] = index;
		}
	}
	delete[] hash_table;
	hash_table = new_hash_table;
}

void HashTable::shrinkHashTable() {
	int old_size = tableSize;
	HashNode* index, *iterator;
	tableSize /= 4;
	HashNode** new_hash_table = new HashNode*[tableSize];
	for (int i = 0; i < tableSize; i++) {
		new_hash_table[i] = nullptr;
	}
	for (int i = 0; i < old_size; i++) { 
		iterator = hash_table[i];
		while(iterator){
			try{
			index = new_hash_table[hash(iterator->data->getID())];
			new_hash_table[hash(iterator->data->getID())] = new HashNode(iterator->data);
			new_hash_table[hash(iterator->data->getID())]->next = index;
			iterator = iterator->next;
			}
			catch(const std::bad_alloc& ba){
				for (int j = 0; j < i ; j++) {
					while(new_hash_table[i]) {
						index = new_hash_table[i]->next;
						delete new_hash_table[i];
						new_hash_table[i] = index;
					}
				}
			delete[] new_hash_table;
			}
		}
	}
	for (int i = 0; i < old_size; i++) {
		while(hash_table[i]) {
			index = hash_table[i]->next;
			delete hash_table[i];
			hash_table[i] = index;
		}
	}
	delete[] hash_table;
	hash_table = new_hash_table;
}