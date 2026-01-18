/**
*
* Solution to course project # 7
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2025/2026
*
* @author Victor Agovski
* @idnumber 8MI0600576* @compiler VC
*
* main project
*
*/

#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

// ------------------------------------------------ Global Constants ------------------------------------------------

// Size
const int PILE_SIZE = 106;
const int HAND_SIZE = 14;
const int MAX_PLAYERS = 4;
const int MIN_PLAYERS = 2;

const int MAX_TILE_VALUE = 13;

const int HAND_CAPACITY = PILE_SIZE;
const int MAX_COMBINATIONS = PILE_SIZE;
const int COMBINATION_CAPACITY = PILE_SIZE;

// Game Rules
const int OPENING_POINTS = 30;

const int MIN_COMBINATION_SIZE = 3;
const int MIN_PART_AFTER_SPLIT = 3;
const int MIN_SPLITTABLE_COMBINATION_SIZE = 6;

const int MAX_GROUP_SIZE = 4;
const int JOKER_VALUE = 0;

const int COLOR_COUNT = 4;
const int DUPLICATE_SETS = 2;
const int MIN_TILE_VALUE = 1;

// Input
const int CIN_IGNORE_LIMIT = 10000;

// Commands
const int CANCEL_COMMAND = -1;

// Menu choices - main turn (opened)
const int TURN_PLAY_NEW = 1;
const int TURN_EXTEND_EXISTING = 2;
const int TURN_STEAL_AND_USE = 3;
const int TURN_REARRANGE = 4;
const int TURN_DRAW_END = 5;

const int REARRANGE_PLAY_NEW = 1;
const int REARRANGE_EXTEND_EXISTING = 2;
const int REARRANGE_STEAL_AND_USE = 3;
const int REARRANGE_FINISH_COMMIT = 4;
const int REARRANGE_CANCEL_ROLLBACK = 5;

// Menu choices - opening turn (not opened)
const int OPEN_TURN_ADD_COMBINATION = 1;
const int OPEN_TURN_DRAW_GIVE_UP = 2;

// Menu choices - steal usage
const int STEAL_USE_NEW_COMBINATION = 1;
const int STEAL_USE_ADD_TO_EXISTING = 2;

// Messages (Errors)
const char* MSG_INVALID_INPUT = "Invalid input.";
const char* MSG_INVALID_CHOICE = "Invalid choice.";
const char* MSG_INVALID_COUNT = "Invalid count.";
const char* MSG_INVALID_HAND_INDEX = "Invalid hand index.";
const char* MSG_INVALID_TABLE_INDEX = "Invalid table index.";

// Messages (Info/Prompts)
const char* MSG_ENTER_PLAYERS = "Enter the number of players (2-4): ";
const char* MSG_TRY_VALID_PLAYERS = "Try to add a valid number of players.";

const char* MSG_TABLE_HEADER = "TABLE:";
const char* MSG_TABLE_EMPTY_PRINT = "(empty)";

const char* MSG_NO_COMB_PLAYED_DRAW = "No combination played. You must draw 1 tile.";
const char* MSG_PILE_EMPTY_ENDS = "Pile is empty. Game ends.";

const char* MSG_OPENING_REQUIRED_PREFIX = "You have NOT opened yet. You must place combinations with total >= ";
const char* MSG_OPENING_REQUIRED_SUFFIX = " points this turn.";

const char* MSG_INVALID_INDEXES = "Invalid indexes.";
const char* MSG_INVALID_COMBINATION = "Invalid combination!";
const char* MSG_COMBINATION_ADDED = "Combination added.";

const char* MSG_STEAL_CANCEL_ROLLBACK = "Steal move cancelled. Everything rolled back.";
const char* MSG_STEAL_SUCCESS = "Steal move successful.";

const char* MSG_REMAINING_TILES_IN_PILE = "Remaining tiles in pile: ";
const char* MSG_ROUND_PREFIX = "========== ROUND ";
const char* MSG_ROUND_SUFFIX = " ==========";
const char* MSG_PLAYER_NEW_HAND_SUFFIX = " new hand:";
const char* MSG_PLAYER_WINS_SUFFIX = " wins!";
const char* MSG_WINNER_PREFIX = "Winner is Player ";
const char* MSG_WINNER_WITH = " with ";
const char* MSG_POINTS_SUFFIX = " points.";
const char* MSG_SEPARATOR_LINE = "==============================";

const char* MSG_PLAYER_TURN_SUFFIX = " turn.";
const char* MSG_YOUR_HAND = "Your hand:";

const char* MSG_MENU_PLAY_NEW = "1) Play a new combination";
const char* MSG_MENU_EXTEND_EXISTING = "2) Add a tile to an existing table combination";
const char* MSG_MENU_STEAL_AND_USE = "3) Steal a tile from table and use it (same turn)";
const char* MSG_MENU_REARRANGE = "4) Rearrange table (multiple actions, commit/rollback)";
const char* MSG_MENU_DRAW_END = "5) Draw a tile (end turn)";
const char* MSG_CHOOSE_PROMPT = "Choose: ";

const char* MSG_OPENING_POINTS_STATUS_PREFIX = "Current placed points this turn: ";
const char* MSG_SLASH = " / ";
const char* MSG_OPEN_MENU_ADD_COMBINATION = "1) Add a combination";
const char* MSG_OPEN_MENU_DRAW_GIVE_UP = "2) Draw a tile (give up opening and end turn)";
const char* MSG_CHOOSE_TILES_TO_FORM = "Choose tiles to form a combination.";
const char* MSG_HOW_MANY_TILES_IN_COMB = "How many tiles in the combination?";
const char* MSG_ENTER_INDEXES_PREFIX = "Enter ";
const char* MSG_ENTER_INDEXES_SUFFIX = " indexes: ";
const char* MSG_THIS_COMBINATION_POINTS_PREFIX = "This combination points: ";
const char* MSG_OPENED_SUCCESS_PREFIX = "You have opened (>=";
const char* MSG_OPENED_SUCCESS_SUFFIX = ")! Turn ends.";

