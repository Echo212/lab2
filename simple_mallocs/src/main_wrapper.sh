g++ -g main.cpp -o main
ulimit -v 80000
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./main