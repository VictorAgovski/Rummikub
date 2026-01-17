#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

// ------------------------------------------------ Global Constants ------------------------------------------------

const int PILE_SIZE = 106;
const int HAND_SIZE = 14;
const int MAX_PLAYERS = 4;

// ------------------------------------------------ Classes ------------------------------------------------

enum Color { ORANGE, RED, BLUE, BLACK };

struct Tile {
    int value;
    Color color;
};

struct Player {
    Tile hand[PILE_SIZE];
    int currentHandSize;
    bool hasOpened;
};

struct Combination {
    Tile tiles[PILE_SIZE];
    int size;
};

struct Table {
    Combination combinations[PILE_SIZE];
    int combinationsCount;
};

// ------------------------------------------------ Functions Declarations ------------------------------------------------

void enterValidPlayersCount(int& playersCount);

void createTiles(Tile* pile);
void shuffleTiles(Tile* pile, int pileSize = PILE_SIZE);

bool drawFromPile(Tile* pile, int& pileSize, Tile* tile);
void addToHand(Player* player, const Tile& tile);
void seedTilesToPlayers(Player* players, int playersCount, Tile* pile, int& pileSize);

char colorToChar(Color color);
void printTile(const Tile& tile);
void printHand(const Player& player);
void printAllHands(const Player* players, const int playersCount);

bool isJoker(const Tile& tile);

bool isValidGroup(const Tile* tiles, int size);

void sortArrayFunc(int* array, int size);
bool isValidRun(const Tile* tiles, int size);

bool isValidCombination(const Tile* tiles, int size);

void printHandWithIndexes(const Player& player);
bool removeFromHand(Player& player, int index);
void addCombinationToTable(Table& table, const Tile* tiles, int size);
void printTable(const Table& table);

bool readChosenIndexes(int* chosenIndexes, int chosenCount, int handSize);
void buildChosenTiles(const Player& player, const int* chosenIndexes, int chosenCount, Tile* chosenTiles);
void sortIndexesDesc(int* array, int size);
void removeChosenFromHand(Player& player, int* chosenIndexes, int chosenCount);

/*!!!!!!!!!!!*/ bool tryPlayCombinationTurn(Player& player, Table& table, int playerNumber); /*!!!!!!!!!!!*/

int tilePoints(const Tile& tile);
int handPoints(const Player& player);

bool drawOneTile(Player& player, Tile* pile, int& pileSize);
int findWinnerByLowestPoints(Player* players, int playersCount);

int combinationPoints(const Tile* tiles, int size);

void addToHandEnd(Player& player, const Tile& tile);
void removeLastCombinations(Table& table, int count);

bool tryAddTileToExistingCombination(Player& player, Table& table, int playerNumber);

bool removeTileFromCombination(Combination& combination, int index, Tile& removedTile);
bool insertTileInCombinationAt(Combination& combination, const Tile& tile, int index);
void printCombinationWithIndexes(const Combination& combination);

/*!!!!!!!!!!!*/ bool tryStealAndUseTile(Player& player, Table& table, int playerNumber); /*!!!!!!!!!!!*/
bool tryCreateNewCombinationWithExtraTile(Player& player, Table& table, const Tile& extraTile);
bool tryAddExtraTileToExistingCombination(Table& table, const Tile& extraTile);

bool tryReplaceJokerAndSteal(Player& player, Combination& source, int jokerIndex, Tile& stolenJoker, Tile& usedHandTile, bool& tookFromHand);

bool splitOneCombinationIntoTwo(const Combination& src, int splitPos, Combination& left, Combination& right);
bool trySplitAfterRemoval(Table& table, int sourceIndex);

bool isTableValid(const Table& table);
void removeEmptyCombinations(Table& table);

bool tryPlayNewCombination(Player& player, Table& table, int playerNumber);
bool tryRearrangeTurn(Player& player, Table& table, int playerNumber);

// ------------------------------------------------ main() Function ------------------------------------------------

int main()
{
    srand(time(nullptr));

    Tile pile[PILE_SIZE];
    int pileSize = PILE_SIZE;

    Player players[MAX_PLAYERS]; 
    int playersCount;
    enterValidPlayersCount(playersCount);

    createTiles(pile);
    shuffleTiles(pile, pileSize);
    seedTilesToPlayers(players, playersCount, pile, pileSize);

    printAllHands(players, playersCount);

    cout << "Remaining tiles in pile: " << pileSize << endl;

    Table table;
    table.combinationsCount = 0;

    bool gameOver = false;

    while (!gameOver) 
    {
        for (int i = 0; i < playersCount; i++) {
            cout << endl << "==============================" << endl;
            printTable(table);

            bool played = tryPlayCombinationTurn(players[i], table, i + 1);

            cout << endl << "Player " << (i + 1) << " new hand:" << endl;
            printHandWithIndexes(players[i]);

            if (players[i].currentHandSize == 0) {
                cout << endl << "Player " << (i + 1) << " wins!" << endl;
                gameOver = true;
                break;
            }

            if (!played) {
                cout << "No combination played. You must draw 1 tile." << endl;

                if (!drawOneTile(players[i], pile, pileSize)) 
                {
                    cout << endl << "Pile is empty. Game ends." << endl;
                    int winnerIndex = findWinnerByLowestPoints(players, playersCount);

                    cout << "Winner is Player " << (winnerIndex + 1) << " with " << handPoints(players[winnerIndex]) << " points." << endl;

                    gameOver = true;
                    break;
                }
            }
        }
    }

    return 0;
}

