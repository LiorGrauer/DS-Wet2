#include "./tree.h"


Tree::Tree(): size(0), index(0){
    zero.reset(new LevelNode());
}

int Tree::getSize() const{
    return this->size;
}

const std::shared_ptr<LevelNode> Tree::getHead(){
    return this->head;
}

//searches for node with given data starts recursively from the tree_node given
static std::shared_ptr<LevelNode> findNode(std::shared_ptr<LevelNode> tree_node, int level){
    if (!tree_node)
        return nullptr;
    if (*tree_node == level)
        return tree_node;
    else{
        if (*tree_node < level)
            return findNode(tree_node->getRight(), level);
        else
            return findNode(tree_node->getLeft(), level);
    }
}

//returns pointer to the data of the node in the tree with given data, 
//and nullptr if there are no node with this data in the tree
const std::shared_ptr<LevelNode> Tree::find(int level){
    std::shared_ptr<LevelNode> node = findNode(this->getHead(), level);
    if (!node)
        return nullptr;
    else
        return node;
}

static int calculateHeight (const std::shared_ptr<LevelNode> node){
    if ((node->getLeft() != nullptr) && (node->getRight() != nullptr)){ //not leaf
        if (node->getLeft()->getHeight() > node->getRight()->getHeight())
            return node->getLeft()->getHeight() + 1;
        else 
            return node->getRight()->getHeight() + 1;
    } else {
        if (node->getLeft() != nullptr)
            return node->getLeft()->getHeight() + 1;
        else
            if (node->getRight() != nullptr)
                return -node->getRight()->getHeight() + 1;
    }
    return 0;
}

void Tree::insert(std::shared_ptr<LevelNode> node){
    //std::shared_ptr<TreeNode<T>> node(new TreeNode<T>(data));
    if(node == nullptr)
        return;
    this->size++;
    if (this->head == nullptr) //this new node is the only one in the tree
        this->head = node;
    else{
        std::shared_ptr<LevelNode> iterator = this->head;
        while (1) { //searches for the right place for the new node in the tree
            if (*iterator < *node){
                if (iterator->getRight() != nullptr)
                    iterator = iterator->getRight();
                else{ //insert the node in the reight place as right son
                    iterator->changeRight(node);
                    node->changeFather(iterator);
                    iterator->changeHeight(calculateHeight(iterator));
                    break;
                    }
            }
            else{
                if (iterator->getLeft() != nullptr)
                    iterator = iterator->getLeft();
                else{ //insert the node in the reight place as left son
                    iterator->changeLeft(node);
                    node->changeFather(iterator);
                    iterator->changeHeight(calculateHeight(iterator));
                    break;
                }
            }
        }
        iterator = node;
        this->updateRanks(node, node);
        while (iterator != nullptr){ //fixes the AVL tree with roll if needed, checks all the way to the root
            iterator->changeHeight(calculateHeight(iterator));
            if (makeRoll(iterator)) //roll made so the tree is balanced now
                break;
            iterator = iterator->getFather();
       }
    }    
}

static void updateRank (std::shared_ptr<LevelNode> node){
    if (node->getLeft() != nullptr){
        node->changeSumLevels(node->getSumLevels() + node->getLeft()->getSumLevels());
        for (int i=1; i<=200; i++)
            node->changePlayers(node->getPlayers(i) + node->getLeft()->getPlayers(i), i);
    }
    if (node->getRight() != nullptr){
        node->changeSumLevels(node->getSumLevels() + node->getRight()->getSumLevels());
        for (int i=1; i<=200; i++)
            node->changePlayers(node->getPlayers(i) + node->getRight()->getPlayers(i), i);
    }
}

static void addRank (std::shared_ptr<LevelNode> node, std::shared_ptr<LevelNode> add){
    node->changeSumLevels(node->getSumLevels() + add->getSumLevels());
    for (int i=1; i<=200; i++)
        node->changePlayers(node->getPlayers(i) + add->getPlayers(i), i);
}

void Tree::updateRanks (std::shared_ptr<LevelNode> node, std::shared_ptr<LevelNode> add){
    std::shared_ptr<LevelNode> iterator = node->getFather();
    while (iterator != nullptr){
        addRank(iterator, add);
        iterator = iterator->getFather();
    }
}

