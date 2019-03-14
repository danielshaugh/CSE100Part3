/**
 * Name: Daniel Shaughnessy
 * Project: PA3, Part 3
 * */


#include "Graph.hpp"
#include <algorithm>
#include <functional>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <string>
#include <utility>
#include <queue>
#include <chrono>

#define TIMER true
using namespace std;

typedef pair<int, Node*> linkedNode;
// index, episode, episode name, segment, dialogue, type, actor, character, detail, date, series, transmission date
enum RECORD_VALS {INDEX, EPISODE, EPISODE_NAME, SEGMENT, DIALOGUE, TYPE, ACTOR, CHARACTER, DETAIL, DATE, SERIES, TRANSMISSION_DATE};

/**
 * Graph class constructor
*/
Graph::Graph(void)
    : nodes() {
      // insert root node
    }

/**
 * Graph class destructor 
*/
Graph::~Graph(void) {
  for(unordered_map<string, Node*>::iterator itr = this->nodes.begin(); itr != this->nodes.end(); itr++) {
    delete itr->second;
  }
}

/**
 * Function: insertNode
 * Public facing node insertion function
 * Parameters:
 * string type - type of node to be inserted
 * auto value - contains appropriate type checked value of insertion
 * Output:
 * true if successfully inserted, false if failed to insert.
 */
bool insertNode(string type, auto value) {
  if( type.compare("actor") || type.compare("segment") || type.compare("episode")w )
}


