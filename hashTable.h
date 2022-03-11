#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include "./player.h"

const int TABLE_SIZE = 4;

class HashTable{
    private:
        struct HashNode {
            public:
                Player* data;
                HashNode* next;
				HashNode() = default;
                HashNode(Player* data) : data(data), next(nullptr) {};
        };
		HashNode** hash_table;
        int tableSize;
        int curr_size;

        void extendHashTable();
        void shrinkHashTable();
		
    public:
        HashTable();
        ~HashTable();
		int hash(int key);
        void insert(Player* data);
		void remove(int id);
        Player* find(int id);
};

#endif /* HASHTABLE_H_ */