static void deleteRank (std::shared_ptr<LevelNode> node, std::shared_ptr<LevelNode> remove){
    node->changeSumLevels(node->getSumLevels() - remove->getSumLevels());
    for (int i=1; i<=200; i++)
        node->changePlayers(node->getPlayers(i) - remove->getPlayers(i), i);
}

void Tree::updateRanksAfterRemove(std::shared_ptr<LevelNode> start, std::shared_ptr<LevelNode> stop, std::shared_ptr<LevelNode> remove){
    std::shared_ptr<LevelNode> iterator = start->getFather();
    while (iterator != nullptr && !(*iterator == *stop)){
        deleteRank(iterator, remove);
        iterator = iterator->getFather();
    }
}

static void removeSonsRank (std::shared_ptr<LevelNode> node){
    if (node->getLeft() != nullptr){
        node->changeSumLevels(node->getSumLevels() - node->getLeft()->getSumLevels());
        for (int i=1; i<=200; i++)
            node->changePlayers(node->getPlayers(i) - node->getLeft()->getPlayers(i), i);
    }
    if (node->getRight() != nullptr){
        node->changeSumLevels(node->getSumLevels() - node->getRight()->getSumLevels());
        for (int i=1; i<=200; i++)
            node->changePlayers(node->getPlayers(i) - node->getRight()->getPlayers(i), i);
    }
}

static int calculateBF (const std::shared_ptr<LevelNode> node){
    if ((node->getLeft() == nullptr) || (node->getRight() == nullptr)){ //only one son
        if (node->getLeft() != nullptr) //only left son
            return node->getLeft()->getHeight()+1;
        else {
            if (node->getRight() != nullptr) //only right son
                return -node->getRight()->getHeight()-1;
            else //leaf
                return 0;
        }      
    }
    return node->getLeft()->getHeight() - node->getRight()->getHeight();
}

//checks the BF and makes roll if needed. returns if rolled.
bool Tree::makeRoll (std::shared_ptr<LevelNode> node){
    int BF_father, BF_son;
    BF_father = calculateBF(node);
    if (BF_father < L_ROLL && BF_father > R_ROLL) //no roll needed
        return false;
    else{
        if (BF_father == L_ROLL){
            BF_son = calculateBF(node->getLeft());
            if (BF_son == LR)
                makeLR(node);
            else
                makeLL(node);
        }
        else{ //R_ROLL
            BF_son = calculateBF(node->getRight());
            if (BF_son == RL)
                makeRL(node);
            else
                makeRR(node);
        }
    } 
    return true;
}

//makes LL roll with all needed updates
void Tree::makeLL (std::shared_ptr<LevelNode> node){
    std::shared_ptr<LevelNode> new_node = node->getLeft();
    removeSonsRank(node);
    removeSonsRank(new_node);
    node->changeLeft(new_node->getRight());
    new_node->changeRight(node);
    if(node->getFather() == nullptr) //the roll is on the root
        this->head = new_node;
    else{
        if (node == node->getFather()->getLeft()) //the roll is on left son
            node->getFather()->changeLeft(new_node);
        else //the roll is on right son
            node->getFather()->changeRight(new_node);
    }
    new_node->changeFather(node->getFather());
    node->changeFather(new_node);
    if (node->getLeft() != nullptr)
        node->getLeft()->changeFather(node);
    node->changeHeight(calculateHeight(node));
    new_node->changeHeight(calculateHeight(new_node));
    updateRank(node);
    updateRank(new_node);
}

