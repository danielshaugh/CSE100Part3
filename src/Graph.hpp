#ifndef GRAPH_HPP
#define GRAPH_HPP

#pragma once
#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#define DEBUG true
using namespace std;

// Node object for storing ids
struct Node {
  // available types: 0 = "actor", 1 - "segment", 2 - "episode"
  const int type;
  // stored value of node
  string value;
  // list of connected nodes <node*, type of node>
  unordered_map<Node*, int> connectedNodes;
  // default constructor function
  Node(int type, string value) :  type(type), value(value) {};
};
class Graph {
 protected:
  // stores by type
  unordered_map<string, Node*> nodes;
 public:
  Graph(void);
  ~Graph(void);

  Node* insertNode(int type, string value);
  bool insertRelation(Node* node1, Node* node2);
  bool getNode(Node* ref, const string& key);
  bool loadFromFile(const char* in_filename);
  bool findSecondWrapper(const char* in_filename, const char* search_file, const char* out_filename);
  string findSecond(string actor, int startEpisode = 0, int endEpisode = 0);
};

#endif  // GRAPH_HPP

