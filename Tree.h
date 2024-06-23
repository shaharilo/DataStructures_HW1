
#ifndef WET1_TREE_H
#define WET1_TREE_H
#pragma once

#include <iostream>
#include "TreeNode.h"
using std::cout;
using std::endl;

enum returnOption {SUCCESS,FAILURE,ALREADY_EXIST, NOT_VALID};
enum operationType {INSERT,REMOVE};



template<class Data>
class Tree {
    TreeNode<Data> *root; //pointer to the root of the tree
    int num_of_node;

public:
    Tree() : root(nullptr), num_of_node(0) {};

    virtual ~Tree() = default;

    TreeNode<Data> *getRoot() {
        return this->root;
    }

    int getNumOfNode() {
        return this->num_of_node;
    }

    void setRoot(TreeNode<Data> *new_root) {
        this->root = new_root;
    }

    void setNumOfNode(int new_num_of_node) {
        this->num_of_node = new_num_of_node;
    }

    TreeNode<Data> *findNode(TreeNode<Data> *node_to_find) {
        TreeNode<Data> *current_root = this->root;
        while (current_root != nullptr) {
            if (*current_root == *node_to_find) {
                return current_root;
            } else if (*current_root > *node_to_find) {
                current_root = current_root->getLeft();
            } else {
                current_root = current_root->getRight();
            }
        }
        return nullptr;
    }

    TreeNode<Data> *findNode(int key) {
        TreeNode<Data> *current_root = this->root;
        while (current_root != nullptr) {
            if (current_root->getSortKey1() == key) {
                return current_root;
            } else if (current_root->getSortKey1() > key) {
                current_root = current_root->getLeft();
            } else {
                current_root = current_root->getRight();
            }
        }
        return nullptr;
    }

    returnOption insertNode(TreeNode<Data> *new_node) {
        if (this->findNode(new_node) != nullptr) return ALREADY_EXIST;
        TreeNode<Data> *current_root = this->root;
        if (!current_root) this->setRoot(new_node);
        while (current_root != nullptr) {
            if (*current_root > *new_node) {
                if (current_root->getLeft() == nullptr) {
                    current_root->setLeft(new_node);
                    new_node->setParent(current_root);
                    break;
                } else {
                    current_root = current_root->getLeft();
                }
            } else {
                if (current_root->getRight() == nullptr) {
                    current_root->setRight(new_node);
                    new_node->setParent(current_root);
                    break;
                } else {
                    current_root = current_root->getRight();
                }
            }
        }
        current_root = new_node;
        while (current_root) {
            //try to fix bug
           // int H_before_update = current_root->getHeight();
           // int BF_before_update = current_root->getBalanceFactor();
            updateHeightAndBalanceFactorForNode(current_root);
            if (current_root->getBalanceFactor() == 2 || current_root->getBalanceFactor() == -2) break;
           // if (current_root->getHeight() == H_before_update && current_root->getBalanceFactor() == BF_before_update && current_root != new_node) break;
            current_root = current_root->getParent();
        }
        fixTreeBalance(new_node, INSERT);

        this->setNumOfNode((this->getNumOfNode()) + 1);
        return SUCCESS;
    }

    //ATTENTION:The function assumes that the root is one of the fatherhood of the leaf
    void updateTreeHeightsAndBalanceFactorFromLeafToRoot(TreeNode<Data> *leaf, TreeNode<Data> *root) {
        if (!root || !leaf) return;

        TreeNode<Data> *curr_node;
        if (leaf->nodeIsLeaf()) {
            leaf->setHeight(0);
            leaf->setBalanceFactor(0);
            curr_node = leaf->getParent();
        } else curr_node = leaf;

        if (curr_node == nullptr) return;
        TreeNode<Data> *curr_son = leaf; // curr_son->getParent() = curr_node;
        while (curr_node != root) {
            updateHeightAndBalanceFactorForNode(curr_node);
            curr_son = curr_node;
            curr_node = curr_node->getParent();
        }

        TreeNode<Data> *root_before_rotate;
        side son_side = root->whichSideOfMeTheChildIs(curr_son);
        if (son_side == LEFT) root_before_rotate = root->getRight();
        else if (son_side == RIGHT) root_before_rotate = root->getLeft();

        if (root_before_rotate) updateHeightAndBalanceFactorForNode(root_before_rotate);
        updateHeightAndBalanceFactorForNode(root);



        while (root->getParent()) {
//        side root_side = root->whichSideOfMeTheChildIs(root);
//        if (root_side == IS_ROOT) return;
            updateHeightAndBalanceFactorForNode(root->getParent());
            root = root->getParent();
        }
    }