// ------------------------------------------------ Functions Definitions ------------------------------------------------ 

void enterValidPlayersCount(int& playersCount) {
    while (true) 
    {
        cout << "Enter the number of players (2-4): ";
        cin >> playersCount;

        if (cin.fail() || (playersCount < 2 || playersCount > 4))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Try to add a valid number of players." << endl;
            continue;
        }

        break;
    }
}

void createTiles(Tile* pile){
    const int COLORS_COUNT = 4;
    const int VALUES_COUNT = 13;
    
    int index = 0;

    for (int k = 0; k < 2; k++)
    {
        for (int i = 0; i < COLORS_COUNT; i++)
        {
            for (int j = 1; j <= VALUES_COUNT; j++)
            {
                pile[index].color = (Color)i;
                pile[index].value = j;
                index++;
            }
        }
    }

    // jokers
    pile[index++] = Tile{ 0, RED };
    pile[index++] = Tile{ 0, BLACK };

    if (index != PILE_SIZE) {
        cout << "Error: wrong tiles count: " << index << endl;
        return;
    }
}

void shuffleTiles(Tile* pile, int pileSize) {
    if (pileSize <= 1)
    {
        return;
    }

    for (int i = pileSize - 1; i > 0; i--)
    {
        int randomIndex = rand() % (i + 1);
        Tile temp = pile[i];
        pile[i] = pile[randomIndex];
        pile[randomIndex] = temp;
    }
}

bool drawFromPile(Tile* pile, int& pileSize, Tile* tile) {
    if (tile == nullptr || pileSize <= 0) 
    {
        return false;
    }

    *tile = pile[--pileSize];

    return true;
}

void addToHand(Player* player, const Tile& tile) {
    if (player == nullptr)
    {
        return;
    }
    if (player->currentHandSize >= PILE_SIZE)
    {
        return;
    }

    player->hand[player->currentHandSize] = tile;
    player->currentHandSize++;
}

void seedTilesToPlayers(Player* players, int playersCount, Tile* pile, int& pileSize) {
    if (pileSize < playersCount * HAND_SIZE) 
    {
        return;
    }

    for (int i = 0; i < playersCount; i++)
    {
        players[i].currentHandSize = 0;
        players[i].hasOpened = false;
    }

    for (int i = 0; i < HAND_SIZE; i++)
    {
        for (int j = 0; j < playersCount; j++)
        {
            Tile tile;
            if (drawFromPile(pile, pileSize, &tile))
            {
                addToHand(&players[j], tile);
            }
        }
    }
}

char colorToChar(Color color) {
    switch (color)
    {
    case ORANGE: return 'O';
    case RED: return 'R';
    case BLUE: return 'B';
    case BLACK: return 'K';
    }

    return '?';
}

void printTile(const Tile& tile) {
    if (tile.value == 0) {
        cout << "[J-" << colorToChar(tile.color) << "]";
    }
    else {
        cout << "[" << tile.value << "-" << colorToChar(tile.color) << "]";
    }
}

void printHand(const Player& player) {
    for (int i = 0; i < player.currentHandSize; i++) {
        printTile(player.hand[i]);
        cout << " ";
    }
    cout << endl;
}

void printAllHands(const Player* players, const int playersCount) {
    if (players == nullptr)
    {
        return;
    }

    for (int i = 0; i < playersCount; i++) {
        cout << "Player " << (i + 1) << " hand:" << endl;
        printHand(players[i]);
    }
}

bool isJoker(const Tile& tile) {
    if (tile.value == 0)
    {
        return true;
    }
    return false;
}

bool isValidGroup(const Tile* tiles, int size) {
    if (size != 3 && size != 4) 
    {
        return false;
    }

    const int COLORS_COUNT = 4;
    int targetValue = -1;

    for (int i = 0; i < size; i++) 
    {
        const Tile& currentTile = tiles[i];
        if (!isJoker(currentTile))
        {
            targetValue = currentTile.value;
            break;
        }
    }

    if (targetValue == -1) {
        return true;
    }

    bool usedColors[COLORS_COUNT] = { false, false, false, false };

    for (int i = 0; i < size; i++) 
    {
        const Tile& currentTile = tiles[i];

        if (isJoker(currentTile))
        {
            continue;
        }

        if (currentTile.value != targetValue)
        {
            return false;
        }

        if (usedColors[(int)currentTile.color])
        {
            return false;
        }
        usedColors[(int)currentTile.color] = true;
    }

    return true;
}

