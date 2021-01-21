#ifndef SUDOKU_H
#define SUDOKU_H

#include "dlx.h"
#include <vector>
#include <string>

//makes Dlx that represents the problem
Dlx initialize_sudoku(std::string input);

//transforms the string output of the problem created above and prints it in a
//nicer way.
std::string format_sudoku_ans(std::string dlx_ans);

#endif /* SUDOKU_H */
