#include "dlx.h"
#include "pentominoes.h"
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>

//as described in the paper, the first 12 columns describe whether or not each
//pentomino has already been used so that the same piece isn't used twice and
//the next 8x8 - 2x2 columns describe whether or not a single available square
//is occupied. each row describes placing one pentomino in one location.

//8x8 board with the center 2x2 cut out
Dlx initialize_pentominoes() {
    std::vector<std::vector<bool>> rows;
    rows.reserve(2000); //probably too much
    //a list of all 12 penttominoes
    //only first five coordinates are in the pentomino; the last value is
    //(the largest x value, the largest y value).
    std::vector<std::vector<std::vector<short>>> pents = {
        {{0,0},{0,1},{0,2},{0,3},{1,3}},
        {{0,0},{0,1},{0,2},{1,2},{1,3}},
        {{1,0},{1,1},{1,2},{0,2},{1,3}},
        {{0,0},{1,0},{1,1},{0,2},{1,2}},
        {{0,0},{1,0},{1,1},{0,1},{1,2}},
        {{0,0},{0,1},{0,2},{1,2},{2,2}},
        {{0,0},{0,1},{1,1},{2,1},{2,2}},
        {{0,1},{1,0},{1,1},{1,2},{2,2}},
        {{0,1},{1,1},{2,0},{2,1},{2,2}},
        {{0,0},{0,1},{1,1},{1,2},{2,2}},
        {{0,1},{1,0},{1,1},{1,2},{2,1},{2,2}}};
    //for every pentomino, for every location in the board
    for(int n=0; n<11; ++n) {
        for(int i=0; i<8; ++i) {
            for(int j=0; j<8; ++j) {
                //if the bottom right of the pentomino is outside of the board, conntinue
                if(pents[n].back()[0]+i >= 8 || pents[n].back()[1]+j >= 8) {
                    continue;
                }
                //add all 8 rotations and reflections of the pentomino at the location
                //also mark all those placements with the information about which
                //pentomino choosing that row would use up.
                for(int k=0; k<8; ++k) {
                    rows.emplace_back(76);
                    rows.back()[n] = true;
                }
                //mark columns about the information about which square is occupied
                for(int k=0; k<5; ++k) {
                    rows[rows.size()-1][8*(pents[n][k][0]+i)+(pents[n][k][1]+j)+12] = true;
                    rows[rows.size()-2][8*(pents[n][k][0]+i)+(7-(pents[n][k][1]+j))+12] = true;
                    rows[rows.size()-3][8*(7-(pents[n][k][0]+i))+(pents[n][k][1]+j)+12] = true;
                    rows[rows.size()-4][8*(7-(pents[n][k][0]+i))+(7-(pents[n][k][1]+j))+12] = true;
                    rows[rows.size()-5][8*(pents[n][k][1]+j)+(pents[n][k][0]+i)+12] = true;
                    rows[rows.size()-6][8*(pents[n][k][1]+j)+(7-(pents[n][k][0]+i))+12] = true;
                    rows[rows.size()-7][8*(7-(pents[n][k][1]+j))+(pents[n][k][0]+i)+12] = true;
                    rows[rows.size()-8][8*(7-(pents[n][k][1]+j))+(7-(pents[n][k][0]+i))+12] = true;
                }
                //remove all 8 previous pentominoes if they are in the middle 2x2
                //this filter would be better before the unitary transformatios
                //but it's way easier with the bool arrays vs. the tuples.
                if(rows.back()[39] || rows.back()[40] || rows.back()[47] || rows.back()[48]) {
                    rows.resize(rows.size()-8);
                }
            }
        }
    }

    //add the long piece. Only in one corner in one direction so rotations/reflections
    //of previous solutions aren't considered.
    //there actually is no solution with j=1 or j=2
    for(int i=0; i<2; ++i) {
        for(int j=0; j<3; ++j) {
            rows.emplace_back(76);
            rows.back()[11] = true;
            for(int k=0; k<5; ++k) {
                rows.back()[8*(i+k)+j+12] = true;
            }
        }
    }

    //slide back array so every column is a square to be filled (the middle 2x2
    //doesn't have columns)
    for(std::vector<bool>& r : rows) {
        for(int i=41; i<47; ++i) {
            r[i-2] = r[i];
        }
        for(int i=49; i<76; ++i) {
            r[i-4] = r[i];
        }
        r.resize(72);
    }

    //sort rows and remove identical ones. Some identical ones are produced
    //since some pentominoes are symmetrical
    sort(rows.begin(),rows.end(),
            [](std::vector<bool> a, std::vector<bool> b) {
                long long c = 0;
                for(int i=0; i<a.size(); ++i) {
                    c = c<<1;
                    c += a[i] - b[i];
                }
                return c<0;
            });

    auto inequal = [](std::vector<bool> a, std::vector<bool> b) {
        for(int i=0; i<a.size(); ++i) {
            if(a[i] != b[i]) {
                return true;
            }
        }
        return false;
    };

    int index = 0;
    for(int i=1; i<rows.size(); ++i) {
        if(inequal(rows[i-1],rows[i])) {
            ++index;
        }
        rows[index] = rows[i];
    }
    rows.resize(index+1);

    //make names of columns. the columns about which pieces are used up are
    //named after the piece used up, the other columns are named after which
    //boxes are occupied.
    std::vector<std::string> names = {"L","N","Y","U","P","V","Z","F","T","W","X","I"};
    names.reserve(72);
    for(int i=0; i<8; ++i) {
        for(int j=0; j<8; ++j) {
            if((i==3||i==4)&&(j==3||j==4)){
                continue;
            }
            names.push_back("(" + std::to_string(i) + "," + std::to_string(j) + ")");
        }
    }
    return Dlx(rows,names);
}