//makes LR roll with all needed updates
void Tree::makeLR (std::shared_ptr<LevelNode> node){
    std::shared_ptr<LevelNode> node1 = node->getLeft();
    std::shared_ptr<LevelNode> node2 = node1->getRight();
    removeSonsRank(node);
    removeSonsRank(node1);
    removeSonsRank(node2);
    node->changeLeft(node2->getRight());
    node1->changeRight(node2->getLeft());
    node2->changeRight(node);
    node2->changeLeft(node1);
    if(node->getFather() == nullptr) //the roll is on the root
        this->head = node2;
    else{
        if (node == node->getFather()->getLeft()) //the roll is on left son
            node->getFather()->changeLeft(node2);
        else //the roll is on right son
            node->getFather()->changeRight(node2);
    }
    node2->changeFather(node->getFather());
    node->changeFather(node2);
    node1->changeFather(node2);
    if (node1->getRight() != nullptr)
        node1->getRight()->changeFather(node1);
    if (node->getLeft() != nullptr)
        node->getLeft()->changeFather(node);
    node1->changeHeight(calculateHeight(node1));
    node->changeHeight(calculateHeight(node));
    node2->changeHeight(calculateHeight(node2));
    updateRank(node);
    updateRank(node1);
    updateRank(node2);
}

//makes RR roll with all needed updates
void Tree::makeRR (std::shared_ptr<LevelNode> node){
    std::shared_ptr<LevelNode> new_node = node->getRight();
    removeSonsRank(node);
    removeSonsRank(new_node);
    node->changeRight(new_node->getLeft());
    new_node->changeLeft(node);
    if(node->getFather() == nullptr) //the roll is on the root
        this->head = new_node;
    else{
        if (node == node->getFather()->getRight()) //the roll is on left son
            node->getFather()->changeRight(new_node);
        else //the roll is on right son
            node->getFather()->changeLeft(new_node);
    }
    new_node->changeFather(node->getFather());
    node->changeFather(new_node);
    if (node->getRight() != nullptr)
        node->getRight()->changeFather(node);
    node->changeHeight(calculateHeight(node));
    new_node->changeHeight(calculateHeight(new_node));
    updateRank(node);
    updateRank(new_node);
}

//makes RL roll with all needed updates
void Tree::makeRL (std::shared_ptr<LevelNode> node){
    std::shared_ptr<LevelNode> node1 = node->getRight();
    std::shared_ptr<LevelNode> node2 = node1->getLeft();
    removeSonsRank(node);
    removeSonsRank(node1);
    removeSonsRank(node2);
    node->changeRight(node2->getLeft());
    node1->changeLeft(node2->getRight());
    node2->changeLeft(node);
    node2->changeRight(node1);
    if(node->getFather() == nullptr) //the roll is on the root
        this->head = node2;
    else{
        if (node == node->getFather()->getLeft()) //the roll is on left son
            node->getFather()->changeLeft(node2);
        else //the roll is on right son
            node->getFather()->changeRight(node2);
    }
    node2->changeFather(node->getFather());
    node->changeFather(node2);
    node1->changeFather(node2);
    if (node1->getLeft() != nullptr)
        node1->getLeft()->changeFather(node1);
    if (node->getRight() != nullptr)
        node->getRight()->changeFather(node);
    node1->changeHeight(calculateHeight(node1));
    node->changeHeight(calculateHeight(node));
    node2->changeHeight(calculateHeight(node2));
    updateRank(node);
    updateRank(node1);
    updateRank(node2);
}

//swaps the data of 2 nodes
static void swapNodes(std::shared_ptr<LevelNode> ptr1, std::shared_ptr<LevelNode> ptr2){
    ptr1->swapData(ptr2);
}

