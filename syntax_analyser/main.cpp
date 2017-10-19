#include <iostream>
#include "pascal_array_declaration/parser.h"
#include "graph_visualizator.h"


int main() {
    parser p;
    try {
        parser::graph_iterator it = p.parse("input.txt");
        try {
            createPNG("out_graph.png", it);
            if (system("xdg-open out_graph.png")) {
                std::cerr << "Can't open file with output graph\n";
            }
        } catch (std::exception& e) {
            std::cout << "Visualization errors\n";
        }

    } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << "Wrong array declaration\n";
    }

}