const char* MSG_YOU_DREW_PREFIX = "You drew: ";

const char* MSG_TABLE_EMPTY_NO_COMB_TO_ADD = "Table is empty. No combinations to add to.";
const char* MSG_EXTEND_TITLE_PREFIX = "Player ";
const char* MSG_EXTEND_TITLE_SUFFIX = " - Extend existing combination (add >=1 tiles)";
const char* MSG_CHOOSE_TABLE_INDEX_PREFIX = "Choose table combination index (0..";
const char* MSG_CHOOSE_TABLE_INDEX_SUFFIX = "): ";
const char* MSG_SPACE = " ";
const char* MSG_COLON = ": ";
const char* MSG_CLOSING_BRACKET = ") ";
const char* MSG_NO_TILES_IN_HAND = "You have no tiles.";
const char* MSG_HOW_MANY_TILES_ADD_PREFIX = "How many tiles do you want to add (1..";
const char* MSG_STEP_CHOOSE_HAND_TILE_PREFIX = "Choose hand tile index to add (";
const char* MSG_STEP_CHOOSE_HAND_TILE_MIDDLE = "/";
const char* MSG_INSERT_POSITION_PREFIX = "Insert position index (0..";
const char* MSG_INVALID_INSERT_POSITION = "Invalid insert position.";
const char* MSG_FAILED_REMOVE_TILE_FROM_HAND = "Failed to remove tile from hand.";
const char* MSG_FAILED_INSERT_TILE_IN_COMB = "Failed to insert tile in combination.";
const char* MSG_INVALID_AFTER_ADDING_CANCELLED = "Invalid after adding tiles. Move cancelled.";
const char* MSG_TILES_ADDED_SUCCESS = "Tiles added successfully.";

const char* MSG_ERROR_WRONG_TILES_COUNT_PREFIX = "Error: wrong tiles count: ";

const char* MSG_TABLE_EMPTY_NOTHING_TO_STEAL = "Table is empty. Nothing to steal.";
const char* MSG_STEAL_TITLE_SUFFIX = " - Steal & use a tile (same turn)";
const char* MSG_SOURCE_COMBINATION_TILES = "Source combination tiles:";
const char* MSG_CHOOSE_TILE_INDEX_TO_STEAL_PREFIX = "Choose tile index to steal (0..";
const char* MSG_INVALID_TILE_INDEX = "Invalid tile index.";
const char* MSG_FAILED_TO_STEAL = "Failed to steal.";
const char* MSG_CANT_STEAL_SOURCE_INVALID = "You can't steal that tile: source combination becomes invalid (and can't be split into valid parts).";
const char* MSG_STOLEN_TILE_PREFIX = "Stolen tile: ";

const char* MSG_USE_STOLEN_TILE_BY = "Use stolen tile by:";
const char* MSG_USE_STOLEN_OPTION_NEW = "1) Create a NEW combination (using your hand + stolen tile)";
const char* MSG_USE_STOLEN_OPTION_ADD = "2) Add it to an EXISTING table combination";

const char* MSG_CANT_CREATE_NEW_WITH_STOLEN = "Can't create a new combination with the stolen tile.";
const char* MSG_ACCORDING_TO_RULES_MUST_ADD_EXISTING = "According to rules, you must add it to an existing table combination (if possible).";

const char* MSG_YOU_CHOSE_STEAL_JOKER = "You chose to steal a JOKER.";
const char* MSG_STEAL_JOKER_MUST_REPLACE = "To steal it, you must REPLACE it with a tile from your hand so the combination stays valid.";
const char* MSG_ENTER_HAND_INDEX_REPLACE_JOKER = "Enter hand index to place instead of the Joker (-1 to cancel): ";
const char* MSG_CANCELLED = "Cancelled.";
const char* MSG_CANT_REPLACE_JOKER_WITH_JOKER = "You can't replace a Joker with another Joker.";
const char* MSG_REPLACEMENT_MAKES_INVALID = "Replacement makes the source combination invalid. Can't steal this Joker that way.";
const char* MSG_JOKER_REPLACED_SUCCESS_PREFIX = "Joker replaced successfully. You stole: ";

const char* MSG_PLAY_NEW_TITLE_SUFFIX = " - Play a NEW combination";

const char* MSG_REARRANGE_HEADER = "=== REARRANGE MODE (multiple actions in one turn) ===";
const char* MSG_REARRANGE_DESCRIPTION = "You can do multiple actions. At the end, the table must be fully valid.";
const char* MSG_TEMP_TABLE = "----- TEMP TABLE -----";
const char* MSG_TEMP_HAND_PREFIX = "----- TEMP HAND (Player ";
const char* MSG_TEMP_HAND_SUFFIX = ") -----";

const char* MSG_REARRANGE_MENU_PLAY_NEW = "1) Play a NEW combination (from hand)";
const char* MSG_REARRANGE_MENU_EXTEND = "2) Add tile(s) to an existing combination (from hand)";
const char* MSG_REARRANGE_MENU_STEAL = "3) Steal a tile and use it (same action)";
const char* MSG_REARRANGE_MENU_FINISH = "4) Finish rearrange (commit if valid)";
const char* MSG_REARRANGE_MENU_CANCEL = "5) Cancel rearrange (rollback)";

const char* MSG_REARRANGE_INVALID_INPUT_CANCELLED = "Invalid input. Rearrange cancelled.";
const char* MSG_REARRANGE_CANCELLED_ROLLBACK = "Rearrange cancelled. Rollback.";
const char* MSG_REARRANGE_DID_NOT_CHANGE = "You didn't change anything. Rearrange not applied.";
const char* MSG_REARRANGE_FINAL_TABLE_INVALID = "Final table is NOT valid. Rearrange cancelled (rollback).";
const char* MSG_REARRANGE_COMMITTED = "Rearrange committed successfully!";
const char* MSG_ACTION_FAILED_TRY_AGAIN = "This action failed. You can try another action, or finish/cancel.";