    void updateHeightAndBalanceFactorForNode(TreeNode<Data> *node) {
        TreeNode<Data> *left = node->getLeft();
        TreeNode<Data> *right = node->getRight();
        if (left == nullptr && right == nullptr) {
            node->setHeight(0);
            node->setBalanceFactor(0);
            return;
        }

        //update Heights
        if (!left) node->setHeight(right->getHeight() + 1);
        else if (!right) node->setHeight(left->getHeight() + 1);
        else {
            int left_height = left->getHeight();
            int right_height = right->getHeight();
            int max_height = right_height;
            if (left_height > right_height) max_height = left_height;
            node->setHeight(max_height + 1);
        }

        //update Balance Factor
        if (!left) node->setBalanceFactor(-1 - right->getHeight());
        else if (!right) node->setBalanceFactor(left->getHeight() +1);
        else node->setBalanceFactor(left->getHeight() - right->getHeight());
    }

    void updateHeightAndBalanceAfterUnite(TreeNode<Data> *sub_root) {
        if (!sub_root) return;

        updateHeightAndBalanceAfterUnite(sub_root->getLeft());
        updateHeightAndBalanceAfterUnite(sub_root->getRight());
        this->updateHeightAndBalanceFactorForNode(sub_root);
    }

    //here need to add recursion because when you delete node you have to do the fixing up until the root
    //bf = BalanceFactor
    void fixTreeBalance(TreeNode<Data>* inserted_node, operationType operation_type) {
        if (inserted_node == nullptr) return;
        //TreeNode<Data> *curr_node = inserted_node->getParent();
        TreeNode<Data> *curr_node = inserted_node;


        int left_bf = 0, right_bf = 0;

        if(curr_node== nullptr) return;
        int node_bf = curr_node->getBalanceFactor();
        while (curr_node) {
            if (node_bf == 2 || node_bf == -2) {
                if (curr_node->getRight() != nullptr) {
                    right_bf = curr_node->getRight()->getBalanceFactor();
                }
                if (curr_node->getLeft() != nullptr) {
                    left_bf = curr_node->getLeft()->getBalanceFactor();
                }

                if (node_bf == 2) {
                    if (left_bf >= 0) {
                        if (operation_type == INSERT) {
                            this->rotateLL(curr_node, inserted_node);
                            return;
                        }
                        else{
                            this->rotateLL(curr_node, curr_node->getLeft()->getLeft());
                        }
                    } else if (left_bf == -1) {
                        if (operation_type == INSERT) {
                            this->rotateLR(curr_node, inserted_node);
                            return;
                        }
                        else{
                            this->rotateLR(curr_node, curr_node->getLeft()->getRight());
                        }
                    }
                } else {
                    if (right_bf <= 0) {
                        if (operation_type == INSERT) {
                            this->rotateRR(curr_node, inserted_node);
                            return;
                        }
                        else{
                            this->rotateRR(curr_node, curr_node->getRight()->getRight());
                        }
                    } else if (right_bf == 1) {
                        if (operation_type == INSERT) {
                            this->rotateRL(curr_node, inserted_node);
                            return;
                        }
                        else{
                            this->rotateRL(curr_node, curr_node->getRight()->getLeft());
                        }
                    }
                }
            }
            curr_node = curr_node->getParent();
            if (curr_node) node_bf = curr_node->getBalanceFactor();
            left_bf = 0;
            right_bf = 0;
        }
    }

