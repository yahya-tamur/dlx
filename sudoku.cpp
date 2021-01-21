//functionality:
//take 81-long string, find any solution.
//
//finding all solutions takes a really long time.
//maybe an interesting problem would be to find all possible sudoku answers.
//There are a lot of solutions, but also a lot of interesting symmetries.
//these symmetries may even be more obvious in set-cover form.
//I don't plan to do that anytime soon though.
//
//col[i + 9*j]: row i, col j is occupied.
//col[i + 9*j + 81]: number i is on row j.
//col[i + 9*j + 162]: number i in on col j.
//col[i + 9*j + 243]: number i is in block j.
//an empty board would have 729 rows, 324 columns.

//I repeated some lines here.
#include "dlx.h"
#include "sudoku.h"
#include <vector>
#include <string>

Dlx initialize_sudoku(std::string input) {
    std::vector<std::vector<bool>> rows;
    for(int i=0; i<9; ++i) {
        for(int j=0; j<9; ++j) {
            if(input[j+9*i] == '0') {
                for(int k=0; k<9; ++k) {
                    rows.emplace_back(324);
                    rows.back()[i+9*j] = true;
                    rows.back()[k+9*j+81] = true;
                    rows.back()[k+9*i+162] = true;
                    rows.back()[k+9*((i/3) + 3*(j/3)) + 243] = true;
                }
            } else {
                int k = input[j+9*i] - '1';
                rows.emplace_back(324);
                rows.back()[i+9*j] = true;
                rows.back()[k+9*j+81] = true;
                rows.back()[k+9*i+162] = true;
                rows.back()[k+9*((i/3) + 3*(j/3)) + 243] = true;
            }
        }
    }
    std::vector<std::string> names(324);
    for(int i=0; i<9; ++i) {
        for(int j=0; j<9; ++j) {
            names[i + 9*j] = "r:" + std::to_string(i) + "c:" + std::to_string(j);
            names[i + 9*j + 81] = "n:" + std::to_string(i+1);
            names[i + 9*j + 162] = "";
            names[i + 9*j + 243] = "";

        }
    }
    return Dlx(rows, names);
}

std::string format_sudoku_ans(std::string dlx_ans) {
    std::string ans(90,'\n');
    std::size_t f = 0;
    while(dlx_ans[f+1] != 0) {
        std::string n = dlx_ans.substr(dlx_ans.find("n:",f)+2,1);
        int i = dlx_ans[dlx_ans.find("r:",f)+2] - '0';
        int j = dlx_ans[dlx_ans.find("c:",f)+2] - '0';
        ans.replace(10*i + j,1,n);
        f = dlx_ans.find('\n',f+1);
    }
    return ans;
}