void sortArrayFunc(int* array, int size) {
    if (array == nullptr || size <= 1)
    {
        return;
    }

    for (int i = 0; i < size - 1; i++) 
    {
        for (int j = 0; j < size - i - 1; j++) 
        {
            if (array[j] > array[j + 1]) 
            {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

bool isValidRun(const Tile* tiles, int size) {
    if (tiles == nullptr) 
    {
        return false;
    }
    if (size < 3) 
    {
        return false;
    }

    bool hasNonJoker = false;
    Color runColor = ORANGE;

    for (int i = 0; i < size; i++) 
    {
        const Tile& currentTile = tiles[i];

        if (!isJoker(currentTile)) 
        {
            runColor = currentTile.color;
            hasNonJoker = true;
            break;
        }
    }

    if (!hasNonJoker)
    {
        return true;
    }

    int values[PILE_SIZE];
    int valuesCount = 0;
    int jokersCount = 0;

    for (int i = 0; i < size; i++) 
    {
        const Tile& currentTile = tiles[i];

        if (isJoker(currentTile)) 
        {
            jokersCount++;
            continue;
        }

        if (currentTile.color != runColor) 
        {
            return false;
        }

        values[valuesCount++] = currentTile.value;
    }

    sortArrayFunc(values, valuesCount);

    for (int i = 1; i < valuesCount; i++)
    {
        if (values[i] == values[i - 1]) 
        {
            return false;
        }
    }

    int neededJokers = 0;
    for (int i = 1; i < valuesCount; i++) 
    {
        int gap = values[i] - values[i - 1] - 1;

        if (gap < 0) 
        {
            return false;
        }
        neededJokers += gap;
    }

    if (neededJokers > jokersCount)
    {
        return false;
    }

    int extra = jokersCount - neededJokers;

    int minV = values[0];
    int maxV = values[valuesCount - 1];

    int maxExtend = (minV - 1) + (13 - maxV);

    return extra <= maxExtend;
}

bool isValidCombination(const Tile* tiles, int size) {
    return isValidGroup(tiles, size) || isValidRun(tiles, size);
}

void printHandWithIndexes(const Player& player) {
    for (int i = 0; i < player.currentHandSize; i++) 
    {
        cout << i << ": ";
        printTile(player.hand[i]);
        cout << endl;
    }
}

bool removeFromHand(Player& player, int index) {
    if (index < 0 || index >= player.currentHandSize) 
    {
        return false;
    }

    for (int i = index; i < player.currentHandSize - 1; i++) 
    {
        player.hand[i] = player.hand[i + 1];
    }
    player.currentHandSize--;

    return true;
}

void addCombinationToTable(Table& table, const Tile* tiles, int size) {
    if (table.combinationsCount >= PILE_SIZE) 
    {
        return;
    }

    table.combinations[table.combinationsCount].size = size;

    for (int i = 0; i < size; i++) 
    {
        table.combinations[table.combinationsCount].tiles[i] = tiles[i];
    }
    table.combinationsCount++;
}

void printTable(const Table& table) {
    cout << endl << "TABLE:" << endl;

    if (table.combinationsCount == 0) 
    {
        cout << "(empty)" << endl;
        return;
    }

    for (int i = 0; i < table.combinationsCount; i++) 
    {
        cout << i << ") ";

        for (int j = 0; j < table.combinations[i].size; j++) 
        {
            printTile(table.combinations[i].tiles[j]);
            cout << " ";
        }

        cout << endl;
    }
}

bool readChosenIndexes(int* chosenIndexes, int chosenCount, int handSize) {
    for (int i = 0; i < chosenCount; i++) 
    {
        cin >> chosenIndexes[i];

        if (cin.fail()) 
        {
            cin.clear(); 
            cin.ignore(10000, '\n'); 
            return false; 
        }

        if (chosenIndexes[i] < 0 || chosenIndexes[i] >= handSize) 
        {
            return false;
        }

        for (int j = 0; j < i; j++) 
        {
            if (chosenIndexes[i] == chosenIndexes[j]) 
            {
                return false;
            }
        }
    }

    return true;
}

void buildChosenTiles(const Player& player, const int* chosenIndexes, int chosenCount, Tile* chosenTiles) {
    for (int i = 0; i < chosenCount; i++) 
    {
        chosenTiles[i] = player.hand[chosenIndexes[i]];
    }
}

void sortIndexesDesc(int* array, int size) {
    if (array == nullptr || size <= 1)
    {
        return;
    }

    for (int i = 0; i < size - 1; i++) 
    {
        for (int j = 0; j < size - i - 1; j++) 
        {
            if (array[j] < array[j + 1])
            {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

void removeChosenFromHand(Player& player, int* chosenIndexes, int chosenCount) {
    sortIndexesDesc(chosenIndexes, chosenCount);

    for (int i = 0; i < chosenCount; i++) 
    {
        removeFromHand(player, chosenIndexes[i]);
    }
}

bool tryPlayCombinationTurn(Player& player, Table& table, int playerNumber) {
    cout << endl << "Player " << playerNumber << " turn." << endl;

    if (player.hasOpened)
    {
        cout << "1) Play a new combination" << endl;
        cout << "2) Add a tile to an existing table combination" << endl;
        cout << "3) Steal a tile from table and use it (same turn)" << endl;
        cout << "4) Rearrange table (multiple actions, commit/rollback)" << endl;
        cout << "5) Draw a tile (end turn)" << endl;
        cout << "Choose: ";

        int choice;
        cin >> choice;

        if (cin.fail()) 
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input." << endl;
            return false;
        }

        if (choice == 5) 
        {
            return false;
        }
        if (choice == 4)
        {
            return tryRearrangeTurn(player, table, playerNumber);
        }
        if (choice == 3)
        {
            return tryStealAndUseTile(player, table, playerNumber);
        }
        if (choice == 2) 
        {
            return tryAddTileToExistingCombination(player, table, playerNumber);
        }
        if (choice == 1)
        {
            return tryPlayNewCombination(player, table, playerNumber);
        }

        cout << "Invalid choice." << endl;
        return false;
    }

    cout << "You have NOT opened yet. You must place combinations with total >= 30 points this turn." << endl;

    int turnSum = 0;

    int startCombinationsCount = table.combinationsCount;

    Tile takenTiles[HAND_SIZE];
    int takenCount = 0;

    while (true)
    {
        cout << endl << "Current placed points this turn: " << turnSum << " / 30" << endl;
        cout << "1) Add a combination" << endl;
        cout << "2) Draw a tile (give up opening and end turn)" << endl;
        cout << "Choose: ";

        int choice;
        cin >> choice;

        if (cin.fail()) 
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input." << endl;

            removeLastCombinations(table, table.combinationsCount - startCombinationsCount);

            for (int i = 0; i < takenCount; i++)
            {
                addToHandEnd(player, takenTiles[i]);
            }

            return false;
        }

        if (choice == 2) 
        {
            removeLastCombinations(table, table.combinationsCount - startCombinationsCount);

            for (int i = 0; i < takenCount; i++) 
            {
                addToHandEnd(player, takenTiles[i]);
            }
            
            return false;
        }

        if (choice != 1) 
        {
            cout << "Invalid choice." << endl;
            continue;
        }

        cout << endl << "Choose tiles to form a combination." << endl;
        printHandWithIndexes(player);

        cout << "How many tiles in the combination?" << endl;
        int chosenCount;
        cin >> chosenCount;

        if (cin.fail()) 
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input." << endl;
            continue;
        }

        if (chosenCount < 3 || chosenCount > HAND_SIZE || chosenCount > player.currentHandSize) 
        {
            cout << "Invalid count." << endl;
            continue;
        }

        int chosenIndexes[HAND_SIZE];
        Tile chosenTiles[HAND_SIZE];

        cout << "Enter " << chosenCount << " indexes: ";

        if (!readChosenIndexes(chosenIndexes, chosenCount, player.currentHandSize)) 
        {
            cout << "Invalid indexes." << endl;
            continue;
        }

        buildChosenTiles(player, chosenIndexes, chosenCount, chosenTiles);

        if (!isValidCombination(chosenTiles, chosenCount)) 
        {
            cout << "Invalid combination!" << endl;
            continue;
        }

        int pts = combinationPoints(chosenTiles, chosenCount);
        cout << "This combination points: " << pts << endl;

        for (int i = 0; i < chosenCount; i++) 
        {
            if (takenCount < HAND_SIZE) 
            {
                takenTiles[takenCount++] = chosenTiles[i];
            }
        }

        addCombinationToTable(table, chosenTiles, chosenCount);
        removeChosenFromHand(player, chosenIndexes, chosenCount);

        turnSum += pts;

        if (turnSum >= 30) 
        {
            player.hasOpened = true;
            cout << "You have opened (>=30)! Turn ends." << endl;
            return true;
        }
    }
}

int tilePoints(const Tile& tile) {
    if (isJoker(tile)) 
    {
        return 30;
    }

    return tile.value;
}

int handPoints(const Player& player) {
    int sum = 0;

    for (int i = 0; i < player.currentHandSize; i++) 
    {
        sum += tilePoints(player.hand[i]);
    }

    return sum;
}

bool drawOneTile(Player& player, Tile* pile, int& pileSize) {
    Tile drawnTile;

    if (!drawFromPile(pile, pileSize, &drawnTile)) 
    {
        return false;
    }

    addToHand(&player, drawnTile);

    cout << "You drew: ";
    printTile(drawnTile);
    cout << endl;

    return true;
}

int findWinnerByLowestPoints(Player* players, int playersCount) {
    int bestIndex = 0;
    int bestPoints = handPoints(players[0]);

    for (int i = 1; i < playersCount; i++) 
    {
        int points = handPoints(players[i]);
        if (points < bestPoints)
        {
            bestPoints = points;
            bestIndex = i;
        }
    }
    return bestIndex;
}

int combinationPoints(const Tile* tiles, int size) {
    int sum = 0;

    for (int i = 0; i < size; i++) 
    {
        sum += tilePoints(tiles[i]);
    }
    return sum;
}

void addToHandEnd(Player& player, const Tile& tile) {
    if (player.currentHandSize >= PILE_SIZE) 
    {
        return;
    }

    player.hand[player.currentHandSize++] = tile;
}

void removeLastCombinations(Table& table, int count) {
    if (count <= 0) 
    {
        return;
    }

    if (count > table.combinationsCount) 
    {
        count = table.combinationsCount;
    }

    table.combinationsCount -= count;
}

bool tryAddTileToExistingCombination(Player& player, Table& table, int playerNumber) {
    if (table.combinationsCount <= 0)
    {
        cout << "Table is empty. No combinations to add to." << endl;
        return false;
    }

    cout << endl << "Player " << playerNumber << " - Extend existing combination (add >=1 tiles)" << endl;

    cout << "Choose table combination index (0.." << (table.combinationsCount - 1) << "): ";
    int tableIndex;
    cin >> tableIndex;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input." << endl;
        return false;
    }

    if (tableIndex < 0 || tableIndex >= table.combinationsCount)
    {
        cout << "Invalid table index." << endl;
        return false;
    }

    if (player.currentHandSize <= 0)
    {
        cout << "You have no tiles." << endl;
        return false;
    }

    cout << "How many tiles do you want to add (1.." << player.currentHandSize << "): ";
    int addCount;
    cin >> addCount;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input." << endl;
        return false;
    }

    if (addCount < 1 || addCount > player.currentHandSize)
    {
        cout << "Invalid count." << endl;
        return false;
    }

    Combination oldCombination = table.combinations[tableIndex];

    Tile removedTiles[HAND_SIZE];
    int removedCount = 0;

    for (int step = 0; step < addCount; step++)
    {
        cout << endl << "Your hand:" << endl;
        printHandWithIndexes(player);

        cout << "Choose hand tile index to add (" << (step + 1) << "/" << addCount << "): ";
        int handIndex;
        cin >> handIndex;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input." << endl;

            table.combinations[tableIndex] = oldCombination;
            for (int i = 0; i < removedCount; i++) 
            {
                addToHandEnd(player, removedTiles[i]);
            }
            return false;
        }

        if (handIndex < 0 || handIndex >= player.currentHandSize)
        {
            cout << "Invalid hand index." << endl;

            table.combinations[tableIndex] = oldCombination;
            for (int i = 0; i < removedCount; i++) 
            {
                addToHandEnd(player, removedTiles[i]);
            }

            return false;
        }

        cout << "Insert position index (0.." << table.combinations[tableIndex].size << "): ";
        int insertPos;
        cin >> insertPos;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input." << endl;

            table.combinations[tableIndex] = oldCombination;

            for (int i = 0; i < removedCount; i++) 
            {
                addToHandEnd(player, removedTiles[i]);
            }
            
            return false;
        }

        if (insertPos < 0 || insertPos > table.combinations[tableIndex].size)
        {
            cout << "Invalid insert position." << endl;

            table.combinations[tableIndex] = oldCombination;

            for (int i = 0; i < removedCount; i++) 
            {
                addToHandEnd(player, removedTiles[i]);
            }
            
            return false;
        }

        Tile tileToAdd = player.hand[handIndex];

        if (!removeFromHand(player, handIndex))
        {
            cout << "Failed to remove tile from hand." << endl;

            table.combinations[tableIndex] = oldCombination;

            for (int i = 0; i < removedCount; i++) 
            {
                addToHandEnd(player, removedTiles[i]);
            }
            
            return false;
        }

        if (removedCount < HAND_SIZE)
        {
            removedTiles[removedCount++] = tileToAdd;
        }

        if (!insertTileInCombinationAt(table.combinations[tableIndex], tileToAdd, insertPos))
        {
            cout << "Failed to insert tile in combination." << endl;

            table.combinations[tableIndex] = oldCombination;

            for (int i = 0; i < removedCount; i++) 
            {
                addToHandEnd(player, removedTiles[i]);
            }
            
            return false;
        }
    }

    Combination& modified = table.combinations[tableIndex];

    if (!isValidCombination(modified.tiles, modified.size))
    {
        cout << "Invalid after adding tiles. Move cancelled." << endl;

        table.combinations[tableIndex] = oldCombination;

        for (int i = 0; i < removedCount; i++) 
        {
            addToHandEnd(player, removedTiles[i]);
        }
        
        return false;
    }

    cout << "Tiles added successfully." << endl;
    return true;
}

void printCombinationWithIndexes(const Combination& combination) {
    for (int i = 0; i < combination.size; i++) 
    {
        cout << i << ": ";
        printTile(combination.tiles[i]);
        cout << " ";
    }

    cout << endl;
}

bool removeTileFromCombination(Combination& combination, int index, Tile& removedTile) {
    if (index < 0 || index >= combination.size) 
    {
        return false;
    }
    
    removedTile = combination.tiles[index];

    for (int i = index; i < combination.size - 1; i++) 
    {
        combination.tiles[i] = combination.tiles[i + 1];
    }
    combination.size--;

    return true;
}

bool insertTileInCombinationAt(Combination& combination, const Tile& tile, int index) {
    if (combination.size >= PILE_SIZE) 
    {
        return false;
    }
    if (index < 0 || index > combination.size) 
    {
        return false;
    }

    for (int i = combination.size; i > index; i--) 
    {
        combination.tiles[i] = combination.tiles[i - 1];
    }

    combination.tiles[index] = tile;
    combination.size++;

    return true;
}

bool tryCreateNewCombinationWithExtraTile(Player& player, Table& table, const Tile& extraTile) {
    cout << "How many tiles from your hand to use with the stolen tile? (0.." << player.currentHandSize << "): ";

    int fromHandCount;
    cin >> fromHandCount;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input." << endl;
        return false;
    }

    if (fromHandCount < 0 || fromHandCount > player.currentHandSize) 
    {
        cout << "Invalid count." << endl;
        return false;
    }

    int newSize = fromHandCount + 1;

    if (newSize < 3)
    {
        cout << "Too few tiles. A combination must be at least 3." << endl;
        return false;
    }

    int chosenIndexes[PILE_SIZE];
    Tile newTiles[PILE_SIZE];

    if (fromHandCount > 0) 
    {
        cout << "Enter " << fromHandCount << " hand indexes: ";
        if (!readChosenIndexes(chosenIndexes, fromHandCount, player.currentHandSize)) 
        {
            cout << "Invalid hand indexes." << endl;
            return false;
        }

        buildChosenTiles(player, chosenIndexes, fromHandCount, newTiles);
    }

    newTiles[fromHandCount] = extraTile;

    if (!isValidCombination(newTiles, newSize))
    {
        cout << "Not a valid combination." << endl;
        return false;
    }

    if (fromHandCount > 0) 
    {
        removeChosenFromHand(player, chosenIndexes, fromHandCount);
    }

    addCombinationToTable(table, newTiles, newSize);

    cout << "New combination created successfully." << endl;

    return true;
}