    returnOption removeNode(TreeNode<Data> *node_to_delete, removeNodeOption remove_node_option = WITHOUT_RELEASING_DATA) {
        if (!findNode(node_to_delete)) return NOT_VALID;

        if (node_to_delete->nodeIsLeaf() && node_to_delete->getParent() == nullptr){ //the last node in the tree
            if(remove_node_option == RELEASE_DATA) delete node_to_delete->getData();
            delete node_to_delete;
            this->setNumOfNode((this->getNumOfNode()) - 1);
            this->setRoot(nullptr);
            return SUCCESS;
        }

        side side_of_node_to_delete = node_to_delete->whichSideOfTheFatherPointsToMe();

        if (node_to_delete->nodeIsLeaf()){
            TreeNode<Data>* parent = node_to_delete->getParent();
            if (side_of_node_to_delete == LEFT) parent->setLeft(nullptr);
            else if (side_of_node_to_delete == RIGHT) parent->setRight(nullptr);
            else this->setRoot(nullptr);

            //if (remove_node_option == WITHOUT_RELEASING_DATA){}
            if (remove_node_option == RELEASE_DATA)  delete node_to_delete->getData();
            setNodePointersNULL(node_to_delete, remove_node_option);
            delete node_to_delete;

            TreeNode<Data>* current_root = parent;
            while(current_root){
                updateHeightAndBalanceFactorForNode(current_root);
                current_root = current_root->getParent();
            }
            fixTreeBalance(parent, REMOVE);

            this->setNumOfNode((this->getNumOfNode()) - 1);
            return SUCCESS;
        }

        //node is not a leaf
        TreeNode<Data> *new_root_of_sub_tree;
        if(!node_to_delete->getRight()){//node that is erased has no right son
            new_root_of_sub_tree = node_to_delete->getLeft();

            TreeNode<Data>* parent = node_to_delete->getParent();
            if (side_of_node_to_delete == LEFT) parent->setLeft(new_root_of_sub_tree);
            else if (side_of_node_to_delete == RIGHT) parent->setRight(new_root_of_sub_tree);
            else this->setRoot(new_root_of_sub_tree);

            new_root_of_sub_tree->setParent(parent);
            if (remove_node_option == RELEASE_DATA) delete node_to_delete->getData();
            setNodePointersNULL(node_to_delete, remove_node_option);
            delete node_to_delete;

            TreeNode<Data>* current_root = new_root_of_sub_tree;
            while(current_root){
                updateHeightAndBalanceFactorForNode(current_root);
                current_root = current_root->getParent();
            }

            fixTreeBalance(new_root_of_sub_tree, REMOVE);
        }
        else {
            if ((node_to_delete->getRight())->getLeft()) {
                new_root_of_sub_tree = findMinimumNodeTree(node_to_delete->getRight());
                TreeNode<Data> *new_root_of_sub_tree_original_parent = new_root_of_sub_tree->getParent();

                TreeNode<Data> *parent = node_to_delete->getParent();
                if (side_of_node_to_delete == LEFT) {
                    parent->setLeft(new_root_of_sub_tree);
                    new_root_of_sub_tree->setParent(parent);
                }
                else if (side_of_node_to_delete == RIGHT) {
                    parent->setRight(new_root_of_sub_tree);
                    new_root_of_sub_tree->setParent(parent);
                }
                else {
                    this->setRoot(new_root_of_sub_tree);
                    new_root_of_sub_tree->setParent(nullptr);
                }

                new_root_of_sub_tree_original_parent->setLeft(new_root_of_sub_tree->getRight());
                if (new_root_of_sub_tree_original_parent->getLeft())
                    (new_root_of_sub_tree_original_parent->getLeft())->setParent(new_root_of_sub_tree_original_parent);

                new_root_of_sub_tree->setLeft(node_to_delete->getLeft());
                if (node_to_delete->getLeft()) node_to_delete->getLeft()->setParent(new_root_of_sub_tree);

                new_root_of_sub_tree->setRight(node_to_delete->getRight());
                if (node_to_delete->getRight()) node_to_delete->getRight()->setParent(new_root_of_sub_tree);

                if (remove_node_option == RELEASE_DATA) delete node_to_delete->getData();
                setNodePointersNULL(node_to_delete, remove_node_option);
                delete node_to_delete;

                TreeNode<Data>* current_root = new_root_of_sub_tree_original_parent;
                while(current_root){
                    updateHeightAndBalanceFactorForNode(current_root);
                    current_root = current_root->getParent();
                }

                fixTreeBalance(new_root_of_sub_tree_original_parent, REMOVE);
            }
            else{
                new_root_of_sub_tree = node_to_delete->getRight();
                TreeNode<Data> *parent = node_to_delete->getParent();
                if (side_of_node_to_delete == LEFT) {
                    parent->setLeft(new_root_of_sub_tree);
                    new_root_of_sub_tree->setParent(parent);
                }
                else if (side_of_node_to_delete == RIGHT) {
                    parent->setRight(new_root_of_sub_tree);
                    new_root_of_sub_tree->setParent(parent);
                }
                else {
                    this->setRoot(new_root_of_sub_tree);
                    new_root_of_sub_tree->setParent(nullptr);
                }
                new_root_of_sub_tree->setLeft(node_to_delete->getLeft());
                if (new_root_of_sub_tree->getLeft()) (new_root_of_sub_tree->getLeft())->setParent(new_root_of_sub_tree);

                if (remove_node_option == RELEASE_DATA)  delete node_to_delete->getData();
                setNodePointersNULL(node_to_delete, remove_node_option);
                delete node_to_delete;

                TreeNode<Data>* current_root = new_root_of_sub_tree;
                while(current_root){
                    updateHeightAndBalanceFactorForNode(current_root);
                    current_root = current_root->getParent();
                }
                fixTreeBalance(new_root_of_sub_tree, REMOVE);
            }
        }

        this->setNumOfNode(this->getNumOfNode() - 1);
        return SUCCESS;
    }

