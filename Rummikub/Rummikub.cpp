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

        if (playersCount >= 2 && playersCount <= 4) 
        {
            break;
        }
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