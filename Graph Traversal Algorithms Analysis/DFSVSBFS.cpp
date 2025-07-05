// BFS algorithm in C++
#include <iostream>
#include <list>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;
typedef chrono::milliseconds MSEC;
typedef chrono::high_resolution_clock HRC;

//BFS START
class Graph {
public:
  int numVertices;
  list<int>* adjLists;
  bool* visited;
  Graph(int vertices);
  void addEdge(int src, int dest);
  void DFS(int vertex);
  void BFS(int vertex);
};

// Create a graph with given vertices,
// and maintain an adjacency list
Graph::Graph(int vertices) {
  numVertices = vertices;
  adjLists = new list<int>[vertices];
  visited = new bool[vertices];
}

// Add edges to the graph
void Graph::addEdge(int src, int dest) {
  adjLists[src].push_back(dest);
  adjLists[dest].push_back(src);
}

// BFS algorithm
void Graph::BFS(int startVertex) {
  for (int i = 0; i < numVertices; i++)
    visited[i] = false;

  list<int> queue;

  visited[startVertex] = true;
  queue.push_back(startVertex);

  list<int>::iterator i;

  while (!queue.empty()) {
    int currVertex = queue.front();
    //cout << "Visited " << currVertex << " ";
    queue.pop_front();

    for (i = adjLists[currVertex].begin(); i != adjLists[currVertex].end(); ++i) {
      int adjVertex = *i;
      if (!visited[adjVertex]) {
        visited[adjVertex] = true;
        queue.push_back(adjVertex);
      }
    }
  }
  delete[] visited;
}
//BFS END

//DFS START
void Graph::DFS(int vertex) {
  visited[vertex] = true;
  //cout << vertex << " ";

  for (int adj : adjLists[vertex]) {
    if (!visited[adj]) {
      DFS(adj);
    }
  }
}
//DFS END
void recordDFS(Graph* g, int startingVertex){
    fstream myFile;
    myFile.open("DFSTime.txt", ios::app);
    auto start = HRC::now();
    g->DFS(startingVertex);
    auto stop = HRC::now();
    auto executeTime = chrono::duration_cast<MSEC>(stop - start);
    cout << "DFS Execution Time: " << ((double) executeTime.count() / 1000.00) << " secs" << endl;
    myFile << ((double) executeTime.count() / 1000.00) << endl;
    //cout << "DFS Finished" << endl;
    myFile.close();
}

void recordBFS(Graph * g, int startingVertex)
{
    fstream myFile;
    myFile.open("BFSTime.txt", ios::app);
    auto start = HRC::now();
    g->BFS(startingVertex);
    auto stop = HRC::now();
    auto executeTime = chrono::duration_cast<MSEC>(stop - start);
    cout << "BFS Execution Time: " << ((double) executeTime.count() / 1000.00) << " secs" << endl;
    myFile << ((double) executeTime.count() / 1000.00) << endl;
    myFile.close();
}
int getRandomVertex(const string filename){
    cout << "Function Entered!" << endl;
    ifstream file(filename);
    vector<int> values;
    string line;

    while(getline(file, line))
    {
        istringstream iss(line);
        int value;
        if(iss >> value){
            values.push_back(value);
        }
        file.close();
    }
    if(!values.empty()){
        return values[rand()%values.size()];
    }
    return 0;
}

void appendSource(const string& filename, int data){
    ofstream file(filename, ios::app);
    if(file.is_open()){
        file << to_string(data) << "\n";
        file.close();
        return;
        //cout << "File inserted!" << endl;
    }
    else{
        cout << "File not open!" << endl;
    }
    return;
}
int main() {
    srand(time(NULL));
    for(int i = 1000; i <= 10000; i += 1000)
    {
        ofstream clearSources("Sources.txt", ios::trunc);
        Graph g(i);
        for(int j = 0; j < i - 1; j++){
            for(int k = j + 1; k < i; k++){
                int hasAConnection = rand()%2;
                if(hasAConnection){
                    g.addEdge(j, k);
                    if(rand()% 100 == 0)
                        appendSource("Sources.txt", j);
                }
            }
        }
        int startingVertex = getRandomVertex("Sources.txt");
        cout << "Vertex retrieved" << endl;
        recordDFS(&g, startingVertex);

        recordBFS(&g, startingVertex);
        //delete[] g.adjLists;
        //delete[] g.visited;
    }



    return 0;
}
