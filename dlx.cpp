//file includes Dlx class.
//you can initialize with a 2D boolean matrix and an array of column names,
//run one of the search functions to produce a solution,
//and use get_ans() to get a string, which represent answers.
//the string consists of rows separated by new lines -- each row is
//column names separated by spaces.
//
//print_internals() is so named because I might add a nicer way to print
//the data structure like
//
//h:13--0--1--2--13
//      |  |  |
//      3--+--4--3
//      |  |  |
//      |  5--6--5
//      |  |  |
//      7--8--9--7
//      |  |  |
//      0  1  2
//
// S =  2, 2, 3
// C= A:0, B:1, C:2
//
// instead of just listing out the values in all the arrays.
// at least print out cols

#include "dlx.h"
#include <vector>
#include <iostream>


//The input is a vector containing rows, not columns.
Dlx::Dlx(const std::vector<std::vector<bool>>& matrix,
            const std::vector<std::string>& names) {
    int num_rows = matrix.size();
    int num_cols = matrix[0].size();
    int num_ones = 0;
    for(int i=0; i<num_rows; i++) {
        for(int j=0; j<num_cols; j++) {
            if(matrix[i][j]) {
                ++num_ones;
            }
        }
    }

    answers = std::vector<std::string>();
    O = std::vector<int>(std::min(num_cols,num_rows),-1);

    //cols have indices [0,num_cols), h has index num_ones + num_cols
    S = std::vector<int>(num_cols,0); // not enough!
    N = std::vector<std::string>(names);
    h = num_cols + num_ones;

    L = std::vector<int>(num_ones + num_cols + 1);
    R = std::vector<int>(num_ones + num_cols + 1);
    D = std::vector<int>(num_ones + num_cols);
    U = std::vector<int>(num_ones + num_cols);
    C = std::vector<int>(num_ones + num_cols);

    //initialize LR relationships for columns and h




    //column i is i, h is num_cols
    for(int i=0; i<num_cols-1; ++i) {
        C[i] = i;
        R[i] = i+1;
        L[i+1] = i;
    }
    C[num_cols-1] = num_cols-1;
    R[num_cols-1] = num_cols + num_ones;
    L[num_cols + num_ones] = num_cols-1;
    R[num_cols + num_ones] = 0;
    L[0] = num_cols + num_ones;

    //move row by row, add cells, update UD and establish LR
    int data_index = num_cols;
    std::vector<int> current_lowest(num_cols,0);
    for(int i=0; i<num_cols; i++) {
        current_lowest[i] = i;
    }
    for(const std::vector<bool>& row : matrix) {
        int begin_data_index = data_index;
        for(int i=0; i<num_cols; ++i) {
            if(row[i]) {
                U[data_index] = current_lowest[i];
                D[current_lowest[i]] = data_index;
                C[data_index] = i;
                current_lowest[i] = data_index;
                ++data_index;
            }
        }
        for(int i=begin_data_index; i<data_index-1; ++i) {
            R[i] = i+1;
            L[i+1] = i;
        }
        R[data_index-1] = begin_data_index;
        L[begin_data_index] = data_index-1;
    }
    for(int i=0; i<num_cols; ++i) {
        D[current_lowest[i]] = i;
        U[i] = current_lowest[i];
    }

    for(int col=R[h]; col !=h; col=R[col]) {
        for(int el=D[col]; el!=col; el=D[el]) {
            ++S[col];
        }
    }
}

void Dlx::search(int k) {
    if(R[h] == h) {
        answers.push_back(produce_rows());
        return;
    }
    int c = R[h]; //chosen column
    int minVal = S[c];
    for(int col=R[c]; col!=h; col=R[col]) {
        if(S[col] < minVal) {
            c = col;
            minVal = S[col];
        }
    }
    cover_column(c);
    for(int r=D[c]; r!=c; r=D[r]) {
        O[k] = r;
        for(int j=R[r]; j!=r; j=R[j]) {
            cover_column(C[j]);
        }
        search(k+1);
        r = O[k];
        c = C[r];
        for(int j=L[r]; j!=r; j=L[j]) {
            uncover_column(C[j]);
        }
    }
    uncover_column(c);
    return;
}

