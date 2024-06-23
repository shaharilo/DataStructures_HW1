//
// Created by shaha on 02/12/2022.
//

#include "PlayersTree.h"

void addAllPlayersGames(TreeNode<Player*>* root ,int num_of_plays){
    if (root == nullptr) return;
    root->getData()->setNumOfGamesPlayed(root->getData()->getNumOfGamesPlayed() + num_of_plays);
    addAllPlayersGames(root->getLeft(), num_of_plays);
    addAllPlayersGames(root->getRight(), num_of_plays);
}

//Assume that the node exists in the tree
//Player* closestToTheLeft(TreeNode<Player*>* node){
//
//    if (!node->getParent()){
//        if (!node->getLeft()) return nullptr;
//
//        TreeNode<Player*>* curr_node = node->getLeft();
//        while(curr_node->getRight()){
//            curr_node = curr_node->getRight();
//        }
//        return curr_node->getData();
//    }
//
//    else{
//        TreeNode<Player*>* parent_path_max = nullptr;
//        TreeNode<Player*>* curr_node = node;
//        while(curr_node->getParent()){
//            curr_node = curr_node->getParent();
//            if (curr_node->getSortKey1() <= node->getSortKey1()) {
//                if (parent_path_max == nullptr) parent_path_max = curr_node;
//                else if (keyComparisonForClosest(node->getData(), parent_path_max->getData(),
//                                                 curr_node->getData()) == -1) parent_path_max = curr_node;
//            }
//        }
//        TreeNode<Player*>* left_path_max = node->getLeft();
//        if (node->getLeft()) {
//            while(left_path_max->getRight()){
//                left_path_max = left_path_max->getRight();
//            }
//        }
//        if (parent_path_max == nullptr && left_path_max == nullptr){
//            return nullptr;
//        }
//        else if (parent_path_max == nullptr){
//            return left_path_max->getData();
//        }
//        else if (left_path_max == nullptr){
// //           Player* player_to_copy = parent_path_max->getData();
// //           std::cout << "player to copy id" << player_to_copy->getId() << std::endl;
////            Player* closest_left = new Player(player_to_copy->getId(),player_to_copy->getTeam(),
////                                               player_to_copy->getNumOfGamesPlayed(),player_to_copy->getGoals(),
////                                               player_to_copy->getCards(),player_to_copy->getGoalKeeperStatus());
////            return closest_left;
//            return parent_path_max->getData();
//        }
//        return (keyComparisonForClosest(node->getData(), parent_path_max->getData(),
//                                        curr_node->getData()) == 1)? parent_path_max->getData():left_path_max->getData();
//    }
//}

int closestToTheLeft(TreeNode<Player*>* node){
    if (!node->getParent()){
        if (!node->getLeft()) return 0;

        TreeNode<Player*>* curr_node = node->getLeft();
        while(curr_node->getRight()){
            curr_node = curr_node->getRight();
        }
        return curr_node->getData()->getId();
    }

    else{
        TreeNode<Player*>* parent_path_max = nullptr;
        TreeNode<Player*>* curr_node = node;
        while(curr_node->getParent()){
            curr_node = curr_node->getParent();
            if (curr_node->getSortKey1() < node->getSortKey1() ||
                (curr_node->getSortKey1() == node->getSortKey1() && curr_node->getSortKey2() > node->getSortKey2()) ||
                (curr_node->getSortKey1() == node->getSortKey1() && curr_node->getSortKey2() == node->getSortKey2() &&
                 curr_node->getSortKey3() < node->getSortKey3())) {
//            if (curr_node->getSortKey1() <= node->getSortKey1()) {
                if (parent_path_max == nullptr) parent_path_max = curr_node;
                else if (keyComparisonForClosest(node->getData(), parent_path_max->getData(),
                                                 curr_node->getData()) == -1) parent_path_max = curr_node;
            }
        }
        TreeNode<Player*>* left_path_max = node->getLeft();
        if (node->getLeft()) {
            while(left_path_max->getRight()){
                left_path_max = left_path_max->getRight();
            }
        }
        if (parent_path_max == nullptr && left_path_max == nullptr){
            return 0;
        }
        else if (parent_path_max == nullptr){
            return left_path_max->getData()->getId();
        }
        else if (left_path_max == nullptr){
            return parent_path_max->getData()->getId();
        }
        return (keyComparisonForClosest(node->getData(), parent_path_max->getData(),
                                        left_path_max->getData()) == 1)? parent_path_max->getData()->getId():left_path_max->getData()->getId();
    }
}


