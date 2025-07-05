// Kruskal's algorithm in C++

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
#include <tuple>
#include <chrono>
#include <fstream>
#include <set>
#include <ctime>
#include <utility>

using namespace std;
typedef chrono::milliseconds MSEC;
typedef chrono::high_resolution_clock HRC;

#define edge pair<int, int>

class Graph {
   private:
  vector<pair<int, edge> > G;  // graph
  vector<pair<int, edge> > T;  // mst
  int *parent;
  int V;  // number of vertices/nodes in graph
   public:
  Graph(int V);
  void AddWeightedEdge(int u, int v, int w);
  int find_set(int i);
  void union_set(int u, int v);
  void kruskal();
  void prim();
  void boruvka();
  void print();
  void generateRandomConnectedGraph(int maxWeight = 100);
};
Graph::Graph(int V) {
  this->V = V;
  parent = new int[V];

  //i 0 1 2 3 4 5
  //parent[i] 0 1 2 3 4 5
  for (int i = 0; i < V; i++)
    parent[i] = i;

  G.clear();
  T.clear();
}
void Graph::AddWeightedEdge(int u, int v, int w) {
  G.push_back(make_pair(w, edge(u, v)));
}
int Graph::find_set(int i) {
  // If i is the parent of itself
  if (i == parent[i])
    return i;
  else
    // Else if i is not the parent of itself
    // Then i is not the representative of his set,
    // so we recursively call Find on its parent
    return find_set(parent[i]);
}

void Graph::union_set(int u, int v) {
  parent[u] = parent[v];
}
void Graph::kruskal() {
  int i, uRep, vRep;
  sort(G.begin(), G.end());  // increasing weight
  for (i = 0; i < G.size(); i++) {
    uRep = find_set(G[i].second.first);
    vRep = find_set(G[i].second.second);
    if (uRep != vRep) {
      T.push_back(G[i]);  // add to tree
      union_set(uRep, vRep);
    }
  }
}

void Graph::prim() {
    // Clear the MST
    T.clear();

    // Create a priority queue to store vertices that are being primed
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    int src = 0; // Start with vertex 0

    // Create a vector for keys and initialize to INFINITE
    vector<int> key(V, INT_MAX);

    // To store parent array which in turn store MST
    vector<int> parent(V, -1);

    // To keep track of vertices included in MST
    vector<bool> inMST(V, false);

    // Insert source itself in priority queue and initialize its key as 0
    pq.push(make_pair(0, src));
    key[src] = 0;

    // Loop until priority queue becomes empty
    while (!pq.empty()) {
        // The first vertex in pair is the minimum key vertex
        int u = pq.top().second;
        pq.pop();

        inMST[u] = true; // Include vertex in MST

        // Iterate through all adjacent vertices of u
        for (auto it = G.begin(); it != G.end(); ++it) {
            // Get vertex label and weight of current adjacent of u
            int v, w;
            if (it->second.first == u) {
                v = it->second.second;
                w = it->first;
            } else if (it->second.second == u) {
                v = it->second.first;
                w = it->first;
            } else {
                continue; // Not adjacent to u
            }

            // If v is not in MST and weight of (u,v) is smaller than current key of v
            if (!inMST[v] && w < key[v]) {
                // Update key of v
                key[v] = w;
                pq.push(make_pair(key[v], v));
                parent[v] = u;
            }
        }
    }

    // Add edges to MST
    for (int i = 1; i < V; ++i) {
        if (parent[i] != -1) {
            T.push_back(make_pair(key[i], edge(parent[i], i)));
        }
    }
}



void Graph::boruvka() {
    // Clear the MST
    T.clear();

    // Initialize parent array for union-find
    for (int i = 0; i < V; i++)
        parent[i] = i;

    // Initialize number of trees and MST weight
    int numTrees = V;
    int MSTweight = 0;

    // Continue until there's only one tree
    while (numTrees > 1) {
        // Create an array to store the cheapest edge for each component
        vector<pair<int, edge>> cheapest(V, make_pair(INT_MAX, edge(-1, -1)));

        // Traverse through all edges
        for (auto it = G.begin(); it != G.end(); ++it) {
            int u = it->second.first;
            int v = it->second.second;
            int w = it->first;

            // Find components of u and v
            int set_u = find_set(u);
            int set_v = find_set(v);

            // If u and v belong to same set, ignore
            if (set_u == set_v)
                continue;

            // Check if this edge is cheaper than current cheapest for set_u
            if (cheapest[set_u].first > w) {
                cheapest[set_u] = make_pair(w, edge(u, v));
            }

            // Check if this edge is cheaper than current cheapest for set_v
            if (cheapest[set_v].first > w) {
                cheapest[set_v] = make_pair(w, edge(u, v));
            }
        }

        // Add the cheapest edges to MST
        for (int i = 0; i < V; i++) {
            if (cheapest[i].second.first != -1) {
                int u = cheapest[i].second.first;
                int v = cheapest[i].second.second;
                int set_u = find_set(u);
                int set_v = find_set(v);

                if (set_u != set_v) {
                    MSTweight += cheapest[i].first;
                    T.push_back(cheapest[i]);
                    union_set(set_u, set_v);
                    numTrees--;
                }
            }
        }
    }
}


