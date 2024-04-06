#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

void Usage(const char* file) {
    std::cerr << "Usage: " << file << " input_file.txt [-F]" << std::endl;
    exit(1);
}

void ReadFile(char* argv[], std::vector<int>& grid) {
    std::ifstream input(argv[1]);
    if (!input) {
        std::cerr << "Cannot open " << argv[1] << " to read" << std::endl;
        Usage(argv[0]);
    }

    char digit;
    while (input >> digit) {
        if (!isdigit(digit)) {
            std::cerr << "Invalid input file " << argv[1] << std::endl;
            input.close();

            Usage(argv[0]);
        }

        if (digit - '0' != 0 && digit - '0' != 1) {
            std::cerr << "Invalid input file " << argv[1] << std::endl;
            input.close();

            Usage(argv[0]);
        }

        grid.push_back(digit - '0');
    }

    input.close();

    // Ensure input is a square
    double size = sqrt(grid.size());
    if (floor(size) != size) {
        std::cerr << "Invalid input file " << argv[1] << std::endl;
        Usage(argv[0]);
    }
}

// Create matrices for all possible moves
void CreateMoves(const std::vector<int>& grid, std::vector<std::vector<int> >& moves) {
    int size = grid.size();
    int length = sqrt(size);

    for (int i = 0; i < size; i++) {
        std::vector<int> move(size, 0);

        move[i] = 1;

        if (i - 1 >= 0 && i % length != 0) move[i - 1] = 1;
        if (i + 1 < size && (i + 1) % length != 0) move[i + 1] = 1;

        if (i - length >= 0) move[i - length] = 1;
        if (i + length < size) move[i + length] = 1;

        moves.push_back(move);
    }
}

// Create solution with gaussian elimination
void CreateSolution(const std::vector<int>& grid, std::vector<int>& solution, std::vector<std::vector<int> >& moves) {
    solution = grid;

    size_t size = grid.size();
    size_t i, j, k;

    for (i = 0; i < size; i++) {
        if (moves[i][i] == 0) {
            for (j = i + 1; j < size; j++) {
                if (moves[j][i] == 1) {
                    std::swap(moves[j], moves[i]);
                    std::swap(solution[j], solution[i]);

                    break;
                }
            }
        }

        for (j = 0; j < size; j++) {
            if (j == i) continue;

            if (moves[j][i] == 1) {
                for (k = 0; k < moves[j].size(); k++) {
                    moves[j][k] = (moves[j][k] + moves[i][k]) % 2;
                }

                solution[j] = (solution[j] + solution[i]) % 2;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) Usage(argv[0]);
    if (argc == 3 && strcmp(argv[2], "-F")) Usage(argv[0]);

    std::vector<int> grid, solution;
    std::vector<std::vector<int> > moves;

    ReadFile(argv, grid);

    CreateMoves(grid, moves);
    CreateSolution(grid, solution, moves);

    int length = sqrt(grid.size());
    
    if (argc == 3) {
        std::string input = std::string(argv[1]);
        std::string file = input.substr(0, input.length() - 4) + "_output.txt";

        std::ofstream output(file);
        if (!output) {
            std::cerr << "Cannot open " << file << " to print" << std::endl;
            exit(1);
        }

        for (size_t i = 0; i < grid.size(); i++) {
            output << ((solution[i] == 1) ? '*' : '-') << " ";
            if ((i + 1) % length == 0) output << std::endl;
        }

        output.close();

    } else {
        for (size_t i = 0; i < grid.size(); i++) {
            std::cout << ((solution[i] == 1) ? '*' : '-') << " ";
            if ((i + 1) % length == 0) std::cout << std::endl;
        }
    }
}
