
#include "worldcup23a1.h"

world_cup_t::world_cup_t() {
    all_teams_by_id = new TeamsTree();
    all_legal_teams_by_id = new TeamsTree();
    all_players_by_id = new PlayersTree();
    all_players_by_goals = new PlayersTree();
}

world_cup_t::~world_cup_t(){

    all_legal_teams_by_id->removeAllNodesFromTree(all_legal_teams_by_id->getRoot());
    delete all_legal_teams_by_id;

    all_teams_by_id->removeAllNodesFromTree(all_teams_by_id->getRoot(),RELEASE_DATA);
    delete all_teams_by_id;

    all_players_by_id->removeAllNodesFromTree(all_players_by_id->getRoot());
    delete all_players_by_id;

    all_players_by_goals->removeAllNodesFromTree(all_players_by_goals->getRoot(), RELEASE_DATA);
    delete all_players_by_goals;

}

StatusType world_cup_t::add_team(int teamId, int points) {

    if(teamId<=0 || points <0) return StatusType::INVALID_INPUT;
    if (all_teams_by_id->findNode(teamId))  return StatusType::FAILURE;

    Team* new_team(new (std::nothrow) Team());
    if (new_team == nullptr) return StatusType::ALLOCATION_ERROR;
    new_team->setId(teamId);
    new_team->setPoints(points);

    TreeNode<Team*>* tree_node_by_id(new (std::nothrow) TreeNode<Team*>(new_team, teamId));
    if (tree_node_by_id == nullptr) {
        delete new_team;
        return StatusType::ALLOCATION_ERROR;
    }

    returnOption status1 = all_teams_by_id->insertNode(tree_node_by_id);
    if (status1 == ALREADY_EXIST) return StatusType::FAILURE;

    return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId) {
    if (teamId<=0) return StatusType::INVALID_INPUT;
    TreeNode<Team*>* node_team = all_teams_by_id->findNode(teamId);
    if (node_team == nullptr)
    {
        return StatusType::FAILURE;
    }

    Team* team = node_team->getData();
    PlayersTree* tree_of_players_in_team = team->getPlayersTreeById();
    if (tree_of_players_in_team->getRoot() != nullptr)
    {
        return StatusType::FAILURE;
    }

    TreeNode<Team*>* node_team_in_legal = all_legal_teams_by_id->findNode(teamId);
    if(node_team_in_legal){
        all_legal_teams_by_id->removeNode(node_team_in_legal);
    }
    delete team->getPlayersTreeById();
    delete team->getPlayersTreeByGoals();
    all_teams_by_id->removeNode(node_team, RELEASE_DATA);
	return StatusType::SUCCESS;
}


StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,
                                   int goals, int cards, bool goalKeeper) {

    returnOption status;
	if(playerId<=0||teamId<=0||gamesPlayed<0||goals<0||cards<0||(gamesPlayed==0 && (goals>0||cards>0))) return StatusType::INVALID_INPUT;

    TreeNode<Team*>* team_of_player = all_teams_by_id->findNode(teamId);
    if(team_of_player == nullptr) return StatusType::FAILURE;

    Team* team = team_of_player->getData();
    int played_games_of_team = team->getNumOfGamesPlayed();

    if(all_players_by_id->findNode(playerId)!= nullptr || team->getPlayersTreeById()->findNode(playerId)!=
                                                          nullptr) return StatusType::FAILURE;

    Player* new_player(new (std::nothrow) Player(playerId,team,(gamesPlayed-played_games_of_team),goals,cards,goalKeeper));
    if(new_player == nullptr) return StatusType::ALLOCATION_ERROR;

    TreeNode<Player*>* player_node_by_id_for_all_players(new (std::nothrow) TreeNode<Player*>(new_player,playerId));
    if(player_node_by_id_for_all_players == nullptr){
        delete new_player;
    }
    //the keys order is for the "get_all_players" and "get_top_scorer" functions
    TreeNode<Player*>* player_node_by_goals_for_all_players(new (std::nothrow) TreeNode<Player*>(new_player,goals,cards,playerId));
    if(player_node_by_goals_for_all_players == nullptr){
        delete player_node_by_id_for_all_players;
        delete new_player;
        return StatusType::ALLOCATION_ERROR;
    }

    TreeNode<Player*>* player_node_by_id_for_team_tree(new (std::nothrow) TreeNode<Player*>(new_player,playerId));
    if(player_node_by_id_for_team_tree == nullptr){
        delete player_node_by_id_for_all_players;
        delete player_node_by_goals_for_all_players;
        delete new_player;
        return StatusType::ALLOCATION_ERROR;
    }
    TreeNode<Player*>* player_node_by_goals_for_team_tree(new (std::nothrow) TreeNode<Player*>(new_player,goals,cards,playerId));
    if(player_node_by_goals_for_team_tree == nullptr){
        delete player_node_by_id_for_all_players;
        delete player_node_by_goals_for_all_players;
        delete player_node_by_id_for_team_tree;
        delete new_player;
        return StatusType::ALLOCATION_ERROR;
    }

    TreeNode<Team*>* node_for_legal_team_tree(new (std::nothrow) TreeNode<Team*>(team, team->getId()));
    if(node_for_legal_team_tree == nullptr){
        delete player_node_by_id_for_all_players;
        delete player_node_by_goals_for_all_players;
        delete player_node_by_id_for_team_tree;
        delete player_node_by_goals_for_team_tree;
        delete new_player;
        return StatusType::ALLOCATION_ERROR;
    }

    //add player to both big players trees
    status = all_players_by_id->insertNode(player_node_by_id_for_all_players);
    if(status == ALREADY_EXIST){
        delete node_for_legal_team_tree;
        delete player_node_by_id_for_all_players;
        delete player_node_by_goals_for_all_players;
        delete player_node_by_id_for_team_tree;
        delete player_node_by_goals_for_team_tree;
        delete new_player;
        return StatusType::FAILURE;
    }
    status = all_players_by_goals->insertNode(player_node_by_goals_for_all_players);
    if(status == ALREADY_EXIST) return StatusType::FAILURE;

    //add player and his stats to the team
    //team->insertPlayer(player_node_by_id,player_node_by_goals);

    team->getPlayersTreeById()->insertNode(player_node_by_id_for_team_tree);
    team->getPlayersTreeByGoals()->insertNode(player_node_by_goals_for_team_tree);

    team->setGoalsOfAllPlayersInTotal(team->getGoalsOfAllPlayersInTotal()+goals);
    team->setCardsOfAllPlayersInTotal(team->getCardsOfAllPlayersInTotal()+cards);

    //check if this player is the top scorer player in team or in total players tree
//    Player* curr_team_top_scorer =  team->getPlayersTreeById()->getTopScorerPlayer();
//    if(curr_team_top_scorer == nullptr || curr_team_top_scorer->getGoals() < new_player->getGoals() ||
//    (curr_team_top_scorer->getGoals()==new_player->getGoals() && curr_team_top_scorer->getCards() > new_player->getCards())
//    || (curr_team_top_scorer->getGoals()==new_player->getGoals() && curr_team_top_scorer->getCards()==new_player->getCards() && curr_team_top_scorer->getId()<playerId)){
//        team->getPlayersTreeById()->setTopScorerPlayer(new_player);
//        team->getPlayersTreeByGoals()->setTopScorerPlayer(new_player);
//    }
//
//    Player* top_scorer_in_total = all_players_by_id->getTopScorerPlayer();
//    if(top_scorer_in_total == nullptr || top_scorer_in_total->getGoals() < new_player->getGoals() ||
//    (top_scorer_in_total->getGoals()==new_player->getGoals() && top_scorer_in_total->getCards() > new_player->getCards())
//    || (top_scorer_in_total->getGoals()==new_player->getGoals() && top_scorer_in_total->getCards()==new_player->getCards() && top_scorer_in_total->getId()<playerId)){
//        all_players_by_id->setTopScorerPlayer(new_player);
//        all_players_by_goals->setTopScorerPlayer(new_player);
//    }


    //set closest right to the added player:
    int player_closest_right_id;
    TreeNode<Player*>* curr_node = all_players_by_goals->getRoot();
    while(curr_node->getRight()){
        curr_node = curr_node->getRight();
    }
    if(curr_node->getData()->getId() == playerId){ //this is the smallest node of all player
        player_closest_right_id = 0;
    }
    else player_closest_right_id = closestToTheRight(player_node_by_goals_for_all_players);
    TreeNode<Player*>* closest_to_right_in_id = all_players_by_id->findNode(player_closest_right_id);

    if(closest_to_right_in_id && closest_to_right_in_id->getData()) {
        TreeNode<Player *> temp;
        temp.setSortKey1(closest_to_right_in_id->getData()->getGoals());
        temp.setSortKey2(closest_to_right_in_id->getData()->getCards());
        temp.setSortKey3(closest_to_right_in_id->getData()->getId());
        TreeNode<Player*>* player_closest_right = all_players_by_goals->findNode(&temp);
        new_player->setClosestRight(player_closest_right->getData());
    }
    else new_player->setClosestRight(nullptr);


    //set closest left to the added player:
    curr_node = all_players_by_goals->getRoot();
    int player_closest_left_id;
    while(curr_node->getLeft()){
        curr_node = curr_node->getLeft();
    }
    if(curr_node->getData()->getId() == playerId){ //this is the smallest node of all player
        player_closest_left_id = 0;
    }
    else player_closest_left_id = closestToTheLeft(player_node_by_goals_for_all_players);

    TreeNode<Player*>* closest_to_left_in_id = all_players_by_id->findNode(player_closest_left_id);
    if(closest_to_left_in_id && closest_to_left_in_id->getData()) {
        TreeNode<Player *> temp2;
        temp2.setSortKey1(closest_to_left_in_id->getData()->getGoals());
        temp2.setSortKey2(closest_to_left_in_id->getData()->getCards());
        temp2.setSortKey3(closest_to_left_in_id->getData()->getId());

        TreeNode<Player *> *player_closest_left = all_players_by_goals->findNode(&temp2);
        new_player->setClosestLeft(player_closest_left->getData());
    }
    else new_player->setClosestLeft(nullptr);


    Player* closest_right = new_player->getClosestRight();
    if (closest_right) closest_right->setClosestLeft(new_player);
    Player* closest_left = new_player->getClosestLeft();
    if (closest_left) closest_left->setClosestRight(new_player);

    //check if player can be goalkeeper and update in team status
    if(goalKeeper){
        team->setGoalKeeperNumber(team->getGoalKeeperNumber()+1);
    }
    //insert to legal_teams_tree if legal
    bool is_team_already_legal = false;
    if(all_legal_teams_by_id->findNode(teamId)!= nullptr) is_team_already_legal = true;
    if(team->getPlayersTreeById()->getNumOfNode()>=11 && team->getGoalKeeperNumber()>0 && !is_team_already_legal){
        all_legal_teams_by_id->insertNode(node_for_legal_team_tree);
    }
    else delete node_for_legal_team_tree;

    TreeNode<Player*>* curr_team_top_scorer_node = team->getPlayersTreeByGoals()->getRoot();
    while (curr_team_top_scorer_node->getRight()) {
        curr_team_top_scorer_node = curr_team_top_scorer_node->getRight();
    }
    team->getPlayersTreeById()->setTopScorerPlayer(curr_team_top_scorer_node->getData());
    team->getPlayersTreeByGoals()->setTopScorerPlayer(curr_team_top_scorer_node->getData());

    TreeNode<Player*>* top_scorer_in_total = all_players_by_goals->getRoot();
    while (top_scorer_in_total->getRight()) {
        top_scorer_in_total = top_scorer_in_total->getRight();
    }
    all_players_by_goals->setTopScorerPlayer(top_scorer_in_total->getData());
    all_players_by_id->setTopScorerPlayer(top_scorer_in_total->getData());

	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_player(int playerId) {
    if(playerId <= 0) return StatusType::INVALID_INPUT;

    TreeNode<Player*>* player_to_remove_by_id_from_all = all_players_by_id->findNode(playerId);
    if(player_to_remove_by_id_from_all == nullptr) return StatusType::FAILURE;


    Player* player_to_remove = player_to_remove_by_id_from_all->getData();

    TreeNode<Player*> temp1;
    temp1.setSortKey1(player_to_remove->getGoals());
    temp1.setSortKey2(player_to_remove->getCards());
    temp1.setSortKey3(playerId);

    TreeNode<Player*>* player_to_remove_by_goal_from_all = all_players_by_goals->findNode(&temp1);

    Team* team_of_player = player_to_remove_by_id_from_all->getData()->getTeam();

    TreeNode<Player*>* player_to_remove_by_id_for_team_tree = (team_of_player->getPlayersTreeById())->findNode(playerId);

    TreeNode<Player*>* player_to_remove_by_goal_for_team_tree = (team_of_player->getPlayersTreeByGoals())->findNode(&temp1);

    Player* closest_right = player_to_remove->getClosestRight();
    Player* closest_left = player_to_remove->getClosestLeft();

    int closest_right_goals = 0, closest_right_id = 0, closest_right_cards = 0;
    if (closest_right) {
        closest_right_goals = closest_right->getGoals();
        closest_right_id = closest_right->getId();
        closest_right_cards = closest_right->getCards();
        closest_right->setClosestLeft(nullptr);
    }

    int closest_left_id = 0, closest_left_goals = 0, closest_left_cards = 0;
    if (closest_left) {
        closest_left_id = closest_left->getId();
        closest_left_goals = closest_left->getGoals();
        closest_left_cards = closest_left->getCards();
        closest_left->setClosestRight(nullptr);
    }

    bool is_top_in_all_players = false;
    bool is_top_in_team = false;
    if(all_players_by_id->getTopScorerPlayer()->getId()==playerId) is_top_in_all_players=true;
    if(team_of_player->getPlayersTreeById()->getTopScorerPlayer()->getId()==playerId) is_top_in_team=true;

    returnOption status_for_id = (team_of_player->getPlayersTreeById())->removeNode(player_to_remove_by_id_for_team_tree);
    returnOption status_for_goals = (team_of_player->getPlayersTreeByGoals())->removeNode(player_to_remove_by_goal_for_team_tree);

    if(player_to_remove->getGoalKeeperStatus()) team_of_player->setGoalKeeperNumber(team_of_player->getGoalKeeperNumber()-1);
    bool team_was_legal = (all_legal_teams_by_id->findNode(team_of_player->getId())!= nullptr);
    if(team_was_legal){
        if(team_of_player->getPlayersTreeById()->getNumOfNode()<11 || team_of_player->getGoalKeeperNumber() == 0 ){
            TreeNode<Team*>* node_to_remove = all_legal_teams_by_id->findNode(team_of_player->getId());
            all_legal_teams_by_id->removeNode(node_to_remove);
        }
    }


    if(status_for_goals!=NOT_VALID && status_for_id!=NOT_VALID){
        //want to change it only if the player was not already removed from the team
        team_of_player->setGoalsOfAllPlayersInTotal(team_of_player->getGoalsOfAllPlayersInTotal() - player_to_remove_by_id_from_all->getData()->getGoals());
        team_of_player->setCardsOfAllPlayersInTotal(team_of_player->getCardsOfAllPlayersInTotal() - player_to_remove_by_id_from_all->getData()->getCards());
    }

    all_players_by_id->removeNode(player_to_remove_by_id_from_all);
    all_players_by_goals->removeNode(player_to_remove_by_goal_from_all, RELEASE_DATA);

    if(is_top_in_all_players){
        TreeNode<Player*>* curr_node = all_players_by_goals->getRoot();
        if(curr_node== nullptr) {
            all_players_by_id->setTopScorerPlayer(nullptr);
            all_players_by_goals->setTopScorerPlayer(nullptr);
        }
        else {
            while (curr_node->getRight()) {
                curr_node = curr_node->getRight();
            }
            all_players_by_goals->setTopScorerPlayer(curr_node->getData());
            all_players_by_id->setTopScorerPlayer(curr_node->getData());
        }
    }
    if(is_top_in_team) {
        TreeNode<Player *> *curr_node = team_of_player->getPlayersTreeByGoals()->getRoot();
        if (curr_node == nullptr) {
            team_of_player->getPlayersTreeByGoals()->setTopScorerPlayer(nullptr);
            team_of_player->getPlayersTreeById()->setTopScorerPlayer(nullptr);
        } else {
            while (curr_node->getRight()) {
                curr_node = curr_node->getRight();
            }
            team_of_player->getPlayersTreeByGoals()->setTopScorerPlayer(curr_node->getData());
            team_of_player->getPlayersTreeById()->setTopScorerPlayer(curr_node->getData());
        }
    }


    TreeNode<Player *> temp;
    TreeNode<Player *> *curr_node = all_players_by_goals->getRoot();
    if (closest_left) {
        temp.setSortKey1(closest_left_goals);
        temp.setSortKey2(closest_left_cards);
        temp.setSortKey3(closest_left_id);
        TreeNode<Player *> *player_to_remove_closest_left_node = all_players_by_goals->findNode(&temp);

        int player_closest_right_id;
        while (curr_node->getRight()) {
            curr_node = curr_node->getRight();
        }
        if (curr_node->getData()->getId() == closest_left_id) { //this is the biggest node of all player
            player_closest_right_id = 0;
        } else player_closest_right_id = closestToTheRight(player_to_remove_closest_left_node);

        TreeNode<Player *> *closest_to_right_in_id = all_players_by_id->findNode(player_closest_right_id);

        if (closest_to_right_in_id && closest_to_right_in_id->getData()) {
            temp.setSortKey1(closest_to_right_in_id->getData()->getGoals());
            temp.setSortKey2(closest_to_right_in_id->getData()->getCards());
            temp.setSortKey3(closest_to_right_in_id->getData()->getId());
            TreeNode<Player *> *player_closest_right = all_players_by_goals->findNode(&temp);
            closest_left->setClosestRight(player_closest_right->getData());
        } else closest_left->setClosestRight(nullptr);
    }


    //set closest left to the added player:
if (closest_right) {
    temp.setSortKey1(closest_right_goals);
    temp.setSortKey2(closest_right_cards);
    temp.setSortKey3(closest_right_id);


    TreeNode<Player *> *player_to_remove_closest_right_node = all_players_by_goals->findNode(&temp);
    curr_node = all_players_by_goals->getRoot();
    int player_closest_left_id;
    while (curr_node->getLeft()) {
        curr_node = curr_node->getLeft();
    }
    if (curr_node->getData()->getId() == closest_right_id) { //this is the smallest node of all player
        player_closest_left_id = 0;
    } else player_closest_left_id = closestToTheLeft(player_to_remove_closest_right_node);

    TreeNode<Player *> *closest_to_left_in_id = all_players_by_id->findNode(player_closest_left_id);
    if (closest_to_left_in_id && closest_to_left_in_id->getData()) {
        TreeNode<Player *> temp2;
        temp2.setSortKey1(closest_to_left_in_id->getData()->getGoals());
        temp2.setSortKey2(closest_to_left_in_id->getData()->getCards());
        temp2.setSortKey3(closest_to_left_in_id->getData()->getId());

        TreeNode<Player *> *player_closest_left = all_players_by_goals->findNode(&temp2);
        closest_right->setClosestLeft(player_closest_left->getData());
    } else closest_right->setClosestLeft(nullptr);
}






    return StatusType::SUCCESS;
}

StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed, int scoredGoals, int cardsReceived){

    if(playerId <= 0 || gamesPlayed < 0 || scoredGoals < 0 || cardsReceived < 0) return StatusType::INVALID_INPUT;

    TreeNode<Player*>* player_to_update = all_players_by_id->findNode(playerId);
    if(player_to_update == nullptr) return StatusType::FAILURE;
    Player* player = player_to_update->getData();
    if(!player) return StatusType::ALLOCATION_ERROR;

    TreeNode<Player*> temp;
    temp.setSortKey1(player->getGoals());
    temp.setSortKey2(player->getCards());
    temp.setSortKey3(player->getId());

    player->setNumOfGamesPlayed((player->getNumOfGamesPlayed())+gamesPlayed);
    player->setGoals((player->getGoals())+scoredGoals);
    player->setCards((player->getCards())+cardsReceived);
    Team* team_of_player = player->getTeam();

    TreeNode<Player*>* node_to_remove_for_tree_all_players = all_players_by_goals->findNode(&temp);
    TreeNode<Player*>* node_to_remove_for_team_tree = team_of_player->getPlayersTreeByGoals()->findNode(&temp);

    TreeNode<Player*>* new_node_for_tree_all_players(new TreeNode<Player*>(player,player->getGoals(),player->getCards(),playerId));
    if(new_node_for_tree_all_players== nullptr) return StatusType::ALLOCATION_ERROR;
    TreeNode<Player*>* new_node_for_team_tree(new TreeNode<Player*>(player,player->getGoals(),player->getCards(),playerId));
    if(new_node_for_team_tree== nullptr){
        delete new_node_for_tree_all_players;
        return StatusType::ALLOCATION_ERROR;
    }

    all_players_by_goals->removeNode(node_to_remove_for_tree_all_players);
    all_players_by_goals->insertNode(new_node_for_tree_all_players);
    team_of_player->getPlayersTreeByGoals()->removeNode(node_to_remove_for_team_tree);
    team_of_player->getPlayersTreeByGoals()->insertNode(new_node_for_team_tree);
    player = new_node_for_team_tree->getData();

    //check if this player is the top scorer player in team or in total players tree
//    Player* curr_team_top_scorer =  team_of_player->getPlayersTreeById()->getTopScorerPlayer();
//    int curr_top_goals = curr_team_top_scorer->getGoals();
//    int curr_top_cards = curr_team_top_scorer->getCards();
//    int curr_top_id = curr_team_top_scorer->getId();
//    int player_goals = player->getGoals();
//    int player_cards = player->getCards();
//    if (curr_team_top_scorer == nullptr || curr_top_goals < player_goals ||
//           (curr_top_goals ==player_goals && curr_top_cards > player_cards) ||(curr_top_goals == player_goals &&
//           curr_top_cards == player_cards && curr_top_id < playerId)){
//        team_of_player->getPlayersTreeById()->setTopScorerPlayer(player);
//        team_of_player->getPlayersTreeByGoals()->setTopScorerPlayer(player);
//    }
//    }


    TreeNode<Player*>* curr_team_top_scorer_node = team_of_player->getPlayersTreeByGoals()->getRoot();
    while (curr_team_top_scorer_node->getRight()) {
        curr_team_top_scorer_node = curr_team_top_scorer_node->getRight();
    }
    team_of_player->getPlayersTreeById()->setTopScorerPlayer(curr_team_top_scorer_node->getData());
    team_of_player->getPlayersTreeByGoals()->setTopScorerPlayer(curr_team_top_scorer_node->getData());

    TreeNode<Player*>* top_scorer_in_total = all_players_by_goals->getRoot();
    while (top_scorer_in_total->getRight()) {
        top_scorer_in_total = top_scorer_in_total->getRight();
    }
    all_players_by_goals->setTopScorerPlayer(top_scorer_in_total->getData());
    all_players_by_id->setTopScorerPlayer(top_scorer_in_total->getData());
//    Player* top_scorer_in_total = all_players_by_id->getTopScorerPlayer();
//
//    if(top_scorer_in_total == nullptr || top_scorer_in_total->getGoals() < player->getGoals() ||
//       (top_scorer_in_total->getGoals()==player->getGoals() && top_scorer_in_total->getCards() > player->getCards()) ||
//            (top_scorer_in_total->getGoals()==player->getGoals() && top_scorer_in_total->getCards()==player->getCards() && top_scorer_in_total->getId()<playerId)){
//        all_players_by_id->setTopScorerPlayer(player);
//        all_players_by_goals->setTopScorerPlayer(player);
//    }


    Player* original_closest_left = player->getClosestLeft();
    Player* original_closest_right = player->getClosestRight();


    int player_closest_right_id;
    TreeNode<Player*>* curr_node = all_players_by_goals->getRoot();
    while(curr_node->getRight()){
        curr_node = curr_node->getRight();
    }
    if(curr_node->getData()->getId() == playerId){ //this is the smallest node of all player
        player_closest_right_id = 0;
    }
    else player_closest_right_id = closestToTheRight(new_node_for_tree_all_players);
    //int player_closest_right_id = closestToTheRight(new_node_for_tree_all_players);
    TreeNode<Player*>* closest_to_right_in_id = all_players_by_id->findNode(player_closest_right_id);

    if(closest_to_right_in_id && closest_to_right_in_id->getData()) {
        TreeNode<Player *> temp1;
        temp1.setSortKey1(closest_to_right_in_id->getData()->getGoals());
        temp1.setSortKey2(closest_to_right_in_id->getData()->getCards());
        temp1.setSortKey3(closest_to_right_in_id->getData()->getId());

        TreeNode<Player *> *player_closest_right = all_players_by_goals->findNode(&temp1);
        player->setClosestRight(player_closest_right->getData());
    }
    else player->setClosestRight(nullptr);


    curr_node = all_players_by_goals->getRoot();
    int player_closest_left_id;
    while(curr_node->getLeft()){
        curr_node = curr_node->getLeft();
    }
    if(curr_node->getData()->getId() == playerId){ //this is the smallest node of all player
        player_closest_left_id = 0;
    }
    else player_closest_left_id = closestToTheLeft(new_node_for_tree_all_players);
    //int player_closest_left_id = closestToTheLeft(new_node_for_tree_all_players);
    TreeNode<Player*>* closest_to_left_in_id = all_players_by_id->findNode(player_closest_left_id);

    if(closest_to_left_in_id && closest_to_left_in_id->getData()) {
        TreeNode<Player *> temp2;
        temp2.setSortKey1(closest_to_left_in_id->getData()->getGoals());
        temp2.setSortKey2(closest_to_left_in_id->getData()->getCards());
        temp2.setSortKey3(closest_to_left_in_id->getData()->getId());

        TreeNode<Player *> *player_closest_left = all_players_by_goals->findNode(&temp2);
        player->setClosestLeft(player_closest_left->getData());
    }
    else player->setClosestLeft(nullptr);

    Player* closest_right = player->getClosestRight();
    if (closest_right) closest_right->setClosestLeft(player);
    Player* closest_left = player->getClosestLeft();
    if (closest_left) closest_left->setClosestRight(player);

    if (original_closest_right != closest_right && original_closest_left != closest_left){
        if(original_closest_right) original_closest_right->setClosestLeft(original_closest_left);
        if(original_closest_left) original_closest_left->setClosestRight(original_closest_right);
    }

    team_of_player->setGoalsOfAllPlayersInTotal(team_of_player->getGoalsOfAllPlayersInTotal()+scoredGoals);
    team_of_player->setCardsOfAllPlayersInTotal(team_of_player->getCardsOfAllPlayersInTotal()+cardsReceived);

    return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2) {
    if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) return StatusType::INVALID_INPUT;

    TreeNode<Team*>* team1_node = all_legal_teams_by_id->findNode(teamId1);
    TreeNode<Team*>* team2_node = all_legal_teams_by_id->findNode(teamId2);
	if (!(team1_node && team2_node)) return StatusType::FAILURE;

    Team* team1 = team1_node->getData();
    Team* team2 = team2_node->getData();
    //if (!(team1->isValidToPlay() && team2->isValidToPlay())) return StatusType::FAILURE;

    int score_team1 = team1->getPoints() + team1->getGoalsOfAllPlayersInTotal() - team1->getCardsOfAllPlayersInTotal();
    int score_team2 = team2->getPoints() + team2->getGoalsOfAllPlayersInTotal() - team2->getCardsOfAllPlayersInTotal();

    if (score_team1 > score_team2) team1->setPoints(team1->getPoints() + 3);
    else if (score_team1 < score_team2) team2->setPoints(team2->getPoints() + 3);
    else{
        team1->setPoints(team1->getPoints() + 1);
        team2->setPoints(team2->getPoints() + 1);
    }

    team1->setNumOfGamesPlayed(team1->getNumOfGamesPlayed() + 1);
    team2->setNumOfGamesPlayed(team2->getNumOfGamesPlayed() + 1);

	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId){
    if (playerId <= 0) return StatusType::INVALID_INPUT;

    TreeNode<Player*>* player_node = all_players_by_id->findNode(playerId);
    if(player_node == nullptr) return StatusType::FAILURE;

    Player* player = player_node->getData();
    Team* player_team = player->getTeam();
	int num_played_games = player->getNumOfGamesPlayed() + player_team->getNumOfGamesPlayed();

	return output_t<int>(num_played_games);
}