void Tree::remove(int level){
    std::shared_ptr<LevelNode> node = findNode(this->head, level);
    std::shared_ptr<LevelNode> swaped = node;
    bool swap = false;
    if(node == nullptr)
        return;
    this->size--;
    removeSonsRank(node);
    if (node->getLeft() != nullptr && node->getRight() != nullptr){ //both left and right sons - make a switch with next node inorder
        std::shared_ptr<LevelNode> node_to_swap = node->getRight();
        while(node_to_swap->getLeft() != nullptr)
            node_to_swap = node_to_swap->getLeft();
        removeSonsRank(node_to_swap);
        swapNodes(node, node_to_swap);
        swaped = node;
        node = node_to_swap;
        swap = true;
    }
    //one or zero sons
    if (node->getLeft() != nullptr){ //only left son
        if (this->getHead() == node){ //the node the root and has only one left son
            this->head = node->getLeft();
            node->getLeft()->changeFather(nullptr);
            node->getLeft()->changeHeight(0);
            return;
        }
        else{
            if (node == node->getFather()->getLeft()) //the node is a left son of his father
                node->getFather()->changeLeft(node->getLeft());
            else //the node is a right son of his father
                node->getFather()->changeRight(node->getLeft());
            node->getLeft()->changeFather(node->getFather());
        }
    }
    else{ //no left son
        if (this->getHead() == node){ //the node the root
            if (node->getRight() == nullptr){ //the node is the only one in the tree
                this->head = nullptr;
                return;
            }
            else{ //only one right son
                this->head = node->getRight();
                node->getRight()->changeFather(nullptr);
                node->getRight()->changeHeight(0);
                return;
            }
        }
        else{ //the node is not the root
            if (node->getRight() != nullptr){ //only right son
                if (node == node->getFather()->getLeft()) //the node is a left son of his father
                    node->getFather()->changeLeft(node->getRight());
                else //the node is a right son of his father
                    node->getFather()->changeRight(node->getRight());
                node->getRight()->changeFather(node->getFather());
            }
            else{ //no sons
                if (node == node->getFather()->getLeft()) //the node is a left son of his father
                    node->getFather()->changeLeft(nullptr);
                else //the node is a right son of his father
                    node->getFather()->changeRight(nullptr);
            }
        }
    }
    /*this->updateRanksAfterRemove(swaped, this->head, node);
    deleteRank(this->head, node);*/
    if (swap){
        this->updateRanksAfterRemove(node, swaped, swaped);
        updateRank(swaped);
    }
    std::shared_ptr<LevelNode> iterator = node;
    while(iterator != nullptr){ //fixes the AVL tree with rolls if needed, checks all the way to the root
        iterator->changeHeight(calculateHeight(iterator));
        makeRoll(iterator);
        iterator = iterator->getFather();
    }
}

//puts the number given of nodes inorder into the array given
void Tree::inorder(std::shared_ptr<LevelNode>* destination, int num){
    this->index = 0;
    if (num == -1) //return all the nodes
        inorderTour(this->head, destination, this->size);
    else
        inorderTour(this->head, destination, num);
}

void Tree::inorderTour(std::shared_ptr<LevelNode> node, std::shared_ptr<LevelNode>* destination, int counter){
    if (node == nullptr || this->index == counter) //no more nodes on the tree / already puts the required number of nodes
        return;
    inorderTour(node->getLeft(), destination, counter); //recursively call on left son
    if(this->index < counter){ //needs to put more nodes in the array inorder
        destination[this->index] = node;
        this->index++;
    }
    inorderTour(node->getRight(), destination, counter); //recursively call on right son
}

//puts the number given of nodes reverse inorder into the array given
void Tree::reverseInorder(std::shared_ptr<LevelNode>* destination, int num){
    this->index = 0;
    if (num == -1)
        reverseInorderTour(this->head, destination, this->size);
    else
        reverseInorderTour(this->head, destination, num);
}

void Tree::reverseInorderTour(std::shared_ptr<LevelNode> node, std::shared_ptr<LevelNode>* destination, int counter){
    if (node == nullptr || this->index == counter) //no more nodes on the tree / already puts the required number of nodes
        return;
    reverseInorderTour(node->getRight(), destination, counter); //recursively call on right son
    if (this->index < counter){ //needs to put more nodes in the array reverse inorder
        destination[this->index] = node;
        this->index++;
    }
    reverseInorderTour(node->getLeft(), destination, counter); //recursively call on left son
}

//puts the number all the nodes inorder into the array given for merge function
void Tree::inorderNodes(std::shared_ptr<LevelNode>* destination){
    this->index = 0;
    inorderNodesTour(this->head, destination, this->size);
}

void Tree::inorderNodesTour(std::shared_ptr<LevelNode> node, std::shared_ptr<LevelNode>* destination, int counter){
    if (node == nullptr || this->index == counter) //no more nodes on the tree / already puts the required number of nodes
        return;
    inorderNodesTour(node->getLeft(), destination, counter); //recursively call on left son
    destination[this->index] = node;
    this->index++;
    inorderNodesTour(node->getRight(), destination, counter); //recursively call on right son
}