bool tryAddExtraTileToExistingCombination(Table& table, const Tile& extraTile) {
    if (table.combinationsCount <= 0) 
    {
        return false;
    }

    cout << "Choose target table combination index (0.." << (table.combinationsCount - 1) << "): ";
    int targetIndex;
    cin >> targetIndex;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input." << endl;
        return false;
    }

    if (targetIndex < 0 || targetIndex >= table.combinationsCount) 
    {
        cout << "Invalid table index." << endl;
        return false;
    }

    cout << "Target combination is:" << endl;
    printCombinationWithIndexes(table.combinations[targetIndex]);

    cout << "Insert position index (0.." << table.combinations[targetIndex].size << "): ";
    int insertPos;
    cin >> insertPos;

    if (cin.fail()) 
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input." << endl;
        return false;
    }

    Combination old = table.combinations[targetIndex];

    if (!insertTileInCombinationAt(table.combinations[targetIndex], extraTile, insertPos))
    {
        cout << "Insert failed." << endl;
        table.combinations[targetIndex] = old;
        return false;
    }

    if (!isValidCombination(table.combinations[targetIndex].tiles, table.combinations[targetIndex].size))
    {
        cout << "Invalid after inserting. Move cancelled." << endl;
        table.combinations[targetIndex] = old;
        return false;
    }

    cout << "Stolen tile added to existing combination successfully." << endl;

    return true;
}

