
#ifndef WET1_TEAM_H
#define WET1_TEAM_H
#pragma once

#include "PlayersTree.h"


class Team {
private:
    int id;
    int points;
    int goal_keeper_number;
    PlayersTree* players_tree_by_id;
    PlayersTree* players_tree_by_goals;
    int goals_of_all_players_in_total;
    int cards_of_all_players_in_total;
    int num_of_games_played;
   // Player* top_scorer_player;

public:
    Team();
    explicit Team(int id, int points,int goal_keeper_number,PlayersTree* players_tree_by_id,
                  PlayersTree* players_tree_by_goals,int goals_of_all_players_in_total,int cards_of_all_players_in_total,
                  int num_of_games_played,Player* top_scorer_player);
    virtual ~Team() = default;
    //~Team();
    Team(const Team& team) = default;

    int getId();
    int getPoints();
    int getGoalKeeperNumber();
    PlayersTree* getPlayersTreeById();
    PlayersTree* getPlayersTreeByGoals();
    int getGoalsOfAllPlayersInTotal();
    int getCardsOfAllPlayersInTotal();
    int getNumOfGamesPlayed();
    //Player* getTopScorerPlayer();

    void setId(int id);
    void setPoints(int points);
    void setGoalKeeperNumber(int num);
    void setPlayersTreeById(PlayersTree* tree);
    void setPlayersTreeByGoals(PlayersTree* tree);
    void setGoalsOfAllPlayersInTotal(int total_goals);
    void setCardsOfAllPlayersInTotal(int total_cards);
    void setNumOfGamesPlayed(int num_of_games_played);
    //void setTopScorerPlayer(Player* top_scorer_player);
    returnOption removePlayer(int player_id);
    bool isValidToPlay();
    void insertPlayer(TreeNode<Player*>* new_player_by_id,TreeNode<Player*>* new_player_by_goals);
};

void getInorderArraySizeR(int* const arr1, int* const arr2, TreeNode<Team*>* node, int min_limit, int max_limit);
void auxInorderArraySizeR(int* const arr1, int* const arr2, TreeNode<Team*>* node,int* i,int min_limit, int max_limit);

#endif //WORLDCUP23A1_H_TEAM_H
