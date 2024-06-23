//
// Created by Toam Elharar on 26/11/2022.
//

#include "Team.h"

Team::Team():id(0),points(0),goal_keeper_number(0),goals_of_all_players_in_total(0),cards_of_all_players_in_total(0),
        num_of_games_played(0)
        {
        this->players_tree_by_id = new PlayersTree();
        this->players_tree_by_goals = new PlayersTree();
        }

Team::Team(int id, int points, int goal_keeper_number, PlayersTree* players_tree_by_id,
           PlayersTree* players_tree_by_goals, int goals_of_all_players_in_total, int cards_of_all_players_in_total,
           int num_of_games_played, Player* top_scorer_player):id(id), points(points),
                                                               goal_keeper_number(goal_keeper_number), players_tree_by_id(players_tree_by_id),
           players_tree_by_goals(players_tree_by_goals), goals_of_all_players_in_total(goals_of_all_players_in_total),
           cards_of_all_players_in_total(cards_of_all_players_in_total), num_of_games_played(num_of_games_played){}

//Team::~Team() {
//    if(players_tree_by_id) players_tree_by_id->removeAllNodesFromTree(players_tree_by_id->getRoot());
//    //delete players_tree_by_id;
//    if(players_tree_by_goals) players_tree_by_goals->removeAllNodesFromTree(players_tree_by_goals->getRoot());
//    //delete players_tree_by_goals;
//}

int Team::getId(){
    return this->id;
}

int Team::getPoints(){
    return this->points;
}

int Team::getGoalKeeperNumber(){
    return this->goal_keeper_number;
}

PlayersTree* Team::getPlayersTreeById(){
    return this->players_tree_by_id;
}

PlayersTree* Team::getPlayersTreeByGoals(){
    return this->players_tree_by_goals;
}

int Team::getGoalsOfAllPlayersInTotal(){
    return this->goals_of_all_players_in_total;
}

int Team::getCardsOfAllPlayersInTotal(){
    return this->cards_of_all_players_in_total;
}

int Team::getNumOfGamesPlayed(){
    return this->num_of_games_played;
}

//Player* Team::getTopScorerPlayer(){
//    return this->top_scorer_player;
//}

void Team::setId(int id){
    this->id = id;
}

void Team::setPoints(int points){
    this->points = points;
}

void Team::setGoalKeeperNumber(int num){
    this->goal_keeper_number = num;
}

void Team::setPlayersTreeById(PlayersTree* tree){
    this->players_tree_by_id = tree;
}

void Team::setPlayersTreeByGoals(PlayersTree* tree){
    this->players_tree_by_goals = tree;
}

void Team::setGoalsOfAllPlayersInTotal(int total_goals){
    this->goals_of_all_players_in_total = total_goals;
}

void Team::setCardsOfAllPlayersInTotal(int total_cards){
    this->cards_of_all_players_in_total = total_cards;
}

void Team::setNumOfGamesPlayed(int num_of_games_played){
    this->num_of_games_played = num_of_games_played;
}

TreeNode<Team*>* findTheFirstInRange(TreeNode<Team*>* root, int min_limit, int max_limit){
    if (!root) return nullptr;
    int rootId = root->getSortKey1();
    if (rootId >= min_limit && rootId <= max_limit) return root;

    TreeNode<Team*>* res;
    if (rootId > max_limit) res = findTheFirstInRange(root->getLeft(), min_limit, max_limit);
    else if (rootId < min_limit) res = findTheFirstInRange(root->getRight(), min_limit, max_limit);

    return res;
}

//need to call this function with the root of the tree
void getInorderArraySizeR(int* const arr1, int* const arr2, TreeNode<Team*>* node, int min_limit, int max_limit) {
    int i = 0;
    TreeNode<Team*> *relevant_root = findTheFirstInRange(node, min_limit, max_limit);
    auxInorderArraySizeR(arr1,arr2, relevant_root, &i, min_limit, max_limit);
}

void auxInorderArraySizeR(int* const arr1, int* const arr2, TreeNode<Team*>* node,int* i,int min_limit, int max_limit) {
    if(node == nullptr) return;

    TreeNode<Team*>* left = node->getLeft();
    if(left) {
        if (node->getLeft()->getSortKey1() >= min_limit && node->getLeft()->getSortKey1() <= max_limit) {
            auxInorderArraySizeR(arr1, arr2, node->getLeft(), i, min_limit, max_limit);
        }
        else if (left->getLeft() != nullptr && left->getSortKey1() > max_limit) auxInorderArraySizeR(arr1, arr2, node->getLeft()->getLeft(), i, min_limit, max_limit);//
        else if (node->getLeft()->getRight() != nullptr) {
            auxInorderArraySizeR(arr1, arr2, node->getLeft()->getRight(), i, min_limit, max_limit);
        }
    }
    if(node->getSortKey1()>=min_limit && node->getSortKey1()<=max_limit){
        arr1[*i] = node->getSortKey1();
        arr2[*i] = node->getData()->getPoints()+node->getData()->getGoalsOfAllPlayersInTotal() - node->getData()->getCardsOfAllPlayersInTotal();
        (*i)++;
    }

    TreeNode<Team*>* right = node->getRight();
    if(right) {
        if (node->getRight()->getSortKey1() >= min_limit && node->getRight()->getSortKey1() <= max_limit) {
            auxInorderArraySizeR(arr1, arr2, node->getRight(), i, min_limit, max_limit);
        }
        else if (right->getRight() != nullptr && right->getSortKey1() < min_limit) auxInorderArraySizeR(arr1, arr2, node->getRight()->getRight(), i, min_limit, max_limit);//
        else if (node->getRight()->getLeft() != nullptr) {
            auxInorderArraySizeR(arr1, arr2, node->getRight()->getLeft(), i, min_limit, max_limit);
        }
    }
}

//void Team::setTopScorerPlayer(Player* top_scorer_player){
//    this->top_scorer_player = top_scorer_player;
//}

//returnOption Team::removePlayer(int player_id){
//    TreeNode<Player*> player_to_remove = *(this->players_tree_by_id->findNode(player_id));
//
//    return SUCCESS;
//}

//bool Team::isValidToPlay(){
//    if (goal_keeper_status && (this->players_tree_by_id)->getNumOfNode() >= 11) return true;
//    return false;
//}

//void Team::insertPlayer(TreeNode<Player*>* new_player_by_id, TreeNode<Player*>* new_player_by_goals){
//    (this->getPlayersTreeById())->insertNode(new_player_by_id);
//    (this->getPlayersTreeByGoals())->insertNode(new_player_by_goals);
//}