#include <iostream>
#include "pascal_array_declaration/parser.h"
#include <graphviz/gvc.h>

static GVC_t* gvc;
static Agraph_t *G;
static int counter;

std::string get_fresh_label() {
    counter++;
    return std::to_string(counter);
}

Agnode_t* createGraph(Agraph_t* G, parser::graph_iterator &it) {
    Agnode_t* parent_node = agnode(G, &get_fresh_label()[0], 1);
    std::string attr_name = "label";
    agset(parent_node, &attr_name[0], &it.get_str()[0]);
    for (auto& child : it.get_children()) {
        Agnode_t* child_node = createGraph(G, child);
        agedge(G, parent_node, child_node, nullptr, 1);
    }
    return parent_node;
}

FILE* createPNG(std::string file_name, parser::graph_iterator& it) {
    gvc = gvContext();
    std::string g_name = "graph";
    G = agopen(&g_name[0], Agstrictdirected, 0);

    counter = 0;
    createGraph(G, it);

    gvLayout(gvc, G, "dot");
    FILE* png = fopen(file_name.c_str(), "w");
    gvRender(gvc, G, "png", png);
    gvFreeLayout(gvc, G);

    agclose(G);
    assert(gvFreeContext(gvc) == 0);
    return png;
}

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