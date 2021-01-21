#ifndef PENTOMINOES_H
#define PENTOMINOES_H

#include "dlx.h"
#include <vector>
#include <algorithm>
#include <string>

//makes Dlx that represents the problem
Dlx initialize_pentominoes();

//transforms the string output of the problem created above and prints it in a
//nicer way.
std::string format_pentomino_output(std::string dlx_output);

#endif /* PENTOMINOES_H */
