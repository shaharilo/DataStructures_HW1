//
// Created by Toam Elharar on 26/11/2022.
//

#include "Player.h"

Player::Player():id(0),team(nullptr), num_of_games_played(0),goals(0), cards(0), goal_keeper_status(false),closest_left(
        nullptr),closest_right(nullptr){}

Player::Player(int id,Team* team, int num_of_games_played,int goals,int cards,bool goal_keeper_status,
               Player* closest_left, Player* closest_right):
               id(id),team(team), num_of_games_played(num_of_games_played),goals(goals), cards(cards),
               goal_keeper_status(goal_keeper_status),closest_left(closest_left),closest_right(closest_right){}


int Player::getId() {
    return this->id;
}
int Player::getNumOfGamesPlayed(){
    return  this->num_of_games_played;
}
int Player::getGoals(){
    return this->goals;
}
int Player::getCards(){
    return this->cards;
}
bool Player::getGoalKeeperStatus(){
    return this->goal_keeper_status;
}
Team* Player::getTeam(){
    return this->team;
}

Player* Player::getClosestLeft(){
    return this->closest_left;
}
Player* Player::getClosestRight(){
    return this->closest_right;
}

void  Player::setNumOfGamesPlayed(int num){
    this->num_of_games_played = num;
}
void  Player::setGoals(int goals){
    this->goals = goals;
}
void  Player::setCards(int cards){
    this->cards = cards;
}
void  Player::setGoalKeeperStatus(bool status){
    this->goal_keeper_status = status;
}
void  Player::setTeam(Team* team_to_set){
    this->team = team_to_set;
}
void Player::setClosestLeft(Player* player){
    this->closest_left = player;
}
void Player::setClosestRight(Player* player){
    this->closest_right = player;
}


/**
 *
 * @param player
 * @param player1
 * @param player2
 * @return 1
 *          if player 1 is closer from left to player
 *          OR
 *          1 if player 2 is closer from right to player
 * @return -1
 *          if player 2 is closer from left to player
 *          OR
 *          1 if player 1 is closer from right to player
 */


int keyComparisonForClosest(Player* player, Player* player1, Player* player2){
    if (player1->getGoals() > player2->getGoals()) return 1;
    else if (player1->getGoals() < player2->getGoals()) return -1;
    //sort_key1 is the same

    if (player1->getCards() < player2->getCards()) return 1;
    else if (player1->getCards() > player2->getCards()) return -1;

    if (player1->getId() > player2->getId()) return 1;
    else if (player1->getId() < player2->getId()) return -1;

//    if (absolute(player->getId() - player1->getId()) < absolute(player->getId() - player2->getId())) return 1;
//    else if (absolute(player->getId() - player1->getId()) > absolute(player->getId() - player2->getId())) return -1;

    return 0;
}

/**
 *
 * @param player
 * @param player1
 * @param player2
 * @return 1
 *          if player 1 is closer from to player
 * @return -1
 *          if player 2 is closer from to player
 */
int calculationOfClosest(Player* player, Player* player1, Player* player2){

    int diff_goal_1 = absolute(player->getGoals() - player1->getGoals());
    int diff_goal_2 = absolute(player->getGoals() - player2->getGoals());
    if (diff_goal_1 < diff_goal_2) return 1;
    else if (diff_goal_1 > diff_goal_2) return -1;

    int diff_card_1 = absolute(player->getCards() - player1->getCards());
    int diff_card_2 = absolute(player->getCards() - player2->getCards());
    if (diff_card_1 < diff_card_2) return 1;
    else if (diff_card_1 > diff_card_2) return -1;

    int diff_id_1 = absolute(player->getId() - player1->getId());
    int diff_id_2 = absolute(player->getId() - player2->getId());
    if (diff_id_1 < diff_id_2) return 1;
    else if (diff_id_1 > diff_id_2) return -1;

    if (player1->getId() > player2->getId()) return 1;
    return -1;
}

int absolute(int a){
    return (a<0)? (-a) : a;
}