void Graph::print() {
  cout << "Edge :"
     << " Weight" << endl;
  for (int i = 0; i < T.size(); i++) {
    cout << T[i].second.first << " - " << T[i].second.second << " : "
       << T[i].first;
    cout << endl;
  }
}

void recordKruskal(Graph* g)
{
    fstream myFile;
    myFile.open("KruskalTime.txt", ios::app);
    auto start = HRC::now();
    g->kruskal();
    auto stop = HRC::now();
    auto executeTime = chrono::duration_cast<MSEC>(stop - start);
    cout << "Kruskal Execution Time: " << ((double) executeTime.count() / 1000.00) << " secs" << endl;
    myFile << ((double) executeTime.count() / 1000.00) << endl;
    myFile.close();
}

void recordPrim(Graph* g)
{
    fstream myFile;
    myFile.open("PrimTime.txt", ios::app);
    auto start = HRC::now();
    g->kruskal();
    auto stop = HRC::now();
    auto executeTime = chrono::duration_cast<MSEC>(stop - start);
    cout << "Prim Execution Time: " << ((double) executeTime.count() / 1000.00) << " secs" << endl;
    myFile << ((double) executeTime.count() / 1000.00) << endl;
    myFile.close();
}

void recordBoruvka(Graph* g)
{
    fstream myFile;
    myFile.open("BoruvkaTime.txt", ios::app);
    auto start = HRC::now();
    g->boruvka();
    auto stop = HRC::now();
    auto executeTime = chrono::duration_cast<MSEC>(stop - start);
    cout << "Boruvka Execution Time: " << ((double) executeTime.count() / 1000.00) << " secs" << endl;
    myFile << ((double) executeTime.count() / 1000.00) << endl;
    myFile.close();
}


void Graph::generateRandomConnectedGraph(int max_weight) {
    // Clear existing edges
    G.clear();

    // First ensure the graph is connected by creating a random spanning tree
    vector<int> nodes(V);
    for (int i = 0; i < V; ++i) {
        nodes[i] = i;
    }

    // Shuffle nodes to create a random order
    random_shuffle(nodes.begin(), nodes.end());

    // Connect nodes to form a tree (ensuring connectivity)
    for (int i = 1; i < V; ++i) {
        // Connect to a random previous node
        int j = rand() % i;
        int weight = 1 + rand() % max_weight;
        AddWeightedEdge(nodes[j], nodes[i], weight);
    }

    // Add additional random edges to make it a full graph
    int extra_edges = V + rand() % V;

    set<pair<int, int>> existing_edges;
    // Add existing edges to the set
    for (const auto &graph_edge : G) {
        int u = graph_edge.second.first;
        int v = graph_edge.second.second;
        if (u > v) swap(u, v);
        existing_edges.insert({u, v});
    }

    for (int i = 0; i < extra_edges; ++i) {
        int u = rand() % V;
        int v = rand() % V;
        if (u == v) continue;  // no self-loops

        // Ensure u < v for consistent storage
        if (u > v) swap(u, v);

        // If edge doesn't already exist
        if (existing_edges.find({u, v}) == existing_edges.end()) {
            int weight = 1 + rand() % max_weight;
            AddWeightedEdge(u, v, weight);
            existing_edges.insert({u, v});
        }
    }
}
int main() {
  for(int i = 1000; i <= 20000; i+= 1000)
  {
      Graph g(i);
      g.generateRandomConnectedGraph(i);
      cout << "Size: " << i << endl;
      recordKruskal(&g);
      recordPrim(&g);
      recordBoruvka(&g);
  }


  return 0;
}
