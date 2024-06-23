//
// Created by shaha on 29/11/2022.
//

#ifndef WET1_TREENODE_H
#define WET1_TREENODE_H
#pragma once


enum side {RIGHT,LEFT,IS_ROOT, IS_NOT_A_CHILD};
enum removeNodeOption {RELEASE_DATA, WITHOUT_RELEASING_DATA};

//DONT INCLUDE ANYTHING HERE!!!!!!!!! WE DONT WANT CIRCLES

template<class Data>
class TreeNode{
    Data data;
    int sort_key1;
    int sort_key2;
    int sort_key3;
    int sort_key4;
    TreeNode* left;
    TreeNode* right;
    TreeNode* parent;
    int height;
    int balance_factor;
//    TreeNode* closest_left;
//    TreeNode* closest_right;

public:
    TreeNode():data(nullptr),sort_key1(0),sort_key2(0),sort_key3(0),
               sort_key4(0),left(nullptr),right(nullptr),parent(nullptr),height(0),balance_factor(0){};
    explicit TreeNode(Data data, int sort_key1,int sort_key2 = 0, int sort_key3 = 0,int sort_key4 = 0,
                      TreeNode* left = nullptr,TreeNode* right = nullptr,TreeNode* parent = nullptr ,int height = 0,
                      int balance_factor = 0):
            data(data),sort_key1(sort_key1),sort_key2(sort_key2),
            sort_key3(sort_key3),sort_key4(sort_key4),left(left),right(right),parent(parent),height(height),
            balance_factor(balance_factor){};
    ~TreeNode() = default;
    TreeNode(const TreeNode &tree_node) = default;

    Data getData(){
        return this->data;
    }

    TreeNode* getRight() {
        return this->right;
    }

    TreeNode* getLeft() {
        return this->left;
    }

    TreeNode* getParent() {
        return this->parent;
    }

    int getSortKey1(){
        return this->sort_key1;
    }

    int getSortKey2(){
        return this->sort_key2;
    }

    int getSortKey3(){
        return this->sort_key3;
    }

    int getSortKey4(){
        return this->sort_key4;
    }

    int getHeight(){
        return this->height;
    }

    int getBalanceFactor() {
        return this->balance_factor;
    }


    void setData(Data data){
        this->data = data;
    }

    void setRight(TreeNode* node) {
        this->right = node;
    }

    void setLeft(TreeNode* node) {
        this->left = node;
    }

    void setParent(TreeNode* node) {
        this->parent = node;
    }

    void setSortKey1(int key){
        this->sort_key1 = key;
    }

    void setSortKey2(int key){
        this->sort_key2 = key;
    }

    void setSortKey3(int key){
        this->sort_key3 = key;
    }

    void setSortKey4(int key){
        this->sort_key4 = key;
    }

    void setKey(int key1, int key2 = 0, int key3 = 0, int key4 = 0) {
        this->setSortKey1(key1);
        this->setSortKey2(key2);
        this->setSortKey3(key3);
        this->setSortKey4(key4);
    }

    void setHeight(int height){
        this->height = height;
    }

    void addHeight(int height_to_add){
        this->height += height_to_add;
    }

    void setBalanceFactor(int bf) {
        this->balance_factor = bf;
    }

    side whichSideOfTheFatherPointsToMe(){
        TreeNode *pointer_to_parent = this->getParent();
        if(!pointer_to_parent) return IS_ROOT;
        if (pointer_to_parent->getLeft() == this) return LEFT;
        return RIGHT;
    }

    side whichSideOfMeTheChildIs(TreeNode* child){
        if(this->left == child) return LEFT;
        else if(this->right == child) return RIGHT;
        return IS_NOT_A_CHILD;
    }

    bool nodeIsLeaf(){
        TreeNode* left_son = this->left;
        TreeNode* right_son = this->right;
        if (left_son== nullptr && right_son== nullptr) return true;
        return false;
    }

    friend bool operator>(TreeNode& node1, TreeNode& node2) {
        if (node1.getSortKey1() > node2.getSortKey1()) return true;
        else if (node1.getSortKey1() < node2.getSortKey1()) return false;
        //sort_key1 is the same

        if (node1.getSortKey2() < node2.getSortKey2()) return true;
        else if (node1.getSortKey2() > node2.getSortKey2()) return false;
        //sort_key2 is the same

        if (node1.getSortKey3() > node2.getSortKey3()) return true;
        else if (node1.getSortKey3() < node2.getSortKey3()) return false;
        //sort_key3 is the same

        if (node1.getSortKey4() > node2.getSortKey4()) return true;
        else if (node1.getSortKey4() < node2.getSortKey4()) return false;

        return false;

    }

    friend bool operator<(TreeNode& node1, TreeNode& node2){
        if (node1.getSortKey1() > node2.getSortKey1()) return false;
        else if (node1.getSortKey1() < node2.getSortKey1()) return true;
        //sort_key1 is the same

        if (node1.getSortKey2() < node2.getSortKey2()) return false;
        else if (node1.getSortKey2() > node2.getSortKey2()) return true;
        //sort_key2 is the same

        if (node1.getSortKey3() > node2.getSortKey3()) return false;
        else if (node1.getSortKey3() < node2.getSortKey3()) return true;
        //sort_key3 is the same

        if (node1.getSortKey4() > node2.getSortKey4()) return false;
        else if (node1.getSortKey4() < node2.getSortKey4()) return true;

        return false;
    }

};

//int absolute(int a);

template<class Data>
bool operator==(TreeNode<Data>& node1, TreeNode<Data>& node2){
    return !(node1 > node2 || node1 < node2);
}

template<class Data>
void setNodePointersNULL(TreeNode<Data>* node, removeNodeOption remove_node_option = WITHOUT_RELEASING_DATA ){
    if (remove_node_option == WITHOUT_RELEASING_DATA) node->setData(nullptr);
    node->setLeft(nullptr);
    node->setRight(nullptr);
    node->setParent(nullptr);
}

/*
 * return 1 if node1 closer to node from node2
 * return -1 if node2 closer to node from node1
 */
//template<class Data>
//int keyComparisonForClosest(TreeNode<Data>& node, TreeNode<Data>& node1, TreeNode<Data>& node2){
//    if (absolute(node.getSortKey1() - node1.getSortKey1()) < absolute(node.getSortKey1() - node2.getSortKey1())) return 1;
//    else if (absolute(node.getSortKey1() - node1.getSortKey1()) > absolute(node.getSortKey1() - node2.getSortKey1())) return -1;
//    //sort_key1 is the same
//
//    if (absolute(node.getSortKey2() - node1.getSortKey2()) < absolute(node.getSortKey2() - node2.getSortKey2())) return 1;
//    else if (absolute(node.getSortKey2() - node1.getSortKey2()) > absolute(node.getSortKey2() - node2.getSortKey2())) return -1;
//
//
//    if (absolute(node.getSortKey3() - node1.getSortKey3()) < absolute(node.getSortKey3() - node2.getSortKey3())) return 1;
//    else if (absolute(node.getSortKey3() - node1.getSortKey3()) > absolute(node.getSortKey3() - node2.getSortKey3())) return -1;
//
//    return 0;
//}

#endif //WET1_TREENODE_H
