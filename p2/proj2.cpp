/*2nd PROJECT - TugaNet
Trabalho realizado por:
Bibiana André (ist194158)
Filipe Abreu (ist1106046)
Grupo al106
*/

#include <iostream>
#include <vector>
#include <stack>

using namespace std;

#define WHITE 0 //not visited
#define GRAY 1 //visiting
#define BLACK 2 //visited

typedef struct{
    int colour = WHITE; //state of visit (WHITE = not visited)
    int scc_ID;
    int streak = 0;
    vector<int> adj; //vertexes this vertex connects to
} vertex;

int n, m; //vertices, edges
vector<vertex> graph; //the graph
vector<vertex> rev_graph; //reverse graph
stack<int> f_stack;
int max_streak = 0; //longest path

void read_input() {
    if((scanf("%d %d", &n, &m) == 2)) {
        graph = vector<vertex>(n);
        rev_graph = vector<vertex>(n);
        for(int i = 0; i < m; i++) {
            int u, v;
            if (scanf("%d %d", &u, &v) == 2 && (u != v)) {
                graph[u-1].adj.push_back(v-1); //build graph
            }
        }
    } 
}

void reverse_graph() {
    for (int i = 0; i < n; i++) {
        for (int neighbor : graph[i].adj) {
            rev_graph[neighbor].adj.push_back(i);
        }
    }
}

void DFS_visit1(int v) {
    stack<int> main_stack;
    graph[v].colour = GRAY;
    main_stack.push(v);
    
    while (!main_stack.empty()) {
        int top = main_stack.top();
        unsigned long int child;
        bool hasChildren = false;
        for(child = 0; child < graph[top].adj.size(); child++) {
            int neighbor = graph[top].adj[child];
            if(graph[neighbor].colour == WHITE) {
                hasChildren = true;
                graph[neighbor].colour = GRAY;
                main_stack.push(neighbor);
                top = main_stack.top();
            }
        }
        if (!hasChildren) {
            f_stack.push(top);
            graph[top].colour = BLACK;
            main_stack.pop();
            continue;
        }
    }   
}

int longest_streak(vector<int>& scc) {
    int max_depth = 0;
    for (int v : scc) {
        for (int neighbor : rev_graph[v].adj) {
            if (rev_graph[v].scc_ID != rev_graph[neighbor].scc_ID) {
                int depth = rev_graph[neighbor].streak + 1;
                if (depth > max_depth)
                    max_depth = depth;
            }
        }
    }
    return max_depth;
}

void DFS_visit2(int v, int type) {
    stack<int> main_stack;
    rev_graph[v].colour = GRAY;
    main_stack.push(v);
    vector<int> scc;
    
    while (!main_stack.empty()) {
        int top = main_stack.top();
        unsigned long int child;
        bool hasChildren = false;
        for(child = 0; child < rev_graph[top].adj.size(); child++) {
            int neighbor = rev_graph[top].adj[child];
            if(rev_graph[neighbor].colour == WHITE) {
                hasChildren = true;
                rev_graph[neighbor].colour = GRAY;
                main_stack.push(neighbor);
                top = main_stack.top();
            }
        }
        if (!hasChildren) {
            rev_graph[top].colour = BLACK;
            main_stack.pop();
            rev_graph[top].scc_ID = type;
            scc.push_back(top);
            continue;
        }
    }  

    int depth = longest_streak(scc);
    for (int i : scc)
        rev_graph[i].streak = depth;
    if (depth > max_streak)
        max_streak = depth;

}

void DFS1() { 
    for (int i = 0; i < n; i++) {
        if (graph[i].colour == WHITE)  
            DFS_visit1(i); 
    }
} 

int DFS2() {
    int type = 0;
    while (!f_stack.empty()) {
        int top = f_stack.top();
        if (rev_graph[top].colour != WHITE) {
            f_stack.pop();
            continue;
        }
        DFS_visit2(top, type); //scc
        f_stack.pop();
        type++;
    }
    return type;
}

int main() {
    read_input();

    if (n < 2 || m < 1) {
        printf("%d\n", max_streak);
        return 0;
    }

    reverse_graph();
    DFS1();
    int num_scc = DFS2();

    if (num_scc <= 1) 
        max_streak = 0;
    
    printf("%d\n", max_streak);
    return 0;
}