void Tree::changeRankPreOrder(std::shared_ptr<LevelNode> node){
    if (node == nullptr)
        return;
    removeSonsRank(node);
    changeRankPreOrder(node->getLeft());
    changeRankPreOrder(node->getRight());
}

void Tree::updateRankPostOrder(std::shared_ptr<LevelNode> node){
    if (node == nullptr)
        return;
    updateRankPostOrder(node->getLeft());
    updateRankPostOrder(node->getRight());
    updateRank(node);
}


static int mergeArray(std::shared_ptr<LevelNode>* arr1, int size1, std::shared_ptr<LevelNode>* arr2, int size2, std::shared_ptr<LevelNode>* destination){
    int i1 = 0, i2 = 0, id = 0, duplicates = 0;
    while (i1 < size1 && i2 < size2){
        if (*arr1[i1] == *arr2[i2]){
            destination[id] = arr1[i1];
            addRank(destination[id], arr2[i2]);
            id++;
            i1++;
            i2++;
            duplicates++;
        }
        else{
            if (*arr1[i1] < *arr2[i2]){
                destination[id] = arr1[i1];
                id++;
                i1++;
            }
            else{
                destination[id] = arr2[i2];
                id++;
                i2++;
            }
        }
    }
    while (i1 < size1){
        destination[id] = arr1[i1];
        id++;
        i1++;
    }
    while (i2 < size2){
        destination[id] = arr2[i2];
        id++;
        i2++;
    }
    return duplicates;
}

//disconnect all the branches of the tree
static void removeBranches(std::shared_ptr<LevelNode> node){
    if (node == nullptr)
        return;
    removeBranches(node->getLeft()); //recursively call on left son
    removeBranches(node->getRight()); //recursively call on right son
    node->removeConnections();
}

//merge given tree into this tree
void Tree::merge(Tree& other){
    this->index = 0;
    int removed = 0;
    std::shared_ptr<LevelNode>* arr1 = new std::shared_ptr<LevelNode> [this->size]; //array for this tree nodes sorted inorder
    try{
        std::shared_ptr<LevelNode>* arr2 = new std::shared_ptr<LevelNode> [other.getSize()]; //array for other tree nodes sorted inorder
        this->changeRankPreOrder(this->head);
        other.changeRankPreOrder(other.head);
        this->inorderNodes(arr1);
        other.inorderNodes(arr2);
        try{
            int new_size = this->size + other.getSize();
            std::shared_ptr<LevelNode>* arr3 = new std::shared_ptr<LevelNode> [new_size];
            removed = mergeArray(arr1, this->size, arr2, other.getSize(), arr3); //merge the sorted arrays into arr3
            new_size -= removed;
            std::shared_ptr<LevelNode> temp = this->head;
            try{
                this->head = nullptr;
                this->buildEmptyTree(new_size); //build new empty tree in this tree in the required size
                //removes all the branches from both trees for merging the two tree to one:
                removeBranches(temp); 
                removeBranches(other.head);
                this->size = new_size;
                other.size = 0;
                this->index = 0;
                this->addInorder(this->head, arr3); //insert all the nodes into the new tree in time complexity of o(n), while n is the total number of nodes
                this->updateRankPostOrder(this->head);
                addRank(this->zero, other.zero);
                other.zero = nullptr;
                delete[] arr1;
                delete[] arr2;
                delete[] arr3;
                other.head = nullptr;
            }
            catch(const std::bad_alloc& ba){
                this->head = temp;
                delete[] arr3;
                throw ba;
            }
        }
        catch(const std::bad_alloc& ba){
            other.updateRankPostOrder(other.head);
            delete[] arr2;
            throw ba;
        }
    }
    catch(const std::bad_alloc& ba){
        this->updateRankPostOrder(this->head);
        delete[] arr1;
        throw ba;
    }  
}

//build empty tree in given size
void Tree::buildEmptyTree(int size){
    this->index = 0;
    int full_tree_size = 1;
    int full_tree_height = 0;
    while (full_tree_size < size){ //calculate the minimal size and height of full tree that is bigger than this tree size
        full_tree_size = full_tree_size*2 + 1;
        full_tree_height++;
    }
    std::shared_ptr<LevelNode> node(new LevelNode);
    this->head = node;
    this->buildNodes(this->head, full_tree_height); //creates the empty full tree
    this->index = 0;
    this->removeLeaves(this->head, full_tree_size - size); //removes the leafs to make the tree in the required size
}

