#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream> 

using namespace std;

struct Edge {
    int u, v, weight;
    bool operator<(Edge const& other) {
        return weight < other.weight;
    }
};

class DSU {
private:
    vector<int> parent, rank;
public:
    DSU(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    int find(int x) {
        if (x != parent[x]) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    bool unionSets(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX != rootY) {
            if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX;
            }
            else if (rank[rootX] < rank[rootY]) {
                parent[rootX] = rootY;
            }
            else {
                parent[rootY] = rootX;
                rank[rootX]++;
            }
            return true;
        }
        return false;
    }

    // In ra các tập hợp hiện tại
    void printSets() {
        cout << "Current sets (parents): ";
        for (int i = 0; i < parent.size(); i++) {
            cout << parent[i] << " ";
        }
        cout << endl;
    }
};

void kruskalMST(int n, vector<Edge>& edges) {
    sort(edges.begin(), edges.end());
    DSU dsu(n);
    vector<Edge> mst_edges;
    int mst_weight = 0;

    cout << "Start Kruskal's algorithm...\n\n";

    for (Edge& edge : edges) {
        cout << "Checking edge: " << edge.u << " - " << edge.v << " with weight " << edge.weight << endl;

        if (dsu.unionSets(edge.u, edge.v)) {
            mst_weight += edge.weight;
            mst_edges.push_back(edge);
            cout << "Edge " << edge.u << " - " << edge.v << " added to MST." << endl;
        }
        else {
            cout << "Edge " << edge.u << " - " << edge.v << " forms a cycle, so it is skipped.\n";
        }

        dsu.printSets(); // In ra tình trạng các tập hợp
        cout << endl;
    }

    cout << "Edges in the Minimum Spanning Tree (MST):\n";
    for (Edge& edge : mst_edges) {
        cout << edge.u << " - " << edge.v << " with weight " << edge.weight << endl;
    }
    cout << "Total weight of MST: " << mst_weight << endl;
}

// Hàm đọc đồ thị từ file
bool readGraphFromFile(const string& filename, int& n, vector<Edge>& edges) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return false;
    }

    int m; // Số cạnh
    inputFile >> n >> m; // Đọc số đỉnh và số cạnh

    edges.clear();
    for (int i = 0; i < m; ++i) {
        int u, v, weight;
        inputFile >> u >> v >> weight; // Đọc thông tin cạnh
        edges.push_back({ u, v, weight });
    }

    inputFile.close();
    return true;
}

void printGraph(int n, const vector<Edge>& edges) {
    cout << "Graph Information:" << endl;
    cout << "Number of vertices (n): " << n << endl;
    cout << "Number of edges (m): " << edges.size() << endl;
    cout << "Edges (u - v with weight):" << endl;

    for (const Edge& edge : edges) {
        cout << edge.u << " - " << edge.v << " with weight " << edge.weight << endl;
    }
    cout << "-----------------------------------" << endl;
}

void exportToDOT(const vector<Edge>& mst_edges, const string& outputFile) {
    ofstream dotFile(outputFile);
    if (!dotFile.is_open()) {
        cerr << "Error: Unable to write to file " << outputFile << endl;
        return;
    }

    dotFile << "graph MST {\n";
    for (const Edge& edge : mst_edges) {
        dotFile << "    " << edge.u << " -- " << edge.v << " [label=\"" << edge.weight << "\"];\n";
    }
    dotFile << "}\n";

    dotFile.close();
    cout << "DOT file exported: " << outputFile << endl;
}

int main() {
    char choice;
    do
    {
        vector<Edge> edges;
        int n = 0; // Số đỉnh    
        string filename;
        cout << "Enter the path to the graph file (e.g., D:\\Toanroirac\\Kruskal\\data\\graph.txt): ";
        cin >> filename;

        if (readGraphFromFile(filename, n, edges)) {
            printGraph(n, edges);
            kruskalMST(n, edges);
            exportToDOT(edges, "mst.dot");
        }
        else {
            cerr << "Unable to read graph data from file!" << endl;
        }
        cout << "Continue? (Y/N): ";
        cin >> choice;
        choice = tolower(choice);

        while (choice != 'y' && choice != 'n') {
            cout << "Invalid choice, please enter again (y/n): ";
            cin >> choice;
            choice = tolower(choice);
        }
    } while (choice == 'y');

    return 0;
}
