#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

// ------------------------------------------------ Global Constants ------------------------------------------------

const int TILE_SIZE = 106;
const int HAND_SIZE = 14;
const int MAX_PLAYERS = 4;

// ------------------------------------------------ Classes ------------------------------------------------

enum Color { ORANGE, RED, BLUE, BLACK };

struct Tile {
    int value;
    Color color;
};

struct Player {
    Tile hand[TILE_SIZE];
    int currentHandSize;
};

struct Combination {
    Tile tiles[TILE_SIZE];
    int size;
};

struct Table {
    Combination combinations[TILE_SIZE];
    int combinationsCount;
};

// ------------------------------------------------ Functions Declarations ------------------------------------------------

void enterValidPlayersCount(int& playersCount);

void createTiles(Tile* pile);
void shuffleTiles(Tile* pile, int pileSize = TILE_SIZE);

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

bool tryPlayCombinationTurn(Player& player, Table& table, int playerNumber);

int tilePoints(const Tile& tile);
int handPoints(const Player& player);

bool drawOneTile(Player& player, Tile* pile, int& pileSize);
int findWinnerByLowestPoints(Player* players, int playersCount);

// ------------------------------------------------ main() Function ------------------------------------------------

int main()
{
    srand(time(nullptr));

    Tile pile[TILE_SIZE];
    int pileSize = TILE_SIZE;

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

    while (!gameOver) {
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

    if (index != TILE_SIZE) {
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
    if (player->currentHandSize >= TILE_SIZE)
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

    int values[TILE_SIZE];
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

    if (neededJokers <= jokersCount)
    {
        return true;
    }

    return false;
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
    if (table.combinationsCount >= TILE_SIZE) 
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
    cout << "1) Play a new combination" << endl;
    cout << "2) Draw a tile (end turn)" << endl;
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

    if (choice == 2)
    {
        return false;
    }

    if (choice != 1) 
    {
        cout << "Invalid choice." << endl;
        return false;
    }

    cout << endl << "Player " << playerNumber << ", choose tiles to form a combination." << endl;
    printHandWithIndexes(player);

    cout << "How many tiles in the combination? " << endl;
    int chosenCount;
    cin >> chosenCount;

    if (cin.fail()) 
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input.\n";
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
        cout << "Invalid indexes (out of range, duplicates, or bad input)." << endl;
        return false;
    }

    buildChosenTiles(player, chosenIndexes, chosenCount, chosenTiles);

    if (!isValidCombination(chosenTiles, chosenCount)) 
    {
        cout << "Invalid combination!" << endl;
        return false;
    }

    cout << "Valid combination! Adding to table..." << endl;
    addCombinationToTable(table, chosenTiles, chosenCount);
    removeChosenFromHand(player, chosenIndexes, chosenCount);

    return true;
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

bool drawOneTile(Player& player, Tile* pile, int& pileSize) 
{
    Tile drawn;

    if (!drawFromPile(pile, pileSize, &drawn)) 
    {
        return false;
    }

    addToHand(&player, drawn);

    cout << "You drew: ";
    printTile(drawn);
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