//build empty nodes for the tree with given height
void Tree::buildNodes(std::shared_ptr<LevelNode> father, int height){
    if (height == 0)
        return;
    std::shared_ptr<LevelNode> left(new LevelNode);
    father->changeLeft(left);
    left->changeFather(father);
    this->index++;
    std::shared_ptr<LevelNode> right(new LevelNode);
    father->changeRight(right);
    right->changeFather(father);
    this->buildNodes(left, height - 1); //recursively call on left son
    this->buildNodes(right, height - 1); //recursively call on right son
}

//removes given number of leaves from the tree
void Tree::removeLeaves(std::shared_ptr<LevelNode> node, int num){
    if (node == nullptr || this->index == num) //no more nodes / no more leafs to remove
        return;
    removeLeaves(node->getRight(), num); //recursively call on right son
    if (this->index < num && node->getLeft() == nullptr && node->getRight() == nullptr){ //need to remove more leaves and this node is a leaf
        if (this->head != node){ //this node is not the root
            if (node == node->getFather()->getLeft())
                node->getFather()->changeLeft(nullptr);
            else
                node->getFather()->changeRight(nullptr);
        }
        else
            this->head = nullptr;
        this->index++;
    }
    removeLeaves(node->getLeft(), num); //recursively call on left son
}

// insert the given nodes (in sorted array) to the tree inorder in time complexity of o(n) while n is the number of nodes
void Tree::addInorder(std::shared_ptr<LevelNode> node, std::shared_ptr<LevelNode>* nodes){
    if (node == nullptr)
        return;
    addInorder(node->getLeft(), nodes); //recursively call on left son
    swapNodes(node, nodes[this->index]);
    node->changeHeight(calculateHeight(node));
    this->index++;
    addInorder(node->getRight(), nodes); //recursively call on right son
}


static void destroyTree(std::shared_ptr<LevelNode> node){
    if (node == nullptr)
        return;
    destroyTree(node->getLeft()); //recursively call on left son
    destroyTree(node->getRight()); //recursively call on right son
    node->destroy();
}

void Tree::destroy(){
    destroyTree(this->head);
    this->head = nullptr;
    this->zero = nullptr;
}

void Tree::addPlayer(int level, int score){
    if (level == 0){
        zero->changePlayers(zero->getPlayers(score) + 1, score); 
    }
    else{
        std::shared_ptr<LevelNode> node = find(level);
        if (node == nullptr){
            std::shared_ptr<LevelNode> new_node(new LevelNode(level, level));
            new_node->changePlayers(1, score);
            this->insert(new_node);
        }
        else{
            std::shared_ptr<LevelNode> iterator = node;
            while (iterator != nullptr){
                iterator->changePlayers(iterator->getPlayers(score) + 1, score);
                iterator->changeSumLevels(iterator->getSumLevels() + level);
                iterator = iterator->getFather();
            }
        }
    } 
}

static int getPlayersInNode(std::shared_ptr<LevelNode> node, int score){
    int players = 0;
    if (score == 0){
        for (int i=1; i<=200; i++){
            players += node->getPlayers(i);
            if (node->getLeft() != nullptr)
                players -= node->getLeft()->getPlayers(i);
            if (node->getRight() != nullptr)
                players -= node->getRight()->getPlayers(i);
        }
    }else{
        players += node->getPlayers(score);
        if (node->getLeft() != nullptr)
            players -= node->getLeft()->getPlayers(score);
        if (node->getRight() != nullptr)
            players -= node->getRight()->getPlayers(score);
    }
    return players;
}

static bool isEmpty(std::shared_ptr<LevelNode> node){
    if(getPlayersInNode(node,0) == 0)
        return true;
    return false;
}