    void rotateLL(TreeNode<Data> *original_parent, TreeNode<Data> *inserted_leaf){
        TreeNode<Data> *Left_child = original_parent->getLeft();

        //Updating the left part of the right child to be the right child of the original parent
        TreeNode<Data> *right_child_of_Left_child = Left_child->getRight();
        original_parent->setLeft(right_child_of_Left_child);
        if (right_child_of_Left_child) right_child_of_Left_child->setParent(original_parent);

        //Updating the pointer to this tree, to be a pointer to the right part of the tree
        side side_of_parent = original_parent->whichSideOfTheFatherPointsToMe();
        TreeNode<Data> *pointer_to_parent = original_parent->getParent();
        if (side_of_parent == IS_ROOT) {
            this->setRoot(Left_child);
            Left_child->setParent(nullptr);
        }
        else {
            if (side_of_parent == RIGHT) pointer_to_parent->setRight(Left_child);
            else pointer_to_parent->setLeft(Left_child);

            Left_child->setParent(pointer_to_parent);
        }

        //Update the right child to be the parent of the original parent
        Left_child->setRight(original_parent);
        original_parent->setParent(Left_child);

        if (inserted_leaf == Left_child) {
            //if (pointer_to_parent == nullptr) updateTreeHeightsAndBalanceFactorFromLeafToRoot(original_parent, Left_child);
            updateTreeHeightsAndBalanceFactorFromLeafToRoot(original_parent, Left_child);
        }
        else updateTreeHeightsAndBalanceFactorFromLeafToRoot(inserted_leaf, Left_child);

    }

    void rotateLR(TreeNode<Data> *original_parent, TreeNode<Data> *inserted_leaf) {
        this->rotateRR(original_parent->getLeft(),inserted_leaf);
        this->rotateLL(original_parent,inserted_leaf);

    }