bool tryStealAndUseTile(Player& player, Table& table, int playerNumber) {
    if (table.combinationsCount <= 0)
    {
        cout << "Table is empty. Nothing to steal." << endl;
        return false;
    }

    cout << endl << "Player " << playerNumber << " - Steal & use a tile (same turn)" << endl;
    printTable(table);

    cout << "Choose source table combination index (0.." << (table.combinationsCount - 1) << "): ";
    int sourceIndex;
    cin >> sourceIndex;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input." << endl;
        return false;
    }

    if (sourceIndex < 0 || sourceIndex >= table.combinationsCount)
    {
        cout << "Invalid table index." << endl;
        return false;
    }

    cout << "Source combination tiles:" << endl;
    printCombinationWithIndexes(table.combinations[sourceIndex]);

    cout << "Choose tile index to steal (0.." << (table.combinations[sourceIndex].size - 1) << "): ";
    int tileIndex;
    cin >> tileIndex;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input." << endl;
        return false;
    }

    if (tileIndex < 0 || tileIndex >= table.combinations[sourceIndex].size)
    {
        cout << "Invalid tile index." << endl;
        return false;
    }

    Combination oldSource = table.combinations[sourceIndex];

    Tile stolen;
    Tile usedHandTile;
    bool tookFromHand = false;

    if (isJoker(table.combinations[sourceIndex].tiles[tileIndex]))
    {
        if (!tryReplaceJokerAndSteal(player, table.combinations[sourceIndex], tileIndex, stolen, usedHandTile, tookFromHand))
        {
            table.combinations[sourceIndex] = oldSource;
            return false;
        }
    }
    else
    {
        if (!removeTileFromCombination(table.combinations[sourceIndex], tileIndex, stolen))
        {
            cout << "Failed to steal." << endl;
            table.combinations[sourceIndex] = oldSource;
            return false;
        }

        if (!isValidCombination(table.combinations[sourceIndex].tiles, table.combinations[sourceIndex].size))
        {
            if (!trySplitAfterRemoval(table, sourceIndex))
            {
                cout << "You can't steal that tile: source combination becomes invalid (and can't be split into valid parts)." << endl;
                table.combinations[sourceIndex] = oldSource;
                return false;
            }
        }
    }

    cout << "Stolen tile: ";
    printTile(stolen);
    cout << endl;

    cout << "Use stolen tile by:" << endl;
    cout << "1) Create a NEW combination (using your hand + stolen tile)" << endl;
    cout << "2) Add it to an EXISTING table combination" << endl;
    cout << "Choose: ";

    int useChoice;
    cin >> useChoice;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input." << endl;
        table.combinations[sourceIndex] = oldSource;
        return false;
    }

    bool success = false;

    if (useChoice == 1)
    {
        cout << "Your hand:" << endl;
        printHandWithIndexes(player);

        success = tryCreateNewCombinationWithExtraTile(player, table, stolen);

        if (!success)
        {
            cout << "Can't create a new combination with the stolen tile." << endl;
            cout << "According to rules, you must add it to an existing table combination (if possible)." << endl;

            success = tryAddExtraTileToExistingCombination(table, stolen);
        }
    }
    else if (useChoice == 2)
    {
        success = tryAddExtraTileToExistingCombination(table, stolen);
    }
    else
    {
        cout << "Invalid choice." << endl;
        success = false;
    }

    if (!success)
    {
        table.combinations[sourceIndex] = oldSource;

        if (tookFromHand)
        {
            addToHandEnd(player, usedHandTile);
        }

        cout << "Steal move cancelled. Everything rolled back." << endl;
        return false;
    }

    cout << "Steal move successful." << endl;
    return true;
}

