/* 
 * File:   main.cpp
 * Author: santony
 *
 * Created on December 26, 2012, 12:22 PM
 */

#include <cstdlib>
#include <utility>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/directed_graph.hpp>

using namespace std;
using namespace boost;

/*
 * 
 */
int main(int argc, char** argv) 
{
    typedef adjacency_list<vecS, vecS, bidirectionalS> Graph;
    
    enum {A, B, C, D, E, F, N };
    const int num_vertices = N;
    const char* name = "ABCDEF";
    
    typedef pair<int, int> Edge;
    Edge edge_array[] = 
    {
        Edge(C, F),
        Edge(B, E),
        Edge(A, B), 
        Edge(B, D),
        Edge(A, C)
        //Edge(B, D),
        //Edge(D, E)
    };
    const int num_edges = sizeof(edge_array) / sizeof(edge_array[0]);
    
    /*
    Graph g(num_vertices);
    for (int i = 0; i != num_edges; ++i)
    {
        add_edge(edge_array[i].first, edge_array[i].second, g);
    }
     * */
    Graph g(edge_array, edge_array + num_edges, num_vertices);

    typedef property_map<Graph, vertex_index_t>::type IndexMap;
    typedef property_map<Graph, edge_index_t>::type EdgeMap;
    IndexMap index = get(vertex_index, g);
    EdgeMap edgeindex = get(edge_index, g);
        
    /*
    typedef graph_traits<Graph>::vertex_descriptor Vertex;
    typedef list<Vertex> MakeOrder;
    MakeOrder make_order;
    topological_sort(g, back_inserter(make_order));
    for (MakeOrder::iterator i = make_order.begin(); i != make_order.end(); ++i)
    {
        //cout << name[*i] << endl;
        if (in_degree(*i, g) > 0)
        {
            Graph::in_edge_iterator j, j_end;
            for (tie(j, j_end) = in_edges(*i, g); j!=j_end; ++j)
            {
                cout << "(" << name[source(*j, g)] << "," << name[target(*j, g)] << ")" << endl;
            }
        }
        
    }
     * */

    
    typedef graph_traits<Graph>::vertex_iterator vertex_iter;
    typedef graph_traits<Graph>::adjacency_iterator AdjIter ;
    std::pair<vertex_iter, vertex_iter> vp;
    AdjIter ai, aiend;
    for (vp = vertices(g); vp.first != vp.second; ++vp.first)
    {
        if (out_degree(index[*vp.first], g) > 0)
        {
            //cout << index[*vp.first] << " " << endl;
            cout << "Adjacent vertex of " << name[index[*vp.first]] << " are: " << endl;
            for (tie (ai, aiend) =adjacent_vertices(*vp.first, g); ai != aiend; ++ai)
            {
                cout << name[index[*ai]] << ", " << endl;
            }
        }
    }
    
    typedef graph_traits<Graph>::edge_iterator edge_iter;
    edge_iter ei, ei_end;
    for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
    {
        cout << "(" << index[source(*ei, g)] << "," << index[target(*ei, g)] << ")" << endl;
    }
    
    
    
    
    return 0;
}

