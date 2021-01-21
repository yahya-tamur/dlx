#include "dlx.h"
#include "pentominoes.h"
#include "sudoku.h"
#include <fstream>
#include <string>
#include <vector>
#include <chrono>


void make_pentominoes() {
        auto start = std::chrono::high_resolution_clock::now();
    Dlx d = initialize_pentominoes();
    d.search();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "it took: " << duration.count() << " microseconds.\n" << std::endl;
    for(std::string s : d.get_ans()) {
        std::cout << format_pentomino_output(s);
    }
}

void see_internals() {
    std::vector<std::vector<bool>> input = {
        {0,0,1,0,1,1,0},
        {1,0,0,1,0,0,1},
        {0,1,1,0,0,1,0},
        {1,0,0,1,0,0,0},
        {0,1,0,0,0,0,1},
        {0,0,0,1,1,0,1}};
    std::vector<std::string> input_names = {"A","B","C","D","E","F","G"};
    Dlx simple_problem(input,input_names);
    std::cout << "\ninternals:" << std::endl;
    simple_problem.print_internals();
    std::cout << "\nlog:" << std::endl;
    simple_problem.search_log();
    std::cout << "\nanswers:" << std::endl;
    for(std::string s : simple_problem.get_ans()) {
        std::cout << s << "\n";
    }
}


//I literally googled 'hard sudoku problem' to find this one
void single_problem() {
    auto start = std::chrono::high_resolution_clock::now();
    Dlx dlx = initialize_sudoku("800000000003600000070090200050007000000045700000100030001000068008500010090000400");
    dlx.search_first();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "it took: " << duration.count() << " microseconds.\n\n" << std::endl;
    std::cout << format_sudoku_ans(dlx.get_ans()[0]) << std::endl;
}


//sudoku.cpp really isn't optimized for many sudoku problems in succession --
//see README
//this does not format and print the answers.
//I guess I'm used to lazy evaluation from haskell -- it feels weird to know that
//yes, everything is still solved, even if the values aren't used.

//this test file is taken straight from another repository.
//https://github.com/shadaj/sudoku
void sudoku_problems_from_file() {
    std::ifstream input( "sudoku17.txt" );
    std::vector<std::string> inputs;
    inputs.reserve(49151); //number of lines in the file. yes it should not be hard-coded
    for(int i=0; i<49151; ++i) {
        std::string line;
        getline(input, line);
        inputs.push_back(line);
    }
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<49151; ++i) {
        Dlx dlx = initialize_sudoku(inputs[i]);
        dlx.search_first();
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "it took: " << duration.count() << " microseconds.\n\n" << std::endl;
}

int main() {
    make_pentominoes();
}