bool tryReplaceJokerAndSteal(Player& player, Combination& source, int jokerIndex, Tile& stolenJoker, Tile& usedHandTile, bool& tookFromHand)
{
    tookFromHand = false;
    stolenJoker = source.tiles[jokerIndex];

    cout << "You chose to steal a JOKER." << endl;
    cout << "To steal it, you must REPLACE it with a tile from your hand so the combination stays valid." << endl;

    cout << "Your hand:" << endl;
    printHandWithIndexes(player);

    cout << "Enter hand index to place instead of the Joker (-1 to cancel): ";
    int handIndex;
    cin >> handIndex;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input." << endl;
        return false;
    }

    if (handIndex == -1)
    {
        cout << "Cancelled." << endl;
        return false;
    }

    if (handIndex < 0 || handIndex >= player.currentHandSize)
    {
        cout << "Invalid hand index." << endl;
        return false;
    }

    Tile candidate = player.hand[handIndex];

    if (isJoker(candidate))
    {
        cout << "You can't replace a Joker with another Joker." << endl;
        return false;
    }

    Tile oldTile = source.tiles[jokerIndex];
    source.tiles[jokerIndex] = candidate;

    if (!isValidCombination(source.tiles, source.size))
    {
        source.tiles[jokerIndex] = oldTile;
        cout << "Replacement makes the source combination invalid. Can't steal this Joker that way." << endl;
        return false;
    }

    if (!removeFromHand(player, handIndex))
    {
        source.tiles[jokerIndex] = oldTile;
        cout << "Failed to remove tile from hand." << endl;
        return false;
    }

    usedHandTile = candidate;
    tookFromHand = true;

    cout << "Joker replaced successfully. You stole: ";
    printTile(stolenJoker);
    cout << endl;

    return true;
}