/**
 * Function: insertNode
 * Parameters:
 * Node* input - node to be inserted
 * Output:
 * pointer to insertedNode
 * 
 * Note: not used in current iteration of code because I lack foresight
*/
Node* Graph::insertNode(int type, auto value) {
  auto output = this->nodes.insert(make_pair(value,Node(idNumber,insertion));
  return (get<0>(output)->second);
}

/**
 * Function: loadFromFile
 * Usage: construct a graph based on input values.
 * Parameters:
 * const char* in_filename - named file to use as ifstream source.
 * Output: true if successful, false if a read failure
 * 
*/
bool Graph::loadFromFile(const char* in_filename) {
  ifstream infile(in_filename);
  Node* insertVal;
  unordered_map<int, Node*>::iterator prevNode; // pointer to store intial pair value when adding to second's list
  pair<unordered_map<string, Node*>::iterator, bool> firstNode; // stores returned value from insert function
  
  // read input into record
  while (infile) {
    string s;
    if (!getline(infile, s)) break;
    istringstream ss(s);
    vector<string> record;
    while (ss) {
      string s;
      if (!getline(ss, s, ',')) break;
      record.push_back(s);
    }
    if (record.size() != 11) {
      continue;
    }
    //get current episode
    Node* currentEpisode = this->getNode(record[EPISODE_NAME]);
    insertVal = new Node(record0);
    firstNode = this->nodes.insert({record0, insertVal});
    insertVal = new Node(record1);
    secondNode = this->nodes.insert({record1,insertVal});
    this->nodes[record0]->connectedNodes.insert(get<0>(secondNode)->second);
    this->nodes[record1]->connectedNodes.insert(get<0>(firstNode)->second);
  }

  if (!infile.eof()) {
    cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }

  infile.close();
  return true;
}

/**
 * Function: pathfinder
 * Purpose: 
 * Parameters:
 * const char* in_filename - input file containing all relationships
 * in_pairfile - file containing list of pairs to find connections for
 * output_file - output file to write shortest paths to
*/
bool Graph::pathfinder(const char* in_filename, const char* in_pairfile, const char* output_file) {
  
  auto start = chrono::high_resolution_clock::now();
  //file handling
  ifstream input(in_pairfile);
  ofstream output;
  output.open(output_file);
  // load graph
  bool loadResult = this->loadFromFile(in_filename);
  if(!loadResult)
  {
    return false;
  }
  // get search pairs
  while (input) {
    string s;
    if (!getline(input, s)) break;

    istringstream ss(s);
    vector<string> record;
    
    while (ss) {
      string s;
      if (!getline(ss, s, ' ')) break;
      record.push_back(s);
    }

    if (record.size() != 2) {
      continue;
    }

    Node* var1 = this->insertNode(stoi(record[0]));
    Node* var2 = this->insertNode(stoi(record[1]));

    
    output << this->findPath(var1, var2);     
  }

    auto stop = chrono::high_resolution_clock::now();

    if(TIMER) {
    auto duration = chrono::duration_cast<chrono::nanoseconds>(stop - start).count(); 
    cout << "duration: " << duration;
    }
  output.close();
  return true;
} 

/**
 * Function: findPath
 * Parameters
 * Node* from - initial Node to search from
 * Node* to - final Node searched for
 * 
 * Output:
 * space separated string list of all steps taken to find shortest path
 * alternately, blank line if no connection exists.
*/
string Graph::findPath(Node* from, Node* to) {

  struct listVal {
    int val1;
    Node* curr;
    Node* prev;
    listVal(int in1 = -1, Node* curr = NULL, Node* prev = NULL): val1(in1), curr(curr), prev(prev){}
  };

  if(from->value == to->value) {
    return to_string(from->value) + "\n";
  }
  //perform a breadth first search for the shortest path
  for(pair<int, Node*>val : this->nodes) {
    get<1>(val)->visited = false;
  }
  Node* currentNode = NULL;
  Node* prevNode = NULL;
  int currentLength;
  queue<listVal> currentList;
  listVal curr;
  // push intial value and length 0 to stack
  listVal input = listVal(0, from, prevNode);
  currentList.push(input);
  // while list is not empty
  while(currentList.size() != 0) {
    //get values from front of list
    curr = currentList.front();
    currentList.pop();
    currentLength = curr.val1;
    currentNode = curr.curr;
    prevNode = curr.prev;
    // set visited/previous    
    currentNode->visited = true;
    currentNode->prev = prevNode;
    // if end, return
    if(currentNode == to) {
      return this->printRoute(to);
    }
    //else keep exploring
    for(Node* temp : currentNode->connectedNodes) {
      if(!temp->visited) {
        temp->visited = true;
        currentList.push(listVal(currentLength+1, temp, currentNode));
      }
    }
    prevNode = currentNode;
  }

  // fail condition
  return "\n";
}

/**
 * Function: printRoute
 * Purpose: prints out list of nodes for findPath
 * Parameters:
 * Node* node - node from list with accurate node->prev values enable 
 * Output:
 * string list of steps taken
 */
string Graph::printRoute(Node * node) {
  Node* curr = node;
  string retVal = "";
  while(curr->prev != NULL) {
    retVal = to_string(curr->value) + " " + retVal;
    curr = curr->prev;
  }
  retVal = to_string(curr->value) + " " + retVal + '\n';
  return retVal;
}

bool Graph::socialGatheringWrapper(const char* in_filename, int k, const char* out_filename) {
  //file handling
  ofstream output;
  output.open(out_filename);
  vector<string> invitees;
  // load graph
  bool loadResult = this->loadFromFile(in_filename);
  if(!loadResult)
  {
    return false;
  }
  else {
    this->socialgathering(invitees, k);
    for(auto obj : invitees) {
      output << obj << endl;
    }
  }
  return 1;
}


/* Implement social gathering*/
//TODO
void Graph::socialgathering(vector<string>& invitees, const int& k) {
  
  // resize vector
  vector<int> inviteVal (this->nodes.size());
  // current degree count
  // current node
  Node* currNode;
  // construct a priority queue using a vector of nodes
  priority_queue<kCoreElem, vector<kCoreElem>, greater<kCoreElem>> decompQueue;
  for(unsigned i = 0; i < inviteVal.size(); i++) {
    inviteVal[i] = this->nodes.at(i)->connectedNodes.size();
    decompQueue.push( make_pair( inviteVal[i], this->nodes.at(i) ) );
  }

  // while elements in queue
  while(!decompQueue.empty()) {
    // get next node
    currNode = get<1>(decompQueue.top());
    decompQueue.pop();
    // if node already visited, move to next value
    if(currNode->visited) {
      continue;
    }
    // set as visited
    currNode->visited = true;
    // for each neighboring node
    for(auto& neighbor : currNode->connectedNodes) {
      // if neighbor node has a higher degree than current node
      if( inviteVal[neighbor->value] > inviteVal[currNode->value] ) {
        // reduce neighbor degree by 1
        inviteVal[neighbor->value] -= 1;
        // replace neighbor value in priority queue
        decompQueue.push(make_pair(inviteVal[neighbor->value],neighbor));
      }
    } 
  }

  for( unsigned i = 0; i < inviteVal.size(); i++) {
    if(inviteVal[i] >= k) {
      invitees.push_back(to_string(this->nodes.at(i)->value));
    }
  }
}


bool Graph::findSecondWrapper(const char* in_filename, const char* search_file, const char* out_filename) {
  // Open in_file
  // construct graph
  // open outfile
  // open search file
  // for each line in search file, validate
  // call findSecond for each line
  // print output to outfile
  // close streams
}

void Graph::findSecond(vector<string>& connections, string actor, int startEpisode, int endEpisode) {
  // Find relevant episodes
    // start at root
    // search all connections with weight 3
}