    void rotateRR(TreeNode<Data> *original_parent, TreeNode<Data> *inserted_leaf) {
        TreeNode<Data> *right_child = original_parent->getRight();

        //Updating the left part of the right child to be the right child of the original parent
        TreeNode<Data> *left_child_of_right_child = right_child->getLeft();
        original_parent->setRight(left_child_of_right_child);
        if (left_child_of_right_child) left_child_of_right_child->setParent(original_parent);

        //Updating the pointer to this tree, to be a pointer to the right part of the tree
        side side_of_parent = original_parent->whichSideOfTheFatherPointsToMe();
        TreeNode<Data> *pointer_to_parent = original_parent->getParent();
        if (side_of_parent == IS_ROOT) {
            this->setRoot(right_child);
            right_child->setParent(nullptr);
        }
        else {
            if (side_of_parent == RIGHT) pointer_to_parent->setRight(right_child);
            else pointer_to_parent->setLeft(right_child);

            right_child->setParent(pointer_to_parent);
        }

        //Update the right child to be the parent of the original parent
        right_child->setLeft(original_parent);
        original_parent->setParent(right_child);

        if (inserted_leaf == right_child) updateTreeHeightsAndBalanceFactorFromLeafToRoot(original_parent, right_child);
        else updateTreeHeightsAndBalanceFactorFromLeafToRoot(inserted_leaf, right_child);



    }

    void rotateRL(TreeNode<Data> *original_parent, TreeNode<Data> *inserted_leaf) {
        this->rotateLL(original_parent->getRight(),inserted_leaf);
        this->rotateRR(original_parent,inserted_leaf);
    }


    void removeAllNodesFromTreeForUnite(TreeNode<Data>* node){
        if (!node) return;
        if (node->nodeIsLeaf()) {
            if (node->getParent() != nullptr) {
                side me_to_parent_side = node->whichSideOfTheFatherPointsToMe();
                if (me_to_parent_side == RIGHT) node->getParent()->setRight(nullptr);
                else if (me_to_parent_side == LEFT) node->getParent()->setLeft(nullptr);
            }
            setNodePointersNULL(node);
            delete node;
            return;
        }
        removeAllNodesFromTreeForUnite(node->getLeft());
        removeAllNodesFromTreeForUnite(node->getRight());
        removeAllNodesFromTreeForUnite(node);
    }

};


template<class Data>
TreeNode<Data>* findMinimumNodeTree(TreeNode<Data>* root){
    if (root->nodeIsLeaf()) return root;
    if (root->getLeft() != nullptr) return findMinimumNodeTree(root->getLeft());
    else return root;
}


/*--------functions for inorder array--------------*/
template<class Data>
//need to call this function with the root of the tree
void getInorderArray(int* const arr, TreeNode<Data>* node) {
    int i = 0;
    auxInorderArray(arr, node, &i);
}

template<class Data>
void auxInorderArray(int* const arr, TreeNode<Data>* node,int* i) {
    if(node == nullptr) return;
    auxInorderArray(arr,node->getLeft(),i);
    arr[*i] = node->getSortKey3();
    (*i)++;
    auxInorderArray(arr,node->getRight(),i);
}

template<class Data>
//need to call this function with the root of the tree
void getInorderArray(TreeNode<Data>** arr, TreeNode<Data>* node) {
    int i = 0;
   // TreeNode<Data>** start_of_arr = arr;
    auxInorderArray(arr, node, &i);
    //arr = start_of_arr;
}

template<class Data>
void auxInorderArray(TreeNode<Data>** arr, TreeNode<Data>* node,int* i) {
    if(node == nullptr) return;

    if (node->getLeft()) auxInorderArray(arr,node->getLeft(),i);

    arr[*i] = node; //removed here the & before node, remember that when debugging
    (*i)++;
    if (node->getRight()) auxInorderArray(arr,node->getRight(),i);
}