bool splitOneCombinationIntoTwo(const Combination& src, int splitPos, Combination& left, Combination& right)
{
    if (splitPos <= 0 || splitPos >= src.size)
    {
        return false;
    }

    left.size = splitPos;
    right.size = src.size - splitPos;

    for (int i = 0; i < left.size; i++)
    {
        left.tiles[i] = src.tiles[i];
    }

    for (int i = 0; i < right.size; i++)
    {
        right.tiles[i] = src.tiles[splitPos + i];
    }

    return true;
}

bool trySplitAfterRemoval(Table& table, int sourceIndex)
{
    if (sourceIndex < 0 || sourceIndex >= table.combinationsCount)
    {
        return false;
    }

    Combination& src = table.combinations[sourceIndex];

    if (src.size < 6)
    {
        return false;
    }

    for (int splitPos = 3; splitPos <= src.size - 3; splitPos++)
    {
        Combination left, right;
        if (!splitOneCombinationIntoTwo(src, splitPos, left, right))
        {
            continue;
        }

        if (isValidCombination(left.tiles, left.size) && isValidCombination(right.tiles, right.size))
        {
            if (table.combinationsCount >= PILE_SIZE)
            {
                return false;
            }

            table.combinations[sourceIndex] = left;
            table.combinations[table.combinationsCount++] = right;

            return true;
        }
    }

    return false;
}