const char* MSG_HAND_LABEL_SUFFIX = " hand:";

const char* MSG_HOW_MANY_FROM_HAND_WITH_STOLEN_PREFIX = "How many tiles from your hand to use with the stolen tile? (0..";
const char* MSG_TOO_FEW_TILES_COMBINATION = "Too few tiles. A combination must be at least 3.";
const char* MSG_HAND_INDEXES_SUFFIX = " hand indexes: ";
const char* MSG_INVALID_HAND_INDEXES = "Invalid hand indexes.";
const char* MSG_NOT_VALID_COMBINATION = "Not a valid combination.";
const char* MSG_NEW_COMBINATION_CREATED = "New combination created successfully.";

const char* MSG_CHOOSE_TARGET_TABLE_INDEX_PREFIX = "Choose target table combination index (0..";
const char* MSG_CHOOSE_SOURCE_TABLE_INDEX_PREFIX = "Choose source table combination index (0..";
const char* MSG_TARGET_COMBINATION_IS = "Target combination is:";
const char* MSG_INSERT_FAILED = "Insert failed.";
const char* MSG_INVALID_AFTER_INSERTING_CANCELLED = "Invalid after inserting. Move cancelled.";
const char* MSG_STOLEN_TILE_ADDED_SUCCESS = "Stolen tile added to existing combination successfully.";

// ANSI COLORS
const char* ANSI_COLOR_ORANGE = "\033[93m";
const char* ANSI_COLOR_RED = "\033[91m";
const char* ANSI_COLOR_BLUE = "\033[94m";
const char* ANSI_COLOR_BLACK = "\033[90m";

const char* ANSI_COLOR_GREEN = "\033[92m";
const char* ANSI_RESET = "\033[0m";

// ------------------------------------------------ Classes ------------------------------------------------

enum Color { ORANGE, RED, BLUE, BLACK };

struct Tile {
    int value;
    Color color;
};

struct Player {
    Tile hand[HAND_CAPACITY];
    int currentHandSize;
    bool hasOpened;
};

struct Combination {
    Tile tiles[COMBINATION_CAPACITY];
    int size;
};

struct Table {
    Combination combinations[MAX_COMBINATIONS];
    int combinationsCount;
};

// ------------------------------------------------ Functions Declarations ------------------------------------------------

// 1) Input / Validation

void enterValidPlayersCount(int& playersCount);
bool readInt(int& x);
bool readChosenIndexes(int* chosenIndexes, int chosenCount, int handSize);

// 2) Printing helpers

const char* ansiColor(Color c);
const char* ansiReset();
char colorToChar(Color color);
void printTile(const Tile& tile);
void printHand(const Player& player);
void printHandWithIndexes(const Player& player);
void printCombinationWithIndexes(const Combination& combination);
void printTable(const Table& table);
void printAllHands(const Player* players, const int playersCount);

// 3) Tiles / Pile

void createTiles(Tile* pile);
void shuffleTiles(Tile* pile, int pileSize = PILE_SIZE);
bool drawFromPile(Tile* pile, int& pileSize, Tile* tile);
bool drawOneTile(Player& player, Tile* pile, int& pileSize);

// 4) Player hand operations

void addToHand(Player* player, const Tile& tile);
void addToHandEnd(Player& player, const Tile& tile);
bool removeFromHand(Player& player, int index);
void seedTilesToPlayers(Player* players, int playersCount, Tile* pile, int& pileSize);
void buildChosenTiles(const Player& player, const int* chosenIndexes, int chosenCount, Tile* chosenTiles);
void sortIndexesDesc(int* array, int size);
void removeChosenFromHand(Player& player, int* chosenIndexes, int chosenCount);

// 5) Table operations

void addCombinationToTable(Table& table, const Tile* tiles, int size);
void removeLastCombinations(Table& table, int count);
void removeEmptyCombinations(Table& table);
bool isTableValid(const Table& table);

// 6) Combination operations

bool removeTileFromCombination(Combination& combination, int index, Tile& removedTile);
bool insertTileInCombinationAt(Combination& combination, const Tile& tile, int index);
bool splitOneCombinationIntoTwo(const Combination& src, int splitPos, Combination& left, Combination& right);
bool trySplitAfterRemoval(Table& table, int sourceIndex);
void rollbackCombination(Combination& target, const Combination& backup, Player& player, Tile* returned, int count);

// 7) Rules / Validation

bool isJoker(const Tile& tile);
bool isValidGroup(const Tile* tiles, int size);
void sortArrayFunc(int* array, int size);
bool isValidRun(const Tile* tiles, int size);
bool isValidCombination(const Tile* tiles, int size);

// 8) Points / Scoring

int tilePoints(const Tile& tile);
int handPoints(const Player& player);
int combinationPoints(const Tile* tiles, int size);
int findWinnerByLowestPoints(Player* players, int playersCount);

// 9) Turn actions / Game moves

