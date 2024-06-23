
#ifndef WET1_TEAMSTREE_H
#define WET1_TEAMSTREE_H
#pragma once

#include "Team.h"
#include "Tree.h"

class TeamsTree : public Tree<Team*>{
public:
    TeamsTree(): Tree<Team*>(){};
    virtual ~TeamsTree() = default;

    void removeAllNodesFromTree(TreeNode<Team*>* root, removeNodeOption remove_node_option = WITHOUT_RELEASING_DATA){
        if (!root) return;
        if (root->nodeIsLeaf()){
            if (remove_node_option == RELEASE_DATA){
                PlayersTree* players_by_id = root->getData()->getPlayersTreeById();
                players_by_id->removeAllNodesFromTree(players_by_id->getRoot());
                delete players_by_id;
                PlayersTree* players_by_goals = root->getData()->getPlayersTreeByGoals();
                players_by_goals->removeAllNodesFromTree(players_by_goals->getRoot());
                delete players_by_goals;
            }

            side me_to_parent_side = root->whichSideOfTheFatherPointsToMe();
            if (me_to_parent_side == RIGHT) root->getParent()->setRight(nullptr);
            else if (me_to_parent_side == LEFT) root->getParent()->setLeft(nullptr);
            else this->setRoot(nullptr);

            if(remove_node_option == RELEASE_DATA){
                delete (root->getData());
                //setNodePointersNULL(root);
            }
            setNodePointersNULL(root);
            delete root;
            return;
        }
        removeAllNodesFromTree(root->getRight(), remove_node_option);
        removeAllNodesFromTree(root->getLeft(), remove_node_option);
        removeAllNodesFromTree(root, remove_node_option);
    }
};

#endif //WET1_TEAMSTREE_H