output_t<int> world_cup_t::get_team_points(int teamId) {
    if (teamId <= 0) return StatusType::INVALID_INPUT;

    TreeNode<Team*>* team_node = all_teams_by_id->findNode(teamId);
    if(team_node == nullptr) return StatusType::FAILURE;

    Team* team1 = team_node->getData();
	return output_t<int>(team1->getPoints());
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId){
	if (teamId1 <= 0 || teamId2 <= 0 || newTeamId <= 0 || teamId1 == teamId2) return StatusType::INVALID_INPUT;
    TreeNode<Team*>* team1_node_by_id = all_teams_by_id->findNode(teamId1);
    TreeNode<Team*>* team2_node_by_id = all_teams_by_id->findNode(teamId2);
    if(team1_node_by_id == nullptr || team2_node_by_id == nullptr) return StatusType::FAILURE;

    if (newTeamId != teamId1 && newTeamId != teamId2){
        if (all_teams_by_id->findNode(newTeamId)) return StatusType::FAILURE;
    }

    bool is_team1_legal = false,is_team2_legal = false;
    Team* team1 = team1_node_by_id->getData();
    Team* team2 = team2_node_by_id->getData();
    if(team1->getGoalKeeperNumber()>0 && team1->getPlayersTreeById()->getNumOfNode() >=11)
    {
        is_team1_legal = true;
    }
    if(team2->getGoalKeeperNumber()>0 && team2->getPlayersTreeById()->getNumOfNode() >=11)
    {
        is_team2_legal = true;
    }

    PlayersTree* team1_players_tree_by_id = team1->getPlayersTreeById();
    PlayersTree* team2_players_tree_by_id = team2->getPlayersTreeById();
    PlayersTree* team1_players_tree_by_goals = team1->getPlayersTreeByGoals();
    PlayersTree* team2_players_tree_by_goals = team2->getPlayersTreeByGoals();

    int new_goal_keeper_number = team1->getGoalKeeperNumber() + team2->getGoalKeeperNumber();
    Player* new_top_scorer_player;
    
//    Player* player1 =  team1->getPlayersTreeById()->getTopScorerPlayer();
//    Player* player2 =  team1->getPlayersTreeById()->getTopScorerPlayer();
//
//    if (player1->getGoals() < player2->getGoals()) new_top_scorer_player = player1;
//    else if (player1->getGoals() > player2->getGoals()) new_top_scorer_player = player2;
//    else{
//        if (player1->getCards() < player2->getCards()) new_top_scorer_player = player1;
//        else if (player1->getCards() > player2->getCards()) new_top_scorer_player = player2;
//        else{
//            if (player1->getId() < player2->getId()) new_top_scorer_player = player1;
//            else new_top_scorer_player = player2;
//        }
//    }

    PlayersTree* new_team_players_id_tree(new PlayersTree());
    if(new_team_players_id_tree== nullptr) return StatusType::ALLOCATION_ERROR;
    PlayersTree* new_team_players_goals_tree(new PlayersTree());
    if(new_team_players_goals_tree== nullptr) {
        delete new_team_players_id_tree;
        return StatusType::ALLOCATION_ERROR;
    }

    int num_of_plays_team1 = team1->getNumOfGamesPlayed();
    addAllPlayersGames(team1_players_tree_by_id->getRoot(),num_of_plays_team1);
    int num_of_plays_team2 = team2->getNumOfGamesPlayed();
    addAllPlayersGames(team2_players_tree_by_id->getRoot(),num_of_plays_team2);


    unite_trees(team1_players_tree_by_id, team2_players_tree_by_id, new_team_players_id_tree);
    unite_trees(team1_players_tree_by_goals, team2_players_tree_by_goals, new_team_players_goals_tree);
//    new_team_players_id_tree->setNumOfNode(original_num_of_nodes_team1_players_id + original_num_of_nodes_team2_players_id);
//    new_team_players_goals_tree->setNumOfNode(original_num_of_nodes_team1_players_goals + original_num_of_nodes_team2_players_goals);

    if (new_team_players_goals_tree->getRoot()){//
        TreeNode<Player*>* curr_team_top_scorer_node = new_team_players_goals_tree->getRoot();
        while (curr_team_top_scorer_node->getRight()) {
            curr_team_top_scorer_node = curr_team_top_scorer_node->getRight();
        }
        new_team_players_id_tree->setTopScorerPlayer(curr_team_top_scorer_node->getData());
        new_team_players_goals_tree->setTopScorerPlayer(curr_team_top_scorer_node->getData());

        new_top_scorer_player = curr_team_top_scorer_node->getData();
    }//

    int new_points = team1->getPoints() + team2->getPoints();
    int new_goals_of_all_players =team1->getGoalsOfAllPlayersInTotal() + team2->getGoalsOfAllPlayersInTotal();
    int new_cards_of_all_players = team1->getCardsOfAllPlayersInTotal() + team2->getCardsOfAllPlayersInTotal();

    if(is_team1_legal){
        TreeNode<Team*>* team1_node_in_legal = all_legal_teams_by_id->findNode(teamId1);
        all_legal_teams_by_id->removeNode(team1_node_in_legal);
    }
    if(is_team2_legal){
        TreeNode<Team*>* team2_node_in_legal = all_legal_teams_by_id->findNode(teamId2);
        all_legal_teams_by_id->removeNode(team2_node_in_legal);
    }
    all_teams_by_id->removeNode(team1_node_by_id, RELEASE_DATA);
    all_teams_by_id->removeNode(team2_node_by_id,RELEASE_DATA);

    Team* new_team(new Team(newTeamId, new_points, new_goal_keeper_number,
                            new_team_players_id_tree,new_team_players_goals_tree,
                            new_goals_of_all_players,
                            new_cards_of_all_players,
                            0, new_top_scorer_player));
    if (new_team == nullptr){
        return StatusType::ALLOCATION_ERROR;
    }

    int size_arr = new_team_players_id_tree->getNumOfNode();
    TreeNode<Player*>** arr = new TreeNode<Player*>*[size_arr];
    if(arr== nullptr) return StatusType::ALLOCATION_ERROR;
    getInorderArray(arr, new_team_players_id_tree->getRoot());

    for (int i = 0; i < size_arr; i++){
        TreeNode<Player*>* curr_node = arr[i];
        Player* player = curr_node->getData();
        player->setTeam(new_team);
    }

    TreeNode<Team*>* new_team_id_node(new TreeNode<Team*>(new_team, new_team->getId()));
    if (new_team_id_node == nullptr){
        delete new_team;
        return StatusType::ALLOCATION_ERROR;
    }

    if(new_team->getPlayersTreeById()->getNumOfNode()>=11 && new_team->getGoalKeeperNumber()>0){
        TreeNode<Team*>* node_for_legal_team_tree(new TreeNode<Team*>(new_team, new_team->getId()));
        if(node_for_legal_team_tree == nullptr){
            delete new_team_id_node;
            delete new_team;
            return StatusType::ALLOCATION_ERROR;
        }
        all_legal_teams_by_id->insertNode(node_for_legal_team_tree);
    }

    all_teams_by_id->insertNode(new_team_id_node);

    delete[] arr;
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_top_scorer(int teamId) {

    if(teamId==0) return StatusType::INVALID_INPUT;
    int top_scorer_id;
    if(teamId>0){
       TreeNode<Team*>* curr_team = all_teams_by_id->findNode(teamId);
       if(curr_team == nullptr || curr_team->getData()->getPlayersTreeById()->getNumOfNode() == 0) return StatusType::FAILURE;//
       top_scorer_id = curr_team->getData()->getPlayersTreeById()->getTopScorerPlayer()->getId();
    }
    else{
        if(all_players_by_id->getNumOfNode()==0) return StatusType::FAILURE;
        top_scorer_id = all_players_by_id->getTopScorerPlayer()->getId();
    }
    return output_t<int>(top_scorer_id);
}

output_t<int> world_cup_t::get_all_players_count(int teamId) {

    if (teamId == 0) return StatusType::INVALID_INPUT;
    if (teamId > 0){
        TreeNode<Team*>* team_node = all_teams_by_id->findNode(teamId);
        if (!team_node) return StatusType::FAILURE;
        int num_of_players_in_team = team_node->getData()->getPlayersTreeById()->getNumOfNode();
        return output_t<int>(num_of_players_in_team);
    }
    int num_of_all_players = all_players_by_id->getNumOfNode();
    return output_t<int>(num_of_all_players);
}

StatusType world_cup_t::get_all_players(int teamId, int *const output){
	if(teamId == 0 || output == nullptr) return StatusType::INVALID_INPUT;
    if(teamId > 0){
        TreeNode<Team*>* curr_team = all_teams_by_id->findNode(teamId);
        if(curr_team == nullptr) return StatusType::FAILURE;
        PlayersTree* players_of_team_by_goals = curr_team->getData()->getPlayersTreeByGoals();
        if(players_of_team_by_goals->getNumOfNode()==0) return StatusType::FAILURE;
        getInorderArray(output,players_of_team_by_goals->getRoot());
    }
    else{
        if(all_players_by_goals->getNumOfNode()==0) return StatusType::FAILURE;
        getInorderArray(output,all_players_by_goals->getRoot());
    }
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId){

    if(playerId <= 0 || teamId <= 0) return StatusType::INVALID_INPUT;
    TreeNode<Team*>* team_node = all_teams_by_id->findNode(teamId);
    if(team_node== nullptr) return StatusType::FAILURE;
    PlayersTree* players_tree_goals = team_node->getData()->getPlayersTreeByGoals();

    //Help search to find the values of the player
    PlayersTree* players_tree_id = team_node->getData()->getPlayersTreeById();
    TreeNode<Player*>* player_node_id = players_tree_id->findNode(playerId);
    if(player_node_id == nullptr) return StatusType::FAILURE;

    TreeNode<Player*> temp;
    temp.setSortKey1(player_node_id->getData()->getGoals());
    temp.setSortKey2(player_node_id->getData()->getCards());
    temp.setSortKey3(player_node_id->getData()->getId());

    TreeNode<Player*>* player_node = players_tree_goals->findNode(&temp);
    if(player_node== nullptr) return StatusType::FAILURE;
    if(all_players_by_id->getNumOfNode()==1 && all_players_by_id->getRoot()==player_node) return StatusType::FAILURE;
    Player* player = player_node->getData();

    Player* closest_left = player->getClosestLeft();
    Player* closest_right = player->getClosestRight();
    if(closest_left == nullptr && closest_right == nullptr) {
        return StatusType::FAILURE;
    }
    if(closest_left == nullptr) return output_t<int>(closest_right->getId());
    if(closest_right== nullptr) return output_t<int>(closest_left->getId());

    int res = calculationOfClosest(player, closest_left, closest_right);

    int left_id = closest_left->getId();
    int right_id = closest_right->getId();
    if (res == 1) return output_t<int>(left_id);
    else if (res == -1) return output_t<int>(right_id);

    if (left_id > right_id) return output_t<int>(left_id);

    return output_t<int>(right_id);
}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId) {
    if(minTeamId<0 || maxTeamId<0 || maxTeamId<minTeamId) return StatusType::INVALID_INPUT;
    if (!(all_legal_teams_by_id->getRoot())) return StatusType::FAILURE;//
    TreeNode<Team*>* max_team = all_legal_teams_by_id->getRoot();
    while(max_team->getRight()){
        max_team = max_team->getRight();
    }
    if (max_team->getSortKey1() < maxTeamId) maxTeamId = max_team->getSortKey1();

    TreeNode<Team*>* min_team = all_legal_teams_by_id->getRoot();
    while(min_team->getLeft()){
        min_team = min_team->getLeft();
    }
    if (min_team->getSortKey1() > minTeamId) minTeamId = min_team->getSortKey1();


    int winner;
    int r = getSizeOfInorderArray(all_legal_teams_by_id->getRoot(),minTeamId,maxTeamId);
    if(r == 0) return StatusType::FAILURE;

    int* teams_id_array = new int[r];
    if(teams_id_array== nullptr) return StatusType::ALLOCATION_ERROR;
    for(int i=0;i<r;i++) teams_id_array[i] = 0;

    int* teams_score_array = new int[r];
    if(teams_score_array == nullptr) {
        delete[] teams_id_array;
        return StatusType::ALLOCATION_ERROR;
    }
    for(int i=0;i<r;i++) teams_score_array[i] = 0;

    //complexity O(r):
    getInorderArraySizeR(teams_id_array,teams_score_array,all_legal_teams_by_id->getRoot(),minTeamId, maxTeamId);
    if(r == 1){
        winner = teams_id_array[0];
        delete[] teams_id_array;
        delete[] teams_score_array;
        return output_t<int>(winner);
    }

    int jumps = 1;
    while(jumps<r){//complexity O(log(r)+r) = O(r) worst case
        for(int i=0;i+jumps<r;i+=2*jumps){
            if(teams_score_array[i]>teams_score_array[i+jumps] || (teams_score_array[i]==teams_score_array[i+jumps] && teams_id_array[i]>teams_id_array[i+jumps])){
                teams_score_array[i]+=3;
                teams_score_array[i]+=teams_score_array[i+jumps];
            }
            else if(teams_score_array[i] < teams_score_array[i+jumps]|| (teams_score_array[i]==teams_score_array[i+jumps] && teams_id_array[i+jumps]>teams_id_array[i])){
                teams_score_array[i]+=3;
                teams_score_array[i]+=teams_score_array[i+jumps];
                teams_id_array[i] = teams_id_array[i+jumps];
             }
        }
        jumps*=2;
    }
    winner = teams_id_array[0];
    delete[] teams_id_array;
    delete[] teams_score_array;
    return output_t<int>(winner);
}