//Player* closestToTheRight(TreeNode<Player*>* node){
//    if (!(node->getParent())){ //node is the root
//        if (!node->getRight()) return nullptr;
//
//        TreeNode<Player*>* curr_node = node->getRight();
//        while(curr_node->getLeft()){
//            curr_node = curr_node->getLeft();
//        }
//
//        return (curr_node)->getData();
//        //return closest_right;
//    }
//
//    else{ //node is not root
//        TreeNode<Player*>* parent_path_max = nullptr;
//        TreeNode<Player*>* curr_node = node;
//        while(curr_node->getParent()){
//            curr_node = curr_node->getParent();
//            if (curr_node->getSortKey1() >= node->getSortKey1()) {
//                if (parent_path_max == nullptr) parent_path_max = curr_node;
//                else if (keyComparisonForClosest(node->getData(), parent_path_max->getData(),
//                                                 curr_node->getData()) == -1) parent_path_max = curr_node;
//            }
//        }
//        TreeNode<Player*>* right_path_max = node->getRight();
//        if (node->getRight()) {
//            while(right_path_max->getLeft()){
//                right_path_max = right_path_max->getLeft();
//            }
//        }
//
//        if (parent_path_max == nullptr && right_path_max == nullptr){
//            return nullptr;
//        }
//        else if (parent_path_max == nullptr) return right_path_max->getData();
//        else if (right_path_max == nullptr) return parent_path_max->getData();
//        return (keyComparisonForClosest(node->getData(), parent_path_max->getData(),
//                                        right_path_max->getData()) == 1)? parent_path_max->getData():right_path_max->getData();
//    }
//}


int closestToTheRight(TreeNode<Player*>* node){
    if (!(node->getParent())){ //node is the root
        if (!node->getRight()) return 0;

        TreeNode<Player*>* curr_node = node->getRight();
        while(curr_node->getLeft()){
            curr_node = curr_node->getLeft();
        }

        return (curr_node)->getData()->getId();
        //return closest_right;
    }

    else{ //node is not root
        TreeNode<Player*>* parent_path_max = nullptr;
        TreeNode<Player*>* curr_node = node;
        while(curr_node->getParent()){
            curr_node = curr_node->getParent();
            if (curr_node->getSortKey1() > node->getSortKey1() ||
                    (curr_node->getSortKey1() == node->getSortKey1() && curr_node->getSortKey2() < node->getSortKey2()) ||
                    (curr_node->getSortKey1() == node->getSortKey1() && curr_node->getSortKey2() == node->getSortKey2() &&
                    curr_node->getSortKey3() > node->getSortKey3())) {
                if (parent_path_max == nullptr) parent_path_max = curr_node;
                else if (keyComparisonForClosest(node->getData(), parent_path_max->getData(),
                                                 curr_node->getData()) == 1) parent_path_max = curr_node;
            }
        }

        TreeNode<Player*>* right_path_max = node->getRight();
        if (node->getRight()) {
            while(right_path_max->getLeft()){
                right_path_max = right_path_max->getLeft();
            }
        }

        if (parent_path_max == nullptr && right_path_max == nullptr){
            return 0;
        }
        else if (parent_path_max == nullptr) return right_path_max->getData()->getId();
        else if (right_path_max == nullptr) return parent_path_max->getData()->getId();
        return (keyComparisonForClosest(node->getData(), parent_path_max->getData(),
                                        right_path_max->getData()) == -1)? parent_path_max->getData()->getId():right_path_max->getData()->getId();
    }
}