/*!!!!!!!!!!!*/ bool tryPlayCombinationTurn(Player& player, Table& table, int playerNumber); /*!!!!!!!!!!!*/
bool tryPlayNewCombination(Player& player, Table& table, int playerNumber);
bool tryAddTileToExistingCombination(Player& player, Table& table, int playerNumber);
/*!!!!!!!!!!!*/ bool tryStealAndUseTile(Player& player, Table& table, int playerNumber); /*!!!!!!!!!!!*/
bool tryCreateNewCombinationWithExtraTile(Player& player, Table& table, const Tile& extraTile);
bool tryAddExtraTileToExistingCombination(Table& table, const Tile& extraTile);
bool tryReplaceJokerAndSteal(Player& player, Combination& source, int jokerIndex, Tile& stolenJoker, Tile& usedHandTile, bool& tookFromHand);
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

    cout << MSG_REMAINING_TILES_IN_PILE << pileSize << endl;

    Table table;
    table.combinationsCount = 0;

    bool gameOver = false;
    int roundCounter = 1;

    while (!gameOver) 
    {
        cout << endl << ANSI_COLOR_GREEN << MSG_ROUND_PREFIX << roundCounter++ << MSG_ROUND_SUFFIX << ANSI_RESET << endl;

        for (int i = 0; i < playersCount; i++) {
            cout << endl << MSG_SEPARATOR_LINE << endl;
            printTable(table);

            bool played = tryPlayCombinationTurn(players[i], table, i + 1);

            cout << endl << MSG_EXTEND_TITLE_PREFIX << (i + 1) << MSG_PLAYER_NEW_HAND_SUFFIX << endl;
            printHandWithIndexes(players[i]);

            if (players[i].currentHandSize == 0) {
                cout << endl << MSG_EXTEND_TITLE_PREFIX << (i + 1) << MSG_PLAYER_WINS_SUFFIX << endl;
                gameOver = true;
                break;
            }

            if (!played) {
                cout << MSG_NO_COMB_PLAYED_DRAW << endl;

                if (!drawOneTile(players[i], pile, pileSize)) 
                {
                    cout << endl << MSG_PILE_EMPTY_ENDS << endl;
                    int winnerIndex = findWinnerByLowestPoints(players, playersCount);

                    cout << MSG_WINNER_PREFIX << (winnerIndex + 1) << MSG_WINNER_WITH << handPoints(players[winnerIndex]) << MSG_POINTS_SUFFIX << endl;

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
        cout << MSG_ENTER_PLAYERS;

        if (!readInt(playersCount) || playersCount < MIN_PLAYERS || playersCount > MAX_PLAYERS)
        {
            cout << MSG_TRY_VALID_PLAYERS << endl;
            continue;
        }

        break;
    }
}

void createTiles(Tile* pile){
    const int COLORS_COUNT = COLOR_COUNT;
    const int VALUES_COUNT = MAX_TILE_VALUE;
    
    int index = 0;

    for (int k = 0; k < DUPLICATE_SETS; k++)
    {
        for (int i = 0; i < COLORS_COUNT; i++)
        {
            for (int j = MIN_TILE_VALUE; j <= VALUES_COUNT; j++)
            {
                pile[index].color = (Color)i;
                pile[index].value = j;
                index++;
            }
        }
    }

    // jokers
    pile[index++] = Tile{ JOKER_VALUE, RED };
    pile[index++] = Tile{ JOKER_VALUE, BLACK };

    if (index != PILE_SIZE) {
        cout << MSG_ERROR_WRONG_TILES_COUNT_PREFIX << index << endl;
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
    if (player->currentHandSize >= HAND_CAPACITY)
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
    cout << ansiColor(tile.color);

    if (tile.value == JOKER_VALUE)
    {
        cout << "[J-" << colorToChar(tile.color) << "]";
    }
    else 
    {
        cout << "[" << tile.value << "-" << colorToChar(tile.color) << "]";
    }

    cout << ansiReset();
}

void printHand(const Player& player) {
    for (int i = 0; i < player.currentHandSize; i++) {
        printTile(player.hand[i]);
        cout << MSG_SPACE;
    }
    cout << endl;
}

void printAllHands(const Player* players, const int playersCount) {
    if (players == nullptr)
    {
        return;
    }

    for (int i = 0; i < playersCount; i++) {
        cout << MSG_EXTEND_TITLE_PREFIX << (i + 1) << MSG_HAND_LABEL_SUFFIX << endl;
        printHand(players[i]);
    }
}

bool isJoker(const Tile& tile) {
    if (tile.value == JOKER_VALUE)
    {
        return true;
    }
    return false;
}

bool isValidGroup(const Tile* tiles, int size) {
    if (size != MIN_COMBINATION_SIZE && size != MAX_GROUP_SIZE)
    {
        return false;
    }

    const int COLORS_COUNT = COLOR_COUNT;
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
    if (size < MIN_COMBINATION_SIZE)
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

    int values[COMBINATION_CAPACITY];
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

    int maxExtend = (minV - 1) + (MAX_TILE_VALUE - maxV);

    return extra <= maxExtend;
}

bool isValidCombination(const Tile* tiles, int size) {
    return isValidGroup(tiles, size) || isValidRun(tiles, size);
}

void printHandWithIndexes(const Player& player) {
    for (int i = 0; i < player.currentHandSize; i++) 
    {
        cout << i << MSG_COLON;
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
    if (table.combinationsCount >= MAX_COMBINATIONS)
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
    cout << endl << MSG_TABLE_HEADER << endl;

    if (table.combinationsCount == 0) 
    {
        cout << MSG_TABLE_EMPTY_PRINT << endl;
        return;
    }

    for (int i = 0; i < table.combinationsCount; i++) 
    {
        cout << i << MSG_CLOSING_BRACKET;

        for (int j = 0; j < table.combinations[i].size; j++) 
        {
            printTile(table.combinations[i].tiles[j]);
            cout << MSG_SPACE;
        }

        cout << endl;
    }
}

bool readChosenIndexes(int* chosenIndexes, int chosenCount, int handSize) {
    for (int i = 0; i < chosenCount; i++) 
    {
        if (!readInt(chosenIndexes[i]))
        {
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
    cout << endl << MSG_EXTEND_TITLE_PREFIX << playerNumber << MSG_PLAYER_TURN_SUFFIX << endl;

    cout << MSG_YOUR_HAND << endl;
    printHandWithIndexes(player);

    if (player.hasOpened)
    {
        cout << MSG_MENU_PLAY_NEW << endl;
        cout << MSG_MENU_EXTEND_EXISTING << endl;
        cout << MSG_MENU_STEAL_AND_USE << endl;
        cout << MSG_MENU_REARRANGE << endl;
        cout << MSG_MENU_DRAW_END << endl;
        cout << MSG_CHOOSE_PROMPT;

        int choice;
        if (!readInt(choice)) 
        {
            return false;
        }      

        if (choice == TURN_DRAW_END)
        {
            return false;
        }
        if (choice == TURN_REARRANGE)
        {
            return tryRearrangeTurn(player, table, playerNumber);
        }
        if (choice == TURN_STEAL_AND_USE)
        {
            return tryStealAndUseTile(player, table, playerNumber);
        }
        if (choice == TURN_EXTEND_EXISTING)
        {
            return tryAddTileToExistingCombination(player, table, playerNumber);
        }
        if (choice == TURN_PLAY_NEW)
        {
            return tryPlayNewCombination(player, table, playerNumber);
        }

        cout << MSG_INVALID_CHOICE << endl;
        return false;
    }

    cout << MSG_OPENING_REQUIRED_PREFIX << OPENING_POINTS << MSG_OPENING_REQUIRED_SUFFIX << endl;

    int turnSum = 0;

    int startCombinationsCount = table.combinationsCount;

    Tile takenTiles[HAND_CAPACITY];
    int takenCount = 0;

    while (true)
    {
        cout << endl << MSG_OPENING_POINTS_STATUS_PREFIX << turnSum << MSG_SLASH << OPENING_POINTS << endl;
        cout << MSG_OPEN_MENU_ADD_COMBINATION << endl;
        cout << MSG_OPEN_MENU_DRAW_GIVE_UP << endl;
        cout << MSG_CHOOSE_PROMPT;

        int choice;
        if (!readInt(choice))
        {
            removeLastCombinations(table, table.combinationsCount - startCombinationsCount);

            for (int i = 0; i < takenCount; i++)
            {
                addToHandEnd(player, takenTiles[i]);
            }

            return false;
        }

        if (choice == OPEN_TURN_DRAW_GIVE_UP)
        {
            removeLastCombinations(table, table.combinationsCount - startCombinationsCount);

            for (int i = 0; i < takenCount; i++) 
            {
                addToHandEnd(player, takenTiles[i]);
            }
            
            return false;
        }

        if (choice != OPEN_TURN_ADD_COMBINATION)
        {
            cout << MSG_INVALID_CHOICE << endl;
            continue;
        }

        cout << endl << MSG_CHOOSE_TILES_TO_FORM << endl;
        printHandWithIndexes(player);

        cout << MSG_HOW_MANY_TILES_IN_COMB << endl;
        int chosenCount;
        if (!readInt(chosenCount))
        {
            return false;
        }

        if (chosenCount < 3 || chosenCount > HAND_SIZE || chosenCount > player.currentHandSize) 
        {
            cout << MSG_INVALID_COUNT << endl;
            continue;
        }

        int chosenIndexes[HAND_SIZE];
        Tile chosenTiles[HAND_SIZE];

        cout << MSG_ENTER_INDEXES_PREFIX << chosenCount << MSG_ENTER_INDEXES_SUFFIX;

        if (!readChosenIndexes(chosenIndexes, chosenCount, player.currentHandSize)) 
        {
            cout << MSG_INVALID_INDEXES << endl;
            continue;
        }

        buildChosenTiles(player, chosenIndexes, chosenCount, chosenTiles);

        if (!isValidCombination(chosenTiles, chosenCount)) 
        {
            cout << MSG_INVALID_COMBINATION << endl;
            continue;
        }

        int pts = combinationPoints(chosenTiles, chosenCount);
        cout << MSG_THIS_COMBINATION_POINTS_PREFIX << pts << endl;

        for (int i = 0; i < chosenCount; i++) 
        {
            if (takenCount < HAND_CAPACITY)
            {
                takenTiles[takenCount++] = chosenTiles[i];
            }
        }

        addCombinationToTable(table, chosenTiles, chosenCount);
        removeChosenFromHand(player, chosenIndexes, chosenCount);

        turnSum += pts;

        if (turnSum >= OPENING_POINTS)
        {
            player.hasOpened = true;
            cout << MSG_OPENED_SUCCESS_PREFIX << OPENING_POINTS << MSG_OPENED_SUCCESS_SUFFIX << endl;
            return true;
        }
    }
}

int tilePoints(const Tile& tile) {
    if (isJoker(tile)) 
    {
        return OPENING_POINTS;
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

    cout << MSG_YOU_DREW_PREFIX;
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
    if (player.currentHandSize >= HAND_CAPACITY)
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
        cout << MSG_TABLE_EMPTY_NO_COMB_TO_ADD << endl;
        return false;
    }

    cout << endl << MSG_EXTEND_TITLE_PREFIX << playerNumber << MSG_EXTEND_TITLE_SUFFIX << endl;

    cout << MSG_CHOOSE_TABLE_INDEX_PREFIX << (table.combinationsCount - 1) << MSG_CHOOSE_TABLE_INDEX_SUFFIX;

    int tableIndex;
    if (!readInt(tableIndex))
    {
        return false;
    }

    if (tableIndex < 0 || tableIndex >= table.combinationsCount)
    {
        cout << MSG_INVALID_TABLE_INDEX << endl;
        return false;
    }

    if (player.currentHandSize <= 0)
    {
        cout << MSG_NO_TILES_IN_HAND << endl;
        return false;
    }

    cout << MSG_HOW_MANY_TILES_ADD_PREFIX << player.currentHandSize << MSG_CHOOSE_TABLE_INDEX_SUFFIX;

    int addCount;
    if (!readInt(addCount))
    {
        return false;
    }

    if (addCount < 1 || addCount > player.currentHandSize)
    {
        cout << MSG_INVALID_COUNT << endl;
        return false;
    }

    Combination oldCombination = table.combinations[tableIndex];

    Tile removedTiles[HAND_CAPACITY];
    int removedCount = 0;

    for (int step = 0; step < addCount; step++)
    {
        cout << endl << MSG_YOUR_HAND << endl;
        printHandWithIndexes(player);

        cout << MSG_STEP_CHOOSE_HAND_TILE_PREFIX << (step + 1) << MSG_STEP_CHOOSE_HAND_TILE_MIDDLE << addCount << MSG_CHOOSE_TABLE_INDEX_SUFFIX;

        int handIndex;
        if (!readInt(handIndex))
        {
            table.combinations[tableIndex] = oldCombination;
            for (int i = 0; i < removedCount; i++)
            {
                addToHandEnd(player, removedTiles[i]);
            }
            return false;
        }

        if (handIndex < 0 || handIndex >= player.currentHandSize)
        {
            cout << MSG_INVALID_HAND_INDEX << endl;

            table.combinations[tableIndex] = oldCombination;
            for (int i = 0; i < removedCount; i++) 
            {
                addToHandEnd(player, removedTiles[i]);
            }

            return false;
        }

        cout << MSG_INSERT_POSITION_PREFIX << table.combinations[tableIndex].size << MSG_CHOOSE_TABLE_INDEX_SUFFIX;

        int insertPos;
        if (!readInt(insertPos))
        {
            table.combinations[tableIndex] = oldCombination;
            for (int i = 0; i < removedCount; i++)
            {
                addToHandEnd(player, removedTiles[i]);
            }
            return false;
        }

        if (insertPos < 0 || insertPos > table.combinations[tableIndex].size)
        {
            cout << MSG_INVALID_INSERT_POSITION << endl;

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
            cout << MSG_FAILED_REMOVE_TILE_FROM_HAND << endl;

            table.combinations[tableIndex] = oldCombination;

            for (int i = 0; i < removedCount; i++) 
            {
                addToHandEnd(player, removedTiles[i]);
            }
            
            return false;
        }

        if (removedCount < HAND_CAPACITY)
        {
            removedTiles[removedCount++] = tileToAdd;
        }

        if (!insertTileInCombinationAt(table.combinations[tableIndex], tileToAdd, insertPos))
        {
            cout << MSG_FAILED_INSERT_TILE_IN_COMB << endl;

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
        cout << MSG_INVALID_AFTER_ADDING_CANCELLED << endl;

        table.combinations[tableIndex] = oldCombination;

        for (int i = 0; i < removedCount; i++) 
        {
            addToHandEnd(player, removedTiles[i]);
        }
        
        return false;
    }

    cout << MSG_TILES_ADDED_SUCCESS << endl;
    return true;
}

void printCombinationWithIndexes(const Combination& combination) {
    for (int i = 0; i < combination.size; i++) 
    {
        cout << i << MSG_COLON;
        printTile(combination.tiles[i]);
        cout << MSG_SPACE;
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
    if (combination.size >= COMBINATION_CAPACITY)
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
    cout << MSG_HOW_MANY_FROM_HAND_WITH_STOLEN_PREFIX << player.currentHandSize << MSG_CHOOSE_TABLE_INDEX_SUFFIX;

    int fromHandCount;
    if (!readInt(fromHandCount))
    {
        return false;
    }

    if (fromHandCount < 0 || fromHandCount > player.currentHandSize) 
    {
        cout << MSG_INVALID_COUNT << endl;
        return false;
    }

    int newSize = fromHandCount + 1;

    if (newSize < MIN_COMBINATION_SIZE)
    {
        cout << MSG_TOO_FEW_TILES_COMBINATION << endl;
        return false;
    }

    int chosenIndexes[HAND_CAPACITY];
    Tile newTiles[HAND_CAPACITY];

    if (fromHandCount > 0) 
    {
        cout << MSG_ENTER_INDEXES_PREFIX << fromHandCount << MSG_HAND_INDEXES_SUFFIX;
        if (!readChosenIndexes(chosenIndexes, fromHandCount, player.currentHandSize)) 
        {
            cout << MSG_INVALID_HAND_INDEXES << endl;
            return false;
        }

        buildChosenTiles(player, chosenIndexes, fromHandCount, newTiles);
    }

    newTiles[fromHandCount] = extraTile;

    if (!isValidCombination(newTiles, newSize))
    {
        cout << MSG_NOT_VALID_COMBINATION << endl;
        return false;
    }

    if (fromHandCount > 0) 
    {
        removeChosenFromHand(player, chosenIndexes, fromHandCount);
    }

    addCombinationToTable(table, newTiles, newSize);

    cout << MSG_NEW_COMBINATION_CREATED << endl;

    return true;
}

bool tryAddExtraTileToExistingCombination(Table& table, const Tile& extraTile) {
    if (table.combinationsCount <= 0) 
    {
        return false;
    }

    cout << MSG_CHOOSE_TARGET_TABLE_INDEX_PREFIX << (table.combinationsCount - 1) << MSG_CHOOSE_TABLE_INDEX_SUFFIX;

    int targetIndex;
    if (!readInt(targetIndex))
    {
        return false;
    }

    if (targetIndex < 0 || targetIndex >= table.combinationsCount) 
    {
        cout << MSG_INVALID_TABLE_INDEX << endl;
        return false;
    }

    cout << MSG_TARGET_COMBINATION_IS << endl;
    printCombinationWithIndexes(table.combinations[targetIndex]);

    cout << MSG_INSERT_POSITION_PREFIX << table.combinations[targetIndex].size << MSG_CHOOSE_TABLE_INDEX_SUFFIX;

    int insertPos;
    if (!readInt(insertPos))
    {
        return false;
    }

    Combination old = table.combinations[targetIndex];

    if (!insertTileInCombinationAt(table.combinations[targetIndex], extraTile, insertPos))
    {
        cout << MSG_INSERT_FAILED << endl;
        table.combinations[targetIndex] = old;
        return false;
    }

    if (!isValidCombination(table.combinations[targetIndex].tiles, table.combinations[targetIndex].size))
    {
        cout << MSG_INVALID_AFTER_INSERTING_CANCELLED << endl;
        table.combinations[targetIndex] = old;
        return false;
    }

    cout << MSG_STOLEN_TILE_ADDED_SUCCESS << endl;

    return true;
}

bool tryStealAndUseTile(Player& player, Table& table, int playerNumber) {
    if (table.combinationsCount <= 0)
    {
        cout << MSG_TABLE_EMPTY_NOTHING_TO_STEAL << endl;
        return false;
    }

    cout << endl << MSG_EXTEND_TITLE_PREFIX << playerNumber << MSG_STEAL_TITLE_SUFFIX << endl;
    printTable(table);

    cout << MSG_CHOOSE_SOURCE_TABLE_INDEX_PREFIX << (table.combinationsCount - 1) << MSG_CHOOSE_TABLE_INDEX_SUFFIX;

    int sourceIndex;
    if (!readInt(sourceIndex))
    {
        return false;
    }

    if (sourceIndex < 0 || sourceIndex >= table.combinationsCount)
    {
        cout << MSG_INVALID_TABLE_INDEX << endl;
        return false;
    }

    cout << MSG_SOURCE_COMBINATION_TILES << endl;
    printCombinationWithIndexes(table.combinations[sourceIndex]);

    cout << MSG_CHOOSE_TILE_INDEX_TO_STEAL_PREFIX << (table.combinations[sourceIndex].size - 1) << MSG_CHOOSE_TABLE_INDEX_SUFFIX;

    int tileIndex;
    if (!readInt(tileIndex))
    {
        return false;
    }

    if (tileIndex < 0 || tileIndex >= table.combinations[sourceIndex].size)
    {
        cout << MSG_INVALID_TILE_INDEX << endl;
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
            cout << MSG_FAILED_TO_STEAL << endl;
            table.combinations[sourceIndex] = oldSource;
            return false;
        }

        if (!isValidCombination(table.combinations[sourceIndex].tiles, table.combinations[sourceIndex].size))
        {
            if (!trySplitAfterRemoval(table, sourceIndex))
            {
                cout << MSG_CANT_STEAL_SOURCE_INVALID << endl;
                table.combinations[sourceIndex] = oldSource;
                return false;
            }
        }
    }

    cout << MSG_STOLEN_TILE_PREFIX;
    printTile(stolen);
    cout << endl;

    cout << MSG_USE_STOLEN_TILE_BY << endl;
    cout << MSG_USE_STOLEN_OPTION_NEW << endl;
    cout << MSG_USE_STOLEN_OPTION_ADD << endl;
    cout << MSG_CHOOSE_PROMPT;

    int useChoice;
    if (!readInt(useChoice))
    {
        table.combinations[sourceIndex] = oldSource;
        return false;
    }

    bool success = false;

    if (useChoice == STEAL_USE_NEW_COMBINATION)
    {
        cout << MSG_YOUR_HAND << endl;
        printHandWithIndexes(player);

        success = tryCreateNewCombinationWithExtraTile(player, table, stolen);

        if (!success)
        {
            cout << MSG_CANT_CREATE_NEW_WITH_STOLEN << endl;
            cout << MSG_ACCORDING_TO_RULES_MUST_ADD_EXISTING << endl;

            success = tryAddExtraTileToExistingCombination(table, stolen);
        }
    }
    else if (useChoice == STEAL_USE_ADD_TO_EXISTING)
    {
        success = tryAddExtraTileToExistingCombination(table, stolen);
    }
    else
    {
        cout << MSG_INVALID_CHOICE << endl;
        success = false;
    }

    if (!success)
    {
        table.combinations[sourceIndex] = oldSource;

        if (tookFromHand)
        {
            addToHandEnd(player, usedHandTile);
        }

        cout << MSG_STEAL_CANCEL_ROLLBACK << endl;
        return false;
    }

    cout << MSG_STEAL_SUCCESS << endl;
    return true;
}

bool tryReplaceJokerAndSteal(Player& player, Combination& source, int jokerIndex, Tile& stolenJoker, Tile& usedHandTile, bool& tookFromHand)
{
    tookFromHand = false;
    stolenJoker = source.tiles[jokerIndex];

    cout << MSG_YOU_CHOSE_STEAL_JOKER << endl;
    cout << MSG_STEAL_JOKER_MUST_REPLACE << endl;

    cout << MSG_YOUR_HAND << endl;
    printHandWithIndexes(player);

    cout << MSG_ENTER_HAND_INDEX_REPLACE_JOKER;

    int handIndex;
    if (!readInt(handIndex))
    {
        return false;
    }

    if (handIndex == CANCEL_COMMAND)
    {
        cout << MSG_CANCELLED << endl;
        return false;
    }

    if (handIndex < 0 || handIndex >= player.currentHandSize)
    {
        cout << MSG_INVALID_HAND_INDEX << endl;
        return false;
    }

    Tile candidate = player.hand[handIndex];

    if (isJoker(candidate))
    {
        cout << MSG_CANT_REPLACE_JOKER_WITH_JOKER << endl;
        return false;
    }

    Tile oldTile = source.tiles[jokerIndex];
    source.tiles[jokerIndex] = candidate;

    if (!isValidCombination(source.tiles, source.size))
    {
        source.tiles[jokerIndex] = oldTile;
        cout << MSG_REPLACEMENT_MAKES_INVALID << endl;
        return false;
    }

    if (!removeFromHand(player, handIndex))
    {
        source.tiles[jokerIndex] = oldTile;
        cout << MSG_FAILED_REMOVE_TILE_FROM_HAND << endl;
        return false;
    }

    usedHandTile = candidate;
    tookFromHand = true;

    cout << MSG_JOKER_REPLACED_SUCCESS_PREFIX;
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

    if (src.size < MIN_SPLITTABLE_COMBINATION_SIZE)
    {
        return false;
    }

    for (int splitPos = MIN_PART_AFTER_SPLIT; splitPos <= src.size - MIN_PART_AFTER_SPLIT; splitPos++)
    {
        Combination left, right;
        if (!splitOneCombinationIntoTwo(src, splitPos, left, right))
        {
            continue;
        }

        if (isValidCombination(left.tiles, left.size) && isValidCombination(right.tiles, right.size))
        {
            if (table.combinationsCount >= MAX_COMBINATIONS)
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
    cout << endl << MSG_EXTEND_TITLE_PREFIX << playerNumber << MSG_PLAY_NEW_TITLE_SUFFIX << endl;
    cout << MSG_CHOOSE_TILES_TO_FORM << endl;
    printHandWithIndexes(player);

    cout << MSG_HOW_MANY_TILES_IN_COMB << endl;

    int chosenCount;
    if (!readInt(chosenCount))
    {
        return false;
    }

    if (chosenCount < 3 || chosenCount > HAND_SIZE || chosenCount > player.currentHandSize)
    {
        cout << MSG_INVALID_COUNT << endl;
        return false;
    }

    int chosenIndexes[HAND_SIZE];
    Tile chosenTiles[HAND_SIZE];

    cout << MSG_ENTER_INDEXES_PREFIX << chosenCount << MSG_ENTER_INDEXES_SUFFIX;

    if (!readChosenIndexes(chosenIndexes, chosenCount, player.currentHandSize))
    {
        cout << MSG_INVALID_INDEXES << endl;
        return false;
    }

    buildChosenTiles(player, chosenIndexes, chosenCount, chosenTiles);

    if (!isValidCombination(chosenTiles, chosenCount))
    {
        cout << MSG_INVALID_COMBINATION << endl;
        return false;
    }

    addCombinationToTable(table, chosenTiles, chosenCount);
    removeChosenFromHand(player, chosenIndexes, chosenCount);

    cout << MSG_COMBINATION_ADDED << endl;
    return true;
}

bool tryRearrangeTurn(Player& player, Table& table, int playerNumber)
{
    cout << endl << MSG_REARRANGE_HEADER << endl;
    cout << MSG_REARRANGE_DESCRIPTION << endl;

    Player tempPlayer = player;
    Table tempTable = table;

    bool didSomething = false;

    while (true)
    {
        cout << endl << MSG_TEMP_TABLE << endl;
        printTable(tempTable);

        cout << endl << MSG_TEMP_HAND_PREFIX << playerNumber << MSG_TEMP_HAND_SUFFIX << endl;
        printHandWithIndexes(tempPlayer);

        cout << endl;
        cout << MSG_REARRANGE_MENU_PLAY_NEW << endl;
        cout << MSG_REARRANGE_MENU_EXTEND << endl;
        cout << MSG_REARRANGE_MENU_STEAL << endl;
        cout << MSG_REARRANGE_MENU_FINISH << endl;
        cout << MSG_REARRANGE_MENU_CANCEL << endl;
        cout << MSG_CHOOSE_PROMPT;

        int choice;
        if (!readInt(choice))
        {
            cout << MSG_REARRANGE_INVALID_INPUT_CANCELLED << endl;
            return false;
        }

        if (choice == REARRANGE_CANCEL_ROLLBACK)
        {
            cout << MSG_REARRANGE_CANCELLED_ROLLBACK << endl;
            return false;
        }

        if (choice == REARRANGE_FINISH_COMMIT)
        {
            removeEmptyCombinations(tempTable);

            if (!didSomething)
            {
                cout << MSG_REARRANGE_DID_NOT_CHANGE << endl;
                return false;
            }

            if (!isTableValid(tempTable))
            {
                cout << MSG_REARRANGE_FINAL_TABLE_INVALID << endl;
                return false;
            }

            player = tempPlayer;
            table = tempTable;

            cout << MSG_REARRANGE_COMMITTED << endl;
            return true;
        }

        bool stepSuccess = false;

        if (choice == REARRANGE_PLAY_NEW)
        {
            stepSuccess = tryPlayNewCombination(tempPlayer, tempTable, playerNumber);
        }
        else if (choice == REARRANGE_EXTEND_EXISTING)
        {
            stepSuccess = tryAddTileToExistingCombination(tempPlayer, tempTable, playerNumber);
        }
        else if (choice == REARRANGE_STEAL_AND_USE)
        {
            stepSuccess = tryStealAndUseTile(tempPlayer, tempTable, playerNumber);
        }
        else
        {
            cout << MSG_INVALID_CHOICE << endl;
            stepSuccess = false;
        }

        if (stepSuccess)
        {
            didSomething = true;
            removeEmptyCombinations(tempTable);
        }
        else
        {
            cout << MSG_ACTION_FAILED_TRY_AGAIN << endl;
        }
    }
}

const char* ansiColor(Color c)
{
    switch (c)
    {
        case ORANGE: return ANSI_COLOR_ORANGE;
        case RED:    return ANSI_COLOR_RED;
        case BLUE:   return ANSI_COLOR_BLUE;
        case BLACK:  return ANSI_COLOR_BLACK;
    }
    return ANSI_RESET;
}

const char* ansiReset()
{
    return ANSI_RESET;
}

bool readInt(int& x)
{
    cin >> x;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(CIN_IGNORE_LIMIT, '\n');
        cout << MSG_INVALID_INPUT << endl;
        return false;
    }

    return true;
}

void rollbackCombination(Combination& target, const Combination& backup, Player& player, Tile* returned, int count)
{
    target = backup;
    for (int i = 0; i < count; i++) 
    {
        addToHandEnd(player, returned[i]);
    }  
}