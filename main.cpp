#include <iostream>

bool check_moves();
char check_line(char);
int check_field(bool, int, int, int);
int compute();
int draw_field();
int enter(char);
void game(int);

struct Move {int row, col;};
Move find_move();

const int MIN = -1000;
const int MAX = 1000;

int field_size = 5;
char field[5][5];

char mark;
int mode;
int player;

int main() {
    system("clear");
    
    field_size = 0;
    std::cout << "Enter field size (from 3 (3x3) to 5 (5x5)): ";

    while(field_size == 0) {
        std::cin >> field_size;

        if(!(3 <= field_size <= 5)) {
            std::cout << "Please choose from 3 to 5.\n";
            field_size = 0;
            std::cin.clear();
        }
    }
    
    for(int i = 0; i < field_size; i++)
        for(int j = 0; j < field_size; j++)
            field[i][j] = '.';

    std::cout << "Game mode.\n";
    std::cout << "1. Player VS Computer.\n";
    std::cout << "2. Player VS Player.\n";
    std::cout << "3. Computer VS Computer.\n";
    std::cout << "Enter game mode: ";

    mode = 0;
    while(mode == 0) {
        std::cin >> mode;
        if(!(1 <= mode <= 3)) {
            std::cout << "Please choose from 1 to 3.\n";
            mode = 0;
            std::cin.clear();
        }
        else game(mode);
    }
    return 0;
}

void game(int mode) {
    int result;
    player = 1;
    mark = 'X';
    Move move;

    std::string p[2] = {"Human ", "Computer "};
    if(mode == 2) {
        p[0] = "Player ";
        p[1] = "Player ";
    }
    else if(mode == 3) p[0] = "Computer ";
    
    bool game = true;
    while(game) {
        draw_field();

        if(mode == 2 || (mode == 1 && player == 1)) {
            move.row = -1;
            move.col = -1;
            while(move.row == -1 && move.col == -1) {
                move.row = enter('r');
                move.col = enter('c');
                if(field[move.row][move.col] == '.')
                    field[move.row][move.col] = mark;
                else {
                    std::cout << "Illegal move.\n";
                    move.row = -1;
                    move.col = -1;
                    std::cin.clear();
                }
            }
        }
        else move = find_move();

        field[move.row][move.col] = mark;
        draw_field();
        result = compute();

        if(result == 1) {
            game = false;
            std::cout << p[player-1];
            if(mode != 1) std::cout << player << ' ';
            std::cout << "wins!\n";
        }
        else if(!check_moves()) {
            game = false;
            std::cout << "Tie.\n";
        }
        if(!game) {
            for(int i = 0; i <= field_size; i++)
                for(int j = 0; j <= field_size; j++)
                    field[i][j] = '.';
            char choice;
            std::cout << "Continue? (y/n) ";
            std::cin >> choice;
            if(choice == 'y') game = true;
            else return;

            if(mark == 'X') mark = 'O';
            else mark = 'X';

            if(mode == 3) {
                Move rand_start;
                rand_start.row = rand() % field_size + 1;
                rand_start.col = rand() % field_size + 1;
                field[rand_start.row][rand_start.col] = mark;
            }
        }

        if(mark == 'X') mark = 'O';
        else mark = 'X';
        
        if(player == 1) player = 2;
        else player = 1;
    }
}

int draw_field() {
    system("clear");
    std::cout << "  ";
    for(int i = 0; i < field_size; i++)
        std::cout << i + 1 << ' '; 
    std::cout << '\n';
    for(int i = 0; i < field_size; i++) {
        std::cout << i + 1 << ' ';
        for(int j = 0; j < field_size; j++)
            std::cout << field[i][j] << ' ';
        std::cout << "\n\n";
    }
    return 0;
}

int enter(char place) {
    int var = -1;
    while(var == -1) {
        if(mode == 1) std::cout << "Enter ";
        else std::cout << "Player " << player << " enter ";
        if(place == 'r') std::cout << "row: ";
        else std::cout << "col: ";
        std::cin >> var;
        if(!(1 <= var <= field_size)) {
            if(place == 'r') std::cout << "Row ";
            else std::cout << "Col ";
            std::cout << "must be from 1 to " << field_size << " .\n";
            var = -1;
            std::cin.clear();
        }
    }
    return var - 1;
}

char check_line(char var) {
    bool x;
    if(var == 'r') {
        for(int i = 0; i < field_size; i++) {
            if(field[i][0] == '.') continue;
            x = true;
            for(int j = 1; j < field_size; j++) {
                if(field[i][j] != field[i][0]) {
                    x = false;
                    break;
                }
            }
            if(x) return field[i][0]; 
        }
    }

    else {
        for(int i = 0; i < field_size; i++) {
            if(field[0][i] == '.') continue;
            x = true;
            for(int j = 1; j < field_size; j++) {
                if(field[j][i] != field[0][i]) {
                    x = false;
                    break;
                }
            }
            if(x) return field[0][i]; 
        }
    }
    return ' ';
}

bool check_diagonal(int num) {
    int start;
    if(num == 1) start = 0;
    else start = field_size - 1;

    if(field[0][start] == '.') return false;
    for(int i = 0; i < field_size; i++)
        if(field[i][start + i*num] != field[0][start])
            return false;
    return true;
}

bool check_moves() {
    for(int a = 0; a < field_size; a++)
        for(int b = 0; b < field_size; b++)
            if(field[a][b] == '.')
                return true;
    return false;
}

int compute() {
    char enemy;
    if(mark == 'X') enemy = 'O';
    else enemy = 'X';

    if(check_line('r') == mark) return +10;
    if(check_line('r') == enemy) return -10;
    if(check_line('c') == mark) return +10;
    if(check_line('c') == enemy) return -10;
    if(check_diagonal(1) && field[0][0] == mark) return +10;
    if(check_diagonal(1) && field[0][0] == enemy) return -10;
    if(check_diagonal(-1) && field[0][field_size-1] == mark) return +10;
    if(check_diagonal(-1) && field[0][field_size-1] == enemy) return -10;
    return 0;
}

int check_field(bool better, int depth, int a, int b) {
    int score = compute();
    if(score != 0 || !check_moves()) return score;
    if(field_size > 3 && depth > 3) return 1;

    char enemy;
    if(mark == 'X') enemy = 'O';
    else enemy = 'X';

    char turn;
    if(better) turn = mark;
    else turn = enemy;

    int best;
    if(better) best = MIN;
    else best = MAX;
    for(int row = 0; row < field_size; row++) {
        for(int col = 0; col < field_size; col++) {
            if(field[row][col] == '.') {
                field[row][col] = turn;
                if(better) {
                    best = std::max(best, check_field(false, depth+1, a, b));
                    a = std::max(a, best);
                }
                else {
                    best = std::min(best, check_field(true, depth+1, a, b));
                    b = std::min(b, best);
                }
                field[row][col] = '.';
                if(field_size > 3 && b <= a) {
                    if(better) return best - depth;
                    return best + depth;
                }
            }
        }
    }
    if(field_size > 3) {
        if(better) return best - depth;
        return best + depth;
    }
    else return best;
}

Move find_move() {
    int best = MIN;
    Move move;
    move.row = -1;
    move.col = -1;
    for(int row = 0; row < field_size; row++) {
        for(int col = 0; col < field_size; col++) {
            if(field[row][col] == '.') {
                field[row][col] = mark;
                int value = check_field(false, 0, MIN, MAX);
                draw_field();
                field[row][col] = '.';
                if(value > best) {
                    move.row = row;
                    move.col = col;
                    best = value;
                }
            }
        }
    }
    return move;
}
