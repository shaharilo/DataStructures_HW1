
#ifndef WET1_PLAYERSTREE_H
#define WET1_PLAYERSTREE_H

#pragma once

#include "Player.h"
#include "TreeNode.h"
#include "Tree.h"

class PlayersTree : public Tree<Player*>{
private:
    Player* top_scorer_player;

public:
    PlayersTree(): Tree<Player*>(), top_scorer_player(nullptr){};
    explicit PlayersTree(Player* top_scorer_player):Tree<Player*>(), top_scorer_player(top_scorer_player){};
    virtual ~PlayersTree() = default;
    PlayersTree(const PlayersTree& p_tree) = default;

    Player* getTopScorerPlayer(){
        return this->top_scorer_player;
    }

    void setTopScorerPlayer(Player* player){
        this->top_scorer_player = player;
    }

    void removeAllNodesFromTree(TreeNode<Player*>* root, removeNodeOption remove_node_option = WITHOUT_RELEASING_DATA){
        if (!root) return;
        if (root->nodeIsLeaf()) {
            // this->removeNode(root, remove_node_option);

            if (root->getParent() != nullptr) {
                side me_to_parent_side = root->whichSideOfTheFatherPointsToMe();
                if (me_to_parent_side == RIGHT) root->getParent()->setRight(nullptr);
                else if (me_to_parent_side == LEFT) root->getParent()->setLeft(nullptr);
            }
            if (remove_node_option == RELEASE_DATA) {
                delete (root->getData());
                //setNodePointersNULL(root);
            }
            setNodePointersNULL(root);
            delete root;
            return;
        }
        removeAllNodesFromTree(root->getLeft(), remove_node_option);
        removeAllNodesFromTree(root->getRight(), remove_node_option);
        removeAllNodesFromTree(root, remove_node_option);
    }

};
void addAllPlayersGames(TreeNode<Player*>* root ,int num_of_plays);
//Player* closestToTheLeft(TreeNode<Player*>* node);
//Player* closestToTheRight(TreeNode<Player*>* node);

int closestToTheLeft(TreeNode<Player*>* node);
int closestToTheRight(TreeNode<Player*>* node);

#endif //WORLDCUP23A1_H_PLAYERSTREE_H
