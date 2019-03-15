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
 * Parameters:
 * Node* input - node to be inserted
 * Output:
 * pointer to insertedNode
 * 
 * Note: not used in current iteration of code because I lack foresight
*/
Node* Graph::insertNode(int type, string value) {
  // attempt insertion
  Node* temp = new Node(type, value);
  auto output = this->nodes.insert(make_pair(value,temp));
  // return output value
  return (output.first->second);
}

/**
 * Function: insertRelation 
 * Purpose: create a connection between two separate nodes
 * Parameters:
 * Node* node1 - pointer to first Node object
 * Node* node2 - pointer to second Node object
 * Outputs:
 *  returns false if connection existed, else true
 */
bool Graph::insertRelation(Node* node1, Node* node2) {
  // create a connection between first and second node
  auto connect1 = node1->connectedNodes.insert(make_pair(node2, node2->type));
  // create a connection between second and first node
  auto connect2 = node2->connectedNodes.insert(make_pair(node1, node1->type));

  // return true if inserted, false if already existed
  return connect1.second && connect2.second;
}

/**
 * Function: getNode
 * Usage: sets input to targeted Node.
 * Parameters: 
 * Node* ref - a pointer to a Node that is set by function
 * string key - key to search for
 * Outputs:
 * returns true if node found, else returns false
 */
bool Graph::getNode(Node* ref, const string& key) {
  auto temp = this->nodes.find(key);
  if(temp == this->nodes.end()) {
    return false;
  }
  ref = temp->second;
  return true;
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
  Node* currentEpisode = nullptr;
  Node* currentSegment = nullptr;
  Node* currentActor = nullptr;
  unordered_map<int, Node*>::iterator prevNode; // pointer to store intial pair value when adding to second's list
  pair<unordered_map<string, Node*>::iterator, bool> firstNode; // stores returned value from insert function
  bool result;
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
    // get or insert current episode
    if(!(this->getNode(currentEpisode, record[EPISODE]))) {
      currentEpisode = this->insertNode(2, record[EPISODE]);
    }
    // get or insert current segment
    if(!(this->getNode(currentSegment, record[SEGMENT]))) {
      currentSegment = this->insertNode(1, record[SEGMENT]);
    }
    // connect these if not already connected
    result = insertRelation(currentEpisode, currentSegment);
    // get or insert current actor
    if(!(this->getNode(currentActor, record[ACTOR]))) {
      currentActor = this->insertNode(0, record[ACTOR]);
    }
    // connect these if not already connected
    result = insertRelation(currentActor, currentSegment);
  }

  if (!infile.eof()) {
    cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }

  infile.close();
  return true;
}

/**
 * Function: findSecondWrapper
 * Purpose: wrapper function for findSecond (parses input) 
 * Parameters:
 * in_filename - name of graph construction file
 * search_file - name of search parameter file
 * out_filename - name of output file
 * Returns:
 * false if read error, else true
 */
bool Graph::findSecondWrapper(const char* in_filename, const char* search_file, const char* out_filename) {
  
  // call findSecond for each line
  // print output to outfile
  // close streams
  auto start = chrono::high_resolution_clock::now();
  //file handling
  ifstream input(search_file);
  ofstream output(out_filename);
  
  // load graph
  if(!(this->loadFromFile(in_filename))) {return false;}
  
  // for each itme in search_file 
  while (input) {
    string s;
    if (!getline(input, s)) break;
    istringstream ss(s);
    vector<string> record;
    while (ss) {
      string s;
      if (!getline(ss, s, ',')) break;
      record.push_back(s);
    }
    if (record.size() != 3) {
      continue;
    }

    string actor = record[0];
    int start = stoi(record[1]);
    int end = stoi(record[2]);
    output << this->findSecond(actor, start, end);
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
 * Function: findSecond
 * Purpose: given an actor and a range of episodes, find the actor who shared the most segments with him
 * Parameters:
 * actor - name of actor to search for
 * startEpisode - first episode
 * endEpisode - last episode
 * Returns:
 * outputs a string containing relative values, or a failure message if invalid parameters
 */
string Graph::findSecond(string actor, int startEpisode, int endEpisode) {

  string failureMessage = "invalid parameters: ";
  string output;
  Node* currActor;
  Node* currEpisode;

  unordered_map<Node*, int> sharedCount;


  // CHECK: valid actor
  if( !(this->getNode(currActor, actor))) {
    failureMessage += "could not find actor.\n";
    return failureMessage;
  }
  //for each episode in range
  for(int i = startEpisode; i <= endEpisode; i++) {
    // CHECK: valid episode enumeration (if does not exist or is not an episode)
    if( !(this->getNode( currEpisode, to_string( i ))) || currEpisode->type != 2) {
      failureMessage += "could not find episode.\n";
      return failureMessage;
    }
    // for each edge in current episode
    for(auto& segment : currEpisode->connectedNodes) {
      // if connected node is a segment and the actor is in that segment
      if(segment.second == 1 && segment.first->connectedNodes.find(currActor) != segment.first->connectedNodes.end()) {
          // increase the contactCount of the relevant actor
          for(auto itActor : segment.first->connectedNodes) {
            if(itActor.second == 0 && itActor.first != currActor) {
              // if exists, increment else add to map
              if(sharedCount.find(itActor.first) == sharedCount.end()) {
                sharedCount.insert(make_pair(itActor.first, 0));
              }
              else {
                sharedCount.find(itActor.first)->second += 1;
              }
          }
        }
      }
    }
  }
  
  for(auto& obj : sharedCount) {
    output += obj.first->value + " : " + to_string(obj.second) + "\n";
  }
  return output;
}