void Tree::removePlayer(int level, int score){
    if (level == 0){
        zero->changePlayers(zero->getPlayers(score) - 1, score); 
    }
    else{
        std::shared_ptr<LevelNode> node = find(level);
        std::shared_ptr<LevelNode> iterator = node;
        while (iterator != nullptr){
            iterator->changePlayers(iterator->getPlayers(score) - 1, score);
            iterator->changeSumLevels(iterator->getSumLevels() - level);
            iterator = iterator->getFather();
        }
        if (isEmpty(node))
            this->remove(level);
    }
}

int Tree::getNumOfPlayers(){
    int counter = 0;
    if (this->head != nullptr){
        for (int i=1; i<=200; i++)
            counter += this->head->getPlayers(i);
    }
    for (int i=1; i<=200; i++)
        counter += this->zero->getPlayers(i);
    return counter;
}  

double Tree::percentOfPlayersWithScoreInBounds(int score, int lowerLevel, int higherLevel){
    std::shared_ptr<LevelNode> max = this->getMaxLevel();
    std::shared_ptr<LevelNode> min = this->getMinLevel();
    if(higherLevel < 0 || lowerLevel > higherLevel || (max && (lowerLevel > max->getLevel())))
        return -1;
    if(!(score < 1 || score > 200)){
        int total_players, total_players_score;
        if (lowerLevel <= 0){
            if (max == nullptr || higherLevel < min->getLevel()){
                total_players_score = getRank(this->zero, score);
                total_players = getRank(this->zero, 0);
            }
            else{ //tree contains node in range
                if (higherLevel < max->getLevel())
                    max = this->findLower(higherLevel);
                total_players_score = getRank(max, score);
                total_players = getRank(max, 0);
            }
        }
        else{ 
            if (max == nullptr)
                return -1;
            if (lowerLevel <= min->getLevel())
                min = this->zero;
            else
                min = findLower(lowerLevel - 1);
            if (higherLevel < max->getLevel())
                max = this->findLower(higherLevel);
            if(max->getLevel() > higherLevel)
                max = this->zero;
            total_players_score = getRank(max, score) - getRank(min, score);
            total_players = getRank(max, 0) - getRank(min, 0);
        }
        if (total_players == 0)
            return -1;
        return (((double)total_players_score)/total_players)*100;
    }
    return 0;
}

std::shared_ptr<LevelNode> Tree::getMaxLevel(){
    if (this->head == nullptr)
        return nullptr;
    std::shared_ptr<LevelNode> iterator = this->head;
    while(iterator->getRight() != nullptr)
        iterator = iterator->getRight();
    return iterator;        
}

std::shared_ptr<LevelNode> Tree::getMinLevel(){
    if (this->head == nullptr)
        return nullptr;
    std::shared_ptr<LevelNode> iterator = this->head;
    while(iterator->getLeft() != nullptr)
        iterator = iterator->getLeft();
    return iterator;   
}

int Tree::getRank(std::shared_ptr<LevelNode> node, int score){ //returns rank by score, if 0 returns sum of all scores (for all nodes include zero)
    if (node == nullptr)
        return 0;
    int rank = 0;
    if (score == 0)
        for (int i=1; i<=200; i++)
            rank += zero->getPlayers(i);
    else
        rank += zero->getPlayers(score);
    if(node == zero)
        return rank;
    std::shared_ptr<LevelNode> iterator = this->head;
    while (iterator && !(iterator == node)){
        if (iterator->getLevel() < node->getLevel()){ //turning right
            if (score == 0){
                for (int i=1; i<=200; i++){
                    rank += iterator->getPlayers(i);
                    if (iterator->getRight() != nullptr)
                        rank -= iterator->getRight()->getPlayers(i);
                }
            }
            else{
                rank += iterator->getPlayers(score);
                if (iterator->getRight() != nullptr)
                    rank -= iterator->getRight()->getPlayers(score);
            }
            iterator = iterator->getRight();
        }
        else{ //turning left
            iterator = iterator->getLeft();
        }
    }
    if (score == 0)
        for (int i=1; i<=200; i++){
            rank += node->getPlayers(i);
            if (node->getRight() != nullptr)
                rank -= node->getRight()->getPlayers(i);
        }
    else{
        rank += node->getPlayers(score);
        if (node->getRight() != nullptr)
            rank -= node->getRight()->getPlayers(score);
    }
    return rank;
}

