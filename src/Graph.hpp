#ifndef GRAPH_HPP
#define GRAPH_HPP

#pragma once
#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#define DEBUG true
using namespace std;

typedef pair<int, Node*> linkedNode;

// Node object for storing ids
struct Node {
  // unique identifier for every node.
  const int GUID;
  //incremental value for generating GUID
  static int GUID_val;
  // available types:  0 = "head", 1 = "actor", 2 - "segment", 3 - "episode"
  const int type;
  // stored value of node
  auto value;
  // list of connected nodes
  unordered_set<linkedNode> connectedNodes;
  // default constructor function
  Node(string type) : GUID(GUID_val++), type(type) {};
};

// static val declaration
int Node::GUID_val = 0;

class Graph {
 protected:
  // stores by type
  unordered_map<string, Node*> nodes;
  Node* insertNode(Node * node);
 public:
  Graph(void);
  ~Graph(void);

  bool insertNode(int type, auto value);

  bool insertRelation(int id1, int id2);
  bool insertRelation(Node* node1, Node* node2, );

  /* YOU CAN MODIFY THIS IF YOU LIKE , in_filename : THE INPUT FILENAME */

  bool loadFromFile(const char* in_filename);

  bool findSecondWrapper(const char* in_filename, const char* out_filename);
  void findSecond(vector<string>& connections, string actor, int startEpisode = 0, int endEpisode = 0);
};

#endif  // GRAPH_HPP

