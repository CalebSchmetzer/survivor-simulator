#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <map>


// Gets users permission to proceed to next event
void proceedPrompt() {
	
	std::cout << '\n';
	std::cout << "\nEnter any key to continue.";

	char temp;
	std::cin >> temp;

	std::cout << '\n';
	std::cout << '\n';
}


// Selects the # of tribes and # of players per tribe
int gameFormatSelection() {
	std::cout << "Which format would you like your season to have?\n";
	std::cout << "1. Two tribes of 8 players (16 total)\n";
	std::cout << "2. Three tribes of 6 players (18 total)\n";
	std::cout << "3. Two tribes of 10 players (20 total)\n";

	int selection{};

	std::cin >> selection;
	std::cout << '\n';

	return selection;
}


// Random prefix and suffix put together for each tribe (Based off the new era four letter tribe names)
std::string tribeNameGeneration() {
	std::string prefixes[]{ "Co","Sa","Se","La","Le","Mo","Tu","Ya","So","Ul" };
	std::string suffixes[]{ "ko","le","co","se","vo","nu","ga","ro","va","to" };

	int prefixSelection = std::rand() % 10;
	int suffixSelection = std::rand() % 10;

	return prefixes[prefixSelection] + suffixes[suffixSelection];
}


// Randomly pick the contestants (Names here are a random mix of some of my favorite new and old era contestants)
std::vector<std::string> setPlayers(int playerCount) {
	std::vector<std::string> potentialPlayers{
	   "Dee", "Sandra", "Rob", "Gabler", "Chris", "Owen", "Cassidy", "Drew", "Colby", "Tina",
	   "Sam", "Parvati", "Genevieve", "Andy", "Ben", "Tony", "Jeremy", "Nick", "Gabby", "Kara",
	   "Rachel", "Carolyn", "Carson", "Russell", "Andrea", "Kelly", "Ciera", "Sierra", "Jesse", "Savage"
	};

	std::random_shuffle(potentialPlayers.begin(), potentialPlayers.end());
	potentialPlayers.resize(playerCount);
	return potentialPlayers;
}


// Create tribes and assign players to them
std::map<std::string, std::vector<std::string>> createTribes(const std::vector<std::string>& players, int formatSelection) {
	std::vector<std::string> tribes;
	tribes.push_back(tribeNameGeneration());
	tribes.push_back(tribeNameGeneration());

	// Add additional tribe for three tribe format
	if (formatSelection == 2) {
		tribes.push_back(tribeNameGeneration());
	}

	std::map<std::string, std::vector<std::string>> tribeAssignments;

	int tribeCount = tribes.size();

	int tribeSize{};

	if (formatSelection == 1) {
		tribeSize = 8;
	}
	else if (formatSelection == 2) {
		tribeSize = 6;
	}
	else {
		tribeSize = 10;
	}

	int index{ 0 };

	for (const auto& tribe : tribes) {
		for (int i = 0; i < tribeSize; ++i) {
			if (index < players.size()) {
				tribeAssignments[tribe].push_back(players[index++]);
			}
		}
	}

	return tribeAssignments;
}


// Size of merge is half of the total player count + 2 (My favorite merge format on Survivor)
int calculateMergeNumber(int formatSelection) {

	int mergeNumber{};

	if (formatSelection == 1) {
		mergeNumber = 10;
	}

	else if (formatSelection == 2) {
		mergeNumber = 11;
	}

	else {
		mergeNumber = 12;
	}

	return mergeNumber;
}


// Handles tribe immunity challenges and tribal council
std::map<std::string, std::vector<std::string>> premergeEliminationCycle(std::map<std::string, std::vector<std::string>>& tribeAssignments) {

	auto losingTribeSelector = tribeAssignments.begin();

	std::advance(losingTribeSelector, std::rand() % tribeAssignments.size());

	std::string selectedTribe = losingTribeSelector->first;
	std::vector<std::string>& tribeMembers = losingTribeSelector->second;

	std::cout << "The " << selectedTribe << " tribe lost the immunity challenge!\n";

	proceedPrompt();

	int index = std::rand() % tribeMembers.size();
	std::string eliminatedPlayer = tribeMembers[index];

	std::cout << eliminatedPlayer << " was voted out by their tribemates!\n";

	tribeMembers.erase(tribeMembers.begin() + index);

	return tribeAssignments;
}


// View the remaining tribes and players
void viewRemainingPremergePlayers(std::map<std::string, std::vector<std::string>>& tribeAssignments) {

	std::cout << "Remaining Players: \n";

	for (const auto& tribe : tribeAssignments) {
		std::cout << tribe.first << " Tribe: ";
		for (const auto& player : tribe.second) {
			std::cout << " " << player;
		}
		std::cout << '\n';
	}
}

