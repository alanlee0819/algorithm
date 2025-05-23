#include <bits/stdc++.h>
#include "json.hpp"
using json = nlohmann::json;

bool inPath(const std::string& u,const std::string& v,
            const std::vector<std::string>& p){
    for(size_t i=1;i<p.size();++i) if(p[i-1]==u && p[i]==v) return true;
    return false;
}

int main(int argc,char* argv[]){
    if(argc<3){
        std::cerr<<"用法: draw_graph <input.json> <output.dot>\n";
        return 1;
    }
    std::ifstream fin(argv[1]);
    json j; fin>>j;

    std::ofstream dot(argv[2]);
    dot<<"digraph G {\n  rankdir=LR;\n"
       <<"  node [shape=circle, style=filled, fillcolor=lightgray];\n";

    /* ===== 節點 ===== */
    std::unordered_set<std::string> pathSet(j["path"].begin(), j["path"].end());
    for(auto& n: j["nodes"]){
        std::string id = n["id"];
        dot<<"  \""<<id<<"\""
           <<(pathSet.count(id)? " [fillcolor=\"salmon\"]":"")
           <<";\n";
    }

    /* ===== 邊 ===== */
    for(auto& e: j["edges"]){
        std::string src = e["source"];
        std::string tgt = e["target"];
        double w        = e["weight"];
        bool hi = inPath(src,tgt,j["path"]);
        dot<<"  \""<<src<<"\" -> \""<<tgt<<"\" "
           <<"[label=\""<<w<<"\""
           <<(hi? ",color=red,penwidth=2":"")
           <<"];\n";
    }
    dot<<"}\n";
}
