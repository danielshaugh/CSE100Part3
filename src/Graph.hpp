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
  // list of connected nodes
  unordered_map<string, Node*> connectedNodes;
  // default constructor function
  Node(int type, string value) :  type(type), value(value) {};
};

// static val declaration
int Node::GUID_val = 0;

class Graph {
 protected:
  // stores by type
  unordered_map<string, Node*> nodes;
 public:
  Graph(void);
  ~Graph(void);

  Node* insertNode(int type, string  value);
  bool insertRelation(int id1, int id2);
  bool insertRelation(Node* node1, Node* node2, );

  /* YOU CAN MODIFY THIS IF YOU LIKE , in_filename : THE INPUT FILENAME */

  bool loadFromFile(const char* in_filename);

  bool findSecondWrapper(const char* in_filename, const char* out_filename);
  void findSecond(vector<string>& connections, string actor, int startEpisode = 0, int endEpisode = 0);
};

#endif  // GRAPH_HPP

