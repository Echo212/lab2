g++ -g julia.cpp -o julia
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./julia
