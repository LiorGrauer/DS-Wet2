#ifndef UNIONFIND_H_
#define UNIONFIND_H_

#include "./tree.h"

class UnionFind {
private:
    int uf_size;
	int* parents; 
    int* size;
    std::shared_ptr<Tree>* group_tree;
    
public:
	UnionFind(int uf_size);
	~UnionFind();

    int find(int i);
	void unionSets(int i, int j);
    std::shared_ptr<Tree> tree(int index);
};

#endif /* UNIONFIND_H_ */