/*

1.7.1 - Maximum Matching for General Graphs (Edmonds's Algorithm)

Description: Given a general directed graph, determine a subset of
the edges such that no vertex is repeated in the subset.

Complexity: O(V^3) on the number of vertices.

*/

#include <algorithm> /* fill() */
#include <iostream>
#include <vector>
using namespace std;

const int MAXN = 100;
int nodes, p[MAXN], q[MAXN], base[MAXN], match[MAXN];
vector<int> adj[MAXN];

int lca(int a, int b) {
  vector<bool> used(nodes);
  for (;;) {
    a = base[a];
    used[a] = true;
    if (match[a] == -1) break;
    a = p[match[a]];
  }
  for (;;) {
    b = base[b];
    if (used[b]) return b;
    b = p[match[b]];
  }
}

void mark_path(vector<bool> & blossom, int v, int b, int children) {
  for (; base[v] != b; v = p[match[v]]) {
    blossom[base[v]] = blossom[base[match[v]]] = true;
    p[v] = children;
    children = match[v];
  }
}

int find_path(int root) {
  vector<bool> used(nodes);
  for (int i = 0; i < nodes; ++i) {
    p[i] = -1;
    base[i] = i;
  }
  used[root] = true;
  int qh = 0, qt = 0;
  q[qt++] = root;
  while (qh < qt) {
    int v = q[qh++];
    for (int j = 0, to; j < adj[v].size(); j++) {
      to = adj[v][j];
      if (base[v] == base[to] || match[v] == to) continue;
      if (to == root || match[to] != -1 && p[match[to]] != -1) {
        int curbase = lca(v, to);
        vector<bool> blossom(nodes);
        mark_path(blossom, v, curbase, to);
        mark_path(blossom, to, curbase, v);
        for (int i = 0; i < nodes; i++)
          if (blossom[base[i]]) {
            base[i] = curbase;
            if (!used[i]) {
              used[i] = true;
              q[qt++] = i;
            }
          }
      } else if (p[to] == -1) {
        p[to] = v;
        if (match[to] == -1) return to;
        to = match[to];
        used[to] = true;
        q[qt++] = to;
      }
    }
  }
  return -1;
}

int max_matching() {
  for (int i = 0; i < nodes; i++) match[i] = -1;
  for (int i = 0; i < nodes; i++) {
    if (match[i] == -1) {
      int v = find_path(i);
      while (v != -1) {
        int pv = p[v];
        int ppv = match[pv];
        match[v] = pv;
        match[pv] = v;
        v = ppv;
      }
    }
  }
  int matches = 0;
  for (int i = 0; i < nodes; i++)
    if (match[i] != -1) matches++;
  return matches / 2;
}

int main() {
  nodes = 4;
  adj[0].push_back(1);
  adj[1].push_back(0);
  adj[1].push_back(2);
  adj[2].push_back(1);
  adj[2].push_back(3);
  adj[3].push_back(2);
  adj[0].push_back(3);
  adj[3].push_back(0);
  cout << max_matching() << endl; //2
  return 0;
}