int Tree::getRankSumLevels(std::shared_ptr<LevelNode> node){ //returns rank by sumLevels
    if (node == nullptr)
        return 0;
    int rank = 0;
    std::shared_ptr<LevelNode> iterator = this->head;
    while (iterator && !(iterator == node)){
        if (iterator->getLevel() < node->getLevel()){ //turning right
            rank += iterator->getSumLevels();
            if (iterator->getRight() != nullptr)
                rank -= iterator->getRight()->getSumLevels();
            iterator = iterator->getRight();
        }
        else{ //turning left
            iterator = iterator->getLeft();
        }
    }
    rank += node->getSumLevels();
    if (node->getRight() != nullptr)
        rank -= node->getRight()->getSumLevels();
    return rank;
}

std::shared_ptr<LevelNode> Tree::findLower(int level){ //node.level <= level
    if (this->head == nullptr)
        return nullptr;
    if (this->find(level) != nullptr)
        return this->find(level); 
    if (this->getMinLevel()->getLevel() > level)
        return this->getMinLevel();
    std::shared_ptr<LevelNode> iterator = this->head;
    std::shared_ptr<LevelNode> lower = this->head;
    while (iterator != nullptr){
        if (iterator->getLeft() == nullptr && iterator->getRight()){
            if (iterator->getLevel() < level)
                lower = iterator;
            if(iterator->getRight()->getLevel() < level){
                iterator = iterator->getRight();
            } else {
                iterator = nullptr;
            }
        }
        else{
            if (iterator->getLevel() < level){
                lower = iterator;
                iterator = iterator->getRight();
            }
            else{
                iterator = iterator->getLeft();
            }
        }
    }
    return lower;
}

double Tree::getAverageHighestPlayerLevelByGroup(int m){
    if (this->head == nullptr)
        return 0;
    std::shared_ptr<LevelNode> node = select(m);
    int diff = m -(getNumOfPlayers() - getRank(node, 0)), total_sum_levels = this->head->getSumLevels(),
        sum_levels_node = getRankSumLevels(node);
    return (total_sum_levels - sum_levels_node + diff*node->getLevel())/((double)m);
}

std::shared_ptr<LevelNode> Tree::select(int rank){ //return node that has the m-th player - minimal rank(node) that: rank(node) >= rank
    std::shared_ptr<LevelNode> iterator = this->head;
    int current, left, right;
    while (iterator != nullptr){
        current = 0, left = 0, right = 0;
        if (iterator->getRight() == nullptr)
            right = 0;
        else
            for (int i=1; i<=200; i++)
                right += iterator->getRight()->getPlayers(i);
        if (right >= rank)
            iterator = iterator->getRight();
        else{
            if (iterator->getLeft() == nullptr)
                left = 0;
            else
                for (int i=1; i<=200; i++)
                    left += iterator->getLeft()->getPlayers(i);
            for (int i=1; i<=200; i++)
                current += iterator->getPlayers(i);
            current -= (left+right);
            if ((current+right) >= rank)
                return iterator;
            rank -= (current + right);
            iterator = iterator->getLeft();
        }
    }
    return this->zero;    
}



void Tree::playersBound(int score, int m, int* LowerBoundPlayers, int* HigherBoundPlayers){
    if(m == 0){
        *LowerBoundPlayers = 0;
        *HigherBoundPlayers = 0;
        return;
    }
    std::shared_ptr<LevelNode> node = this->select(m);
    *LowerBoundPlayers = this->zero->getPlayers(score) - getRank(node, score);
    if(this->head)
        *LowerBoundPlayers += this->head->getPlayers(score);
    int total = this->getNumOfPlayers();
    int left_players = m - (total - getRank(node,0));
    if (left_players >= getPlayersInNode(node, score))
        *HigherBoundPlayers = *LowerBoundPlayers + getPlayersInNode(node, score);
    else
        *HigherBoundPlayers = *LowerBoundPlayers + left_players;
    total = getPlayersInNode(node, 0);
    if((total - getPlayersInNode(node, score)) < left_players)
        *LowerBoundPlayers += (left_players - (total - getPlayersInNode(node, score)));
}