template<class Data>
TreeNode<Data>* findTheFirstInRange(TreeNode<Data>* root, int min_limit, int max_limit){
    if (!root) return nullptr;
    int rootId = root->getSortKey1();
    if (rootId >= min_limit && rootId <= max_limit) return root;

    TreeNode<Data>* res;
    if (rootId > max_limit) res = findTheFirstInRange(root->getLeft(), min_limit, max_limit);
    else if (rootId < min_limit) res = findTheFirstInRange(root->getRight(), min_limit, max_limit);

    return res;
}



template<class Data>
//need to call this function with the root of the tree
int getSizeOfInorderArray(TreeNode<Data>* node, int min_limit, int max_limit) {
    int counter = 0;
    TreeNode<Data> *relevant_root = findTheFirstInRange(node, min_limit, max_limit);
    auxSizeOfInorderArray(relevant_root, &counter, min_limit, max_limit);
    return counter;
}


template<class Data>
void auxSizeOfInorderArray(TreeNode<Data>* node,int* counter,int min_limit, int max_limit) {
    if(node == nullptr) return;

    TreeNode<Data>* left = node->getLeft();
    if(left) {
        if (left->getSortKey1() >= min_limit && left->getSortKey1() <= max_limit) {
            auxSizeOfInorderArray(left, counter, min_limit, max_limit);
        }
        else if (left->getLeft() != nullptr && left->getSortKey1() > max_limit) auxSizeOfInorderArray(left->getLeft(), counter, min_limit, max_limit);//
        else if (left->getRight() != nullptr) {
            auxSizeOfInorderArray(left->getRight(), counter, min_limit, max_limit);
        }
    }
    if(node && node->getSortKey1()>=min_limit && node->getSortKey1()<=max_limit){
        (*counter)++;
    }

    TreeNode<Data>* right = node->getRight();
    if(right) {
        if (right->getSortKey1() >= min_limit && right->getSortKey1() <= max_limit) {
            auxSizeOfInorderArray(right, counter, min_limit, max_limit);
        }
        else if (right->getRight() != nullptr && right->getSortKey1() < min_limit) auxSizeOfInorderArray(right->getRight(), counter, min_limit, max_limit);//
        else if (right->getLeft() != nullptr) {
            auxSizeOfInorderArray(right->getLeft(), counter, min_limit, max_limit);
        }
    }
}




//template<class Data>
///**
// * tree1 is the source and tree 2 is the destination, and also Free source tree
// * @tparam Data
// * @param tree1
// * @param tree2
// */
//void unite_trees(Tree<Data>* const source_tree, Tree<Data>* destination_tree) {
//    if(source_tree == nullptr) return;
//    if(destination_tree == nullptr){
//        destination_tree = source_tree;
//        return;
//    }
//    TreeNode<Data>** arr;
//    source_tree->getInorderArray(arr, source_tree->getRoot());
//    int size_arr = source_tree->getNumOfNode();
//    for (int i = 0; i < size_arr; i++){
//        TreeNode<Data>* curr_node = arr[i];
//        destination_tree->insertNode(*curr_node);
//        curr_node->setData(nullptr);
//        source_tree->removeNode(curr_node, WITHOUT_RELEASING_DATA);
//    }
//    delete source_tree;
//}


//template<class Data>
//void printArr(TreeNode<Data>** arr, int size, char* name){
//    std::cout << name << std::endl;
//
//    for (int i = 0; i < size; i++){
//        int key1 = arr[i]->getSortKey1();
//        int key2 = arr[i]->getSortKey2();
//        int key3 = arr[i]->getSortKey3();
//        std::cout << "i = " << i << ": " << key1 << "," << key2 << "," << key3 << "   " << std::flush;
//    }
//
//    std::cout << std::endl;
//}

template<class Data>
/**
 * destination_tree id differnt from source_tree1 and source_tree2 and destination_tree ist null
 * @tparam Data
 * @param tree1
 * @param tree2
 */