std::string format_pentomino_output(std::string dlx_output) {
    //the first part transformations transforms 12 lines like
    //"(1,0) (1,1) I (1,2) (1,3) (1,4)" into
    //"pIpppppp\n"
    //"pIpppppp\n"
    //"pIpppppp\n"
    //"pIp..ppp\n"
    //"pIp..ppp\n"
    //"pppppppp\n"
    //"pppppppp\n"
    //"pppppppp\n"
    //where 'p' would be filled with other pentominoes, and the second part
    //transforms that into something like
    // _______________
    //| |  _| |_   ___|
    //| | |_   _|_|_  |
    //| |___|_|_   _| |
    //| |   |   | | | |
    //|_|  _|___|_| |_|
    //| |_| |_  | |_  |
    //| |___  | |_  |_|
    //|_____|_|___|___|

    //there are extra spaces at the beginning of every line and an extra line
    std::vector<std::vector<char>> board;
    for(int i=0; i<10; ++i) {
        board.emplace_back(9,' ');
    }
    //[begin, end) of lines.
    int begin = 0;
    int end = dlx_output.find('\n');
    for(; end != std::string::npos; begin = end, end = dlx_output.find('\n',end+1) ) {
        char name;
        for(int i=begin;i<end; ++i) {
            if(isupper(dlx_output[i])) {
                name = dlx_output[i];
                break;
            }
        }
        //c matches every '('
        for(int c=dlx_output.find('(',begin);
                c<end && c!=std::string::npos;
                c=dlx_output.find('(',c+1)) {
            // '/' is one before '0'. c - '/' =  1+((int) (c - '0'))
            // 1+ again so that there is space in the beginning.
            board[dlx_output[c+1]-'0'][dlx_output[c+3]-'/'] = name;
        }
    }

    std::string output(18*8 + 17,' ');
    output.replace(0,17," _______________\n");
    for(int i=0; i<8; ++i) {
        output.replace(17 + 18*i + 16,2,"|\n");
    }
    for(int i=0; i<8; ++i) {
        for(int j=0; j<8; ++j) {
            if(board[i][j+1]!=board[i+1][j+1]) {
                output[17 + 18*i + 2*j + 1] = '_';
                if(output[17 + 18*i + 2*j - 1] == '_') {
                    output[17 + 18*i + 2*j] = '_';
                }
            }
            if(board[i][j+1]!=board[i][j]) {
                output[17 + 18*i + 2*j] = '|';
            }
        }
    }

    return output;
}
