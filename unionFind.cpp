#include "./unionFind.h"

UnionFind::UnionFind(int k) : uf_size(k) {
	parents = new int[uf_size];
	try{
        size = new int[uf_size];
		try{
			group_tree = new std::shared_ptr<Tree>[uf_size];
			for (int i = 0; i < uf_size; i++){
				parents[i] = -1;
				size[i] = 1;
				try{
					group_tree[i].reset(new Tree()); 
				}
				catch(const std::bad_alloc& ba){
					for (int j = 0; j < i; j++){
						group_tree[j] = nullptr;
					delete[] group_tree;
					throw ba;
					}
				}
			}
		}
		catch(const std::bad_alloc& ba){
			delete[] size;
			throw ba;
		}
    }
    catch(const std::bad_alloc& ba){
        delete[] parents;
    }
}

UnionFind::~UnionFind() {
	delete[] parents;
	delete[] size;
    for (int i = 0; i < uf_size; i++)
		if(group_tree[i])
        	group_tree[i]->destroy();
    delete[] group_tree;
}

void UnionFind::unionSets(int a, int b) {
    int large, small;
	int root_a = find(a);
	int root_b = find(b);
	if(root_a == root_b)
		return;
	large = (size[root_a-1] > size[root_b-1]) ? root_a : root_b;
	small = (root_a == large) ? root_b : root_a;
	group_tree[large-1]->merge(*(group_tree[small-1]));
	parents[small-1] = large-1;
	size[large-1] += size[small-1];
    return;
}

int UnionFind::find(int index) {
	int curr = index-1;
	while (parents[curr] != -1)
		curr = parents[curr];
	int root = curr;
	curr = index-1;
	// shorten routes
	while (parents[curr] != -1) {
		int next = parents[curr];
		parents[curr] = root;
		curr = next;
	}
	return root+1;
}

std::shared_ptr<Tree> UnionFind::tree(int index){
	return group_tree[find(index)-1];
}