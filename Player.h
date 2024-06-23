//
// Created by Toam Elharar on 26/11/2022.
//

#ifndef WET1_PLAYER_H
#define WET1_PLAYER_H
#pragma once

//#include "Team.h"
class Team;


class Player {
    int id;
    Team* team;
    int num_of_games_played;
    int goals;
    int cards;
    bool goal_keeper_status;
    Player* closest_left;
    Player* closest_right;

public:
    Player();
    explicit Player(int id,Team* team, int num_of_games_played,int goals,int cards,bool goal_keeper_status,
                    Player* closest_left = nullptr,Player* closest_right = nullptr);
    virtual ~Player() = default;
    Player(const Player& player) = default;

    int getId();
    Team* getTeam();
    int getNumOfGamesPlayed();
    int getGoals();
    int getCards();
    bool getGoalKeeperStatus();
    Player* getClosestLeft();
    Player* getClosestRight();

    void setTeam(Team* team_to_set);
    void setNumOfGamesPlayed(int num);
    void setGoals(int goals);
    void setCards(int cards);
    void setGoalKeeperStatus(bool status);
    void setClosestLeft(Player* player);
    void setClosestRight(Player* player);
};

int keyComparisonForClosest(Player* player, Player* player1, Player* player2);
int calculationOfClosest(Player* player, Player* player1, Player* player2);
int absolute(int a);


#endif // WORLDCUP23A1_H_PLAYER_H