void Dlx::search_log(int k) {
    if(R[h] == h) {
        std::cout << "answer found." << std::endl;
        answers.push_back(produce_rows());
        return;
    }
    int c = R[h]; //chosen column
    int minVal = S[c];
    for(int col=R[c]; col!=h; col=R[col]) {
        if(S[col] < minVal) {
            c = col;
            minVal = S[col];
        }
    }
    std::cout << "k: " << k << " col: " << c << std::endl;
    cover_column(c);
    for(int r=D[c]; r!=c; r=D[r]) {
        O[k] = r;
        for(int j=R[r]; j!=r; j=R[j]) {
            cover_column(C[j]);
        }
        search_log(k+1);
        r = O[k];
        c = C[r];
        for(int j=L[r]; j!=r; j=L[j]) {
            uncover_column(C[j]);
        }
    }
    uncover_column(c);
    std::cout << "complete. k: " << k << std::endl;
    return;
}

bool Dlx::search_first(int k) {
    if(R[h] == h) {
        answers.push_back(produce_rows());
        return true;
    }
    int c = R[h]; //chosen column
    int minVal = S[c];
    for(int col=R[c]; col!=h; col=R[col]) {
        if(S[col] < minVal) {
            c = col;
            minVal = S[col];
        }
    }
    cover_column(c);
    for(int r=D[c]; r!=c; r=D[r]) {
        O[k] = r;
        for(int j=R[r]; j!=r; j=R[j]) {
            cover_column(C[j]);
        }
        if(search_first(k+1)) {
            return true;
        }
        r = O[k];
        c = C[r];
        for(int j=L[r]; j!=r; j=L[j]) {
            uncover_column(C[j]);
        }
    }
    uncover_column(c);
    return false;
}

std::vector<std::string> Dlx::get_ans() {
    return answers;
}

void Dlx::print_internals() {
    std::cout << "i: ";
    for(int a=0; a<25; ++a){ std::cout << a << "\t";}
    std::cout << std::endl << "O: ";
    for(int a : O){ std::cout << a << "\t";}
    std::cout << std::endl << "L: ";
    for(int a : L){ std::cout << a << "\t";}
    std::cout << std::endl << "R: ";
    for(int a : R){ std::cout << a << "\t";}
    std::cout << std::endl << "U: ";
    for(int a : U){ std::cout << a << "\t";}
    std::cout << std::endl << "D: ";
    for(int a : D){ std::cout << a << "\t";}
    std::cout << std::endl << "C: ";
    for(int a : C){ std::cout << a << "\t";}
    std::cout << std::endl << "S: ";
    for(int a : S){ std::cout << a << "\t";}
    std::cout << std::endl << "N: ";
    for(std::string a : N){ std::cout << a << "\t";}
    std::cout << std::endl << "h: " << h << std::endl;
}

void Dlx::cover_column(int c) {
    L[R[c]] = L[c];
    R[L[c]] = R[c];
    for(int i=D[c]; i!=c; i=D[i]) {
        for(int j=R[i]; j!=i; j=R[j]) {
            U[D[j]] = U[j];
            D[U[j]] = D[j];
            S[C[j]] = S[C[j]] - 1;
        }
    }
}

void Dlx::uncover_column(int c) {
    for(int i=U[c]; i!=c; i=U[i]) {
        for(int j=L[i]; j!=i; j=L[j]) {
            S[C[j]] = S[C[j]] + 1;
            D[U[j]] = j;
            U[D[j]] = j;
        }
    }
    L[R[c]] = c;
    R[L[c]] = c;
}

std::string Dlx::produce_rows() {
    std::string ans = "";
    for(int i=0; i<O.size() && O[i]!=-1; ++i) {
        ans += N[C[O[i]]];
        for(int j=R[O[i]]; j!=O[i]; j=R[j]) {
            ans += " " + N[C[j]];
        }
        ans += "\n";
    }
    return ans;
}