void removeEmptyCombinations(Table& table)
{
    int write = 0;

    for (int i = 0; i < table.combinationsCount; i++)
    {
        if (table.combinations[i].size > 0)
        {
            if (write != i) 
            {
                table.combinations[write] = table.combinations[i];
            }            
            write++;
        }
    }

    table.combinationsCount = write;
}

bool isTableValid(const Table& table)
{
    for (int i = 0; i < table.combinationsCount; i++)
    {
        if (!isValidCombination(table.combinations[i].tiles, table.combinations[i].size))
        {
            return false;
        }
    }

    return true;
}

bool tryPlayNewCombination(Player& player, Table& table, int playerNumber)
{
    cout << endl << "Player " << playerNumber << " - Play a NEW combination" << endl;
    cout << "Choose tiles to form a combination." << endl;
    printHandWithIndexes(player);

    cout << "How many tiles in the combination?" << endl;
    int chosenCount;
    cin >> chosenCount;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input." << endl;
        return false;
    }

    if (chosenCount < 3 || chosenCount > HAND_SIZE || chosenCount > player.currentHandSize)
    {
        cout << "Invalid count." << endl;
        return false;
    }

    int chosenIndexes[HAND_SIZE];
    Tile chosenTiles[HAND_SIZE];

    cout << "Enter " << chosenCount << " indexes: ";

    if (!readChosenIndexes(chosenIndexes, chosenCount, player.currentHandSize))
    {
        cout << "Invalid indexes." << endl;
        return false;
    }

    buildChosenTiles(player, chosenIndexes, chosenCount, chosenTiles);

    if (!isValidCombination(chosenTiles, chosenCount))
    {
        cout << "Invalid combination!" << endl;
        return false;
    }

    addCombinationToTable(table, chosenTiles, chosenCount);
    removeChosenFromHand(player, chosenIndexes, chosenCount);

    cout << "Combination added." << endl;
    return true;
}

bool tryRearrangeTurn(Player& player, Table& table, int playerNumber)
{
    cout << endl << "=== REARRANGE MODE (multiple actions in one turn) ===" << endl;
    cout << "You can do multiple actions. At the end, the table must be fully valid." << endl;

    Player tempPlayer = player;
    Table tempTable = table;

    bool didSomething = false;

    while (true)
    {
        cout << endl << "----- TEMP TABLE -----" << endl;
        printTable(tempTable);

        cout << endl << "----- TEMP HAND (Player " << playerNumber << ") -----" << endl;
        printHandWithIndexes(tempPlayer);

        cout << endl;
        cout << "1) Play a NEW combination (from hand)" << endl;
        cout << "2) Add tile(s) to an existing combination (from hand)" << endl;
        cout << "3) Steal a tile and use it (same action)" << endl;
        cout << "4) Finish rearrange (commit if valid)" << endl;
        cout << "5) Cancel rearrange (rollback)" << endl;
        cout << "Choose: ";

        int choice;
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Rearrange cancelled." << endl;
            return false;
        }

        if (choice == 5)
        {
            cout << "Rearrange cancelled. Rollback." << endl;
            return false;
        }

        if (choice == 4)
        {
            removeEmptyCombinations(tempTable);

            if (!didSomething)
            {
                cout << "You didn't change anything. Rearrange not applied." << endl;
                return false;
            }

            if (!isTableValid(tempTable))
            {
                cout << "Final table is NOT valid. Rearrange cancelled (rollback)." << endl;
                return false;
            }

            player = tempPlayer;
            table = tempTable;

            cout << "Rearrange committed successfully!" << endl;
            return true;
        }

        bool stepSuccess = false;

        if (choice == 1)
        {
            stepSuccess = tryPlayNewCombination(tempPlayer, tempTable, playerNumber);
        }
        else if (choice == 2)
        {
            stepSuccess = tryAddTileToExistingCombination(tempPlayer, tempTable, playerNumber);
        }
        else if (choice == 3)
        {
            stepSuccess = tryStealAndUseTile(tempPlayer, tempTable, playerNumber);
        }
        else
        {
            cout << "Invalid choice." << endl;
            stepSuccess = false;
        }

        if (stepSuccess)
        {
            didSomething = true;
            removeEmptyCombinations(tempTable);
        }
        else
        {
            cout << "This action failed. You can try another action, or finish/cancel." << endl;
        }
    }
}
