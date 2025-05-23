g++ -std=c++17 test_gen.cpp -o test_gen
test_gen 20 25 > sample.txt
g++ -std=c++17 a_star.cpp -o a_star
a_star.exe < sample.txt
g++ -std=c++17 draw_graph.cpp -o draw_graph
draw_graph output.json graph.dot
dot -Tpng graph.dot -o graph.png
start graph.png