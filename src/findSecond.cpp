#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Graph.hpp"

using namespace std;

void usage(char* program_name) {
  cerr << program_name << " called with incorrect arguments." << endl;
  cerr << "Usage: " << program_name
       << " in_filename search_file out_filename"
       << endl;
  exit(-1);
}

int main(int argc, char* argv[]) {
  
  if (argc != 4) {
    usage(argv[0]);
  }
  

  char* in_filename = argv[1];
  char* search_file = argv[2];
  char* out_filename = argv[3];

  //TODO   
 /* You can call the pathfinder function from here */
Graph test;
return test.findSecondWrapper(in_filename, search_file, out_filename);
}  