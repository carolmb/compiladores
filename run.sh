echo "-----------------"
make yacc && 
echo "-----------------" &&
./syntax/yacc/yacc_syn < $1