// Check if it's time to merge
bool mergeCheck(std::map<std::string, std::vector<std::string>>& tribeAssignments, int mergeNumber) {
	int playerCount{ 0 };
	
	for (const auto& tribe : tribeAssignments) {
		playerCount += tribe.second.size();
	}

	if (playerCount == mergeNumber) {
		return true;
	}
	else {
		return false;
	}
}

// Handles disbanding tribes and merging them into one tribe
std::vector<std::string> merge(std::map<std::string, std::vector<std::string>>& tribeAssignments) {

	std::cout << "It's time for the tribes to merge!\n";

	proceedPrompt();

	std::vector<std::string> mergePlayers;

	for (const auto& tribe : tribeAssignments) {
		mergePlayers.insert(mergePlayers.end(), tribe.second.begin(), tribe.second.end());
	}

	tribeAssignments.clear();

	return mergePlayers;
}


// Handles individual immunity and postmerge tribal councils
std::vector<std::string> postmergeEliminationCycle(std::vector<std::string>& mergedPlayers) {

	int immunityIndex = std::rand() % mergedPlayers.size();

	std::string immunityWinner = mergedPlayers[immunityIndex];

	std::cout << immunityWinner << " has won individual immunity!\n";

	proceedPrompt();

	std::vector<std::string> atRiskPlayers;

	for (const auto& player : mergedPlayers) {
		if (player != immunityWinner) {
			atRiskPlayers.push_back(player);
		}
	}

	std::cout << "Everyone goes to tribal council!\n";

	proceedPrompt();

	int eliminationIndex = std::rand() % atRiskPlayers.size();

	std::string eliminatedPlayer = atRiskPlayers[eliminationIndex];

	std::cout << eliminatedPlayer << " was voted out by their tribemates!\n";

	for (auto i = mergedPlayers.begin(); i != mergedPlayers.end(); ++i) {
		if (*i == eliminatedPlayer) {
			mergedPlayers.erase(i);
			break;
		}
	}

	return mergedPlayers;

}


// View the remaining players on the postmerge tribe 
void viewRemainingPostmergePlayers(std::vector<std::string>& mergedTribe) {

	std::cout << "Remaining Players: \n";

	for (const auto& player : mergedTribe) {
		std::cout << player << " ";
	}

	std::cout << '\n';
}


// Check if only two players remain
bool checkForFinalTwo(std::vector<std::string> mergedPlayers) {
	if (mergedPlayers.size() == 2) {
		return true;
	}

	else {
		return false;
	}
}


// Determines the winner from the final two
std::string determineWinner(std::vector<std::string> mergedPlayers) {
	int winnerIndex = std::rand() % mergedPlayers.size();

	std::cout << "The final two are: " << mergedPlayers[0] << " and " << mergedPlayers[1] << "!\n";

	proceedPrompt();

	std::string winner = mergedPlayers[winnerIndex];

	return winner;
}


// Main gameplay loop 
int gameplayLoop(std::map<std::string, std::vector<std::string>> tribeAssignments, int formatSelection) {
	int mergeNumber = calculateMergeNumber(formatSelection);

	// Premerge Phase
	while (!mergeCheck(tribeAssignments, mergeNumber)) {
		viewRemainingPremergePlayers(tribeAssignments);

		proceedPrompt();

		tribeAssignments = premergeEliminationCycle(tribeAssignments);

		proceedPrompt();
	}

	std::vector<std::string> mergedPlayers = merge(tribeAssignments);

	// Postmerge Phase
	while (!checkForFinalTwo(mergedPlayers)) {

		viewRemainingPostmergePlayers(mergedPlayers);

		proceedPrompt();

		mergedPlayers = postmergeEliminationCycle(mergedPlayers);

		proceedPrompt();
	}

	std::string winner = determineWinner(mergedPlayers);
	std::cout << "The winner is: " << winner << "\n";

	return 0;
}


int main() {
	std::srand(std::time(0));
	int formatSelection = gameFormatSelection();

	int playerCount = 0;

	if (formatSelection == 1) {
		playerCount = 16;
	}
	else if (formatSelection == 2) {
		playerCount = 18;
	}
	else {
		playerCount = 20;
	}

	std::vector<std::string> players = setPlayers(playerCount);
	std::map<std::string, std::vector<std::string>> tribeAssignments = createTribes(players, formatSelection);

	gameplayLoop(tribeAssignments, formatSelection);

	return 0;
}