void unite_trees(Tree<Data>* source_tree1, Tree<Data>* source_tree2, Tree<Data>* destination_tree) {
    if(source_tree1 == nullptr && source_tree2 == nullptr) return;

    if (source_tree1->getRoot() == nullptr && source_tree2->getRoot() == nullptr){
        delete source_tree1;
        delete source_tree2;
        return;
    }

    int size_arr1 = source_tree1->getNumOfNode();
    int size_arr2 = source_tree2->getNumOfNode();
    TreeNode<Data>** arr1 = new TreeNode<Data>*[size_arr1];
    TreeNode<Data>** arr2 = new TreeNode<Data>*[size_arr2];
    TreeNode<Data>** arr_dest = new TreeNode<Data>*[size_arr1 + size_arr2];

    getInorderArray(arr1, source_tree1->getRoot());
    getInorderArray(arr2, source_tree2->getRoot());

    margeArrys(arr1, size_arr1, arr2, size_arr2, arr_dest);

    //Tree<Data>* marge_tree = new Tree<Data>();
    createTreeFromInorder(destination_tree, arr_dest, size_arr1 + size_arr2);
    destination_tree->setNumOfNode(size_arr1 + size_arr2);

    source_tree1->removeAllNodesFromTreeForUnite(source_tree1->getRoot());
    source_tree2->removeAllNodesFromTreeForUnite(source_tree2->getRoot());

    delete source_tree1;
    delete source_tree2;

    delete[] arr1;
    delete[] arr2;
    delete[] arr_dest;
}



template<class Data>
void margeArrys(TreeNode<Data>** source_arr1, int size1, TreeNode<Data>** source_arr2, int size2, TreeNode<Data>** destination_arr){
    int index1=0, index2=0, index3=0;
    for (; (index1 < size1) && (index2 < size2); index3++) {
        if (source_arr1[index1]->getSortKey1() < source_arr2[index2]->getSortKey1() ||
        (source_arr1[index1]->getSortKey1() == source_arr2[index2]->getSortKey1() &&
        source_arr1[index1]->getSortKey2() > source_arr2[index2]->getSortKey2())||
        (source_arr1[index1]->getSortKey1() == source_arr2[index2]->getSortKey1() &&
        source_arr1[index1]->getSortKey2() == source_arr2[index2]->getSortKey2() &&
        source_arr1[index1]->getSortKey3() < source_arr2[index2]->getSortKey3())) {
            destination_arr[index3] = source_arr1[index1];
            index1++;
        }
        else {
            destination_arr[index3] = source_arr2[index2];
            index2++;
        }
    }
    for (; index1 < size1; index1++, index3++) {
        destination_arr[index3] = source_arr1[index1];
    }
    for (; index2 < size2; index2++, index3++) {
        destination_arr[index3] = source_arr2[index2];
    }
}


//this function returns the root of the tree
template<class Data>
void createTreeFromInorder(Tree<Data>* tree,TreeNode<Data>** arr, int arr_size){
    int i=0;
    TreeNode<Data>* root = auxCreateUnitedTree(tree,arr,i,arr_size-1);
    root->setParent(nullptr);
    tree->setRoot(root);
    tree->updateHeightAndBalanceAfterUnite(tree->getRoot());
}

template<class Data>
TreeNode<Data>* auxCreateUnitedTree(Tree<Data>* tree,TreeNode<Data>** arr, int start,int end);

template<class Data>
TreeNode<Data>* auxCreateUnitedTree(Tree<Data>* tree,TreeNode<Data>** arr, int start,int end){
    if(start > end) return nullptr;
    int mid = start + (end - start)/2;
    TreeNode<Data>* root = new TreeNode<Data>(*arr[mid]);
    root->setParent(nullptr);
    root->setLeft(nullptr);
    root->setRight(nullptr);
    root->setHeight(0);
    root->setBalanceFactor(0);

    TreeNode<Data>* left = auxCreateUnitedTree(tree, arr, start, mid-1);
    root->setLeft(left);
    if (left) left->setParent(root);
    TreeNode<Data>* right = auxCreateUnitedTree(tree, arr, mid+1, end);
    root->setRight(right);
    if (right) right->setParent(root);


    //updateHeightAndBalanceFactorForNode(root);
    return root;
}

#endif //WORLDCUP23A1_H_TREE_H
