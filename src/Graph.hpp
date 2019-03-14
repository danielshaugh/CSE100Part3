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
  // unique identifier for every node.
  const int GUID;
  //incremental value for generating GUID
  static int GUID_val;
  // available types: "actor", "segment", "episode"
  const string type;
  // list of connected nodes
  unordered_set<Node*> connectedNodes;
  // default constructor function
  Node(string type) : GUID(GUID_val++), type(type){};
};

// static val declaration
int Node::GUID_val = 0;

class Graph {
 protected:
  unordered_map<int, Node*> nodes;

 public:
  Graph(void);

  ~Graph(void);

  //MAYBE ADD SOME MORE METHODS HERE SO AS TO ANSWER QUESTIONS IN YOUR PA
  string printRoute(Node * node);

  Node* insertNode(Node * node);

  Node* insertNode(int idNum);

  bool insertRelation(int id1, int id2);


  /* YOU CAN MODIFY THIS IF YOU LIKE , in_filename : THE INPUT FILENAME */

  bool loadFromFile(const char* in_filename);

  bool pathfinder(const char* in_filename, const char* in_pairfile, const char* output_file);
  string findPath(Node* from, Node* to);

  bool socialGatheringWrapper(const char* in_filename, int k, const char* out_filename);
  void socialgathering(vector<string>& invitees, const int& k);

};

#endif  // GRAPH_HPP

