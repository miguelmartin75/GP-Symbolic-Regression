#ifndef GENETICOPERATIONS_HPP
#define GENETICOPERATIONS_HPP

#include "Function.hpp"

/*
 * There are three major genetic operations that
 * can be performed:
 * 
 * 1. Mutation
 *    Mutation is taking some random chunks of the program and
 *    mutating (changing) it.
 * 2. Mating
 *    Mating is done by taking two "parent" programs and taking random chunks
 *    of them and mushing them together.
 * 3. Duplication
 *    This operation is straight forward and rather obvious to implement.
 *    Basically you just "duplicate" the program and keep it for the next
 *    generation.
 *
 * Source: http://pages.ucsd.edu/~aerobins/genetic/div3.pdf
 */

Function mutate(const Function& fn);
Function mate(const Function& p1, const Function& p2);

#endif // GENETICOPERATIONS_HPP
