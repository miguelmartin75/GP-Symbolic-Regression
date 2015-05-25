#!/bin/zsh
COMMAND='./build/gp_symb_reg_cli'

FILE=$1
shift

echo "Generation,Fitness,Mutated?,Mated?" > $FILE

$COMMAND $@ >> $FILE
