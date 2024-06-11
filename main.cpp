#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include "tokenizer.cpp"
#include "parser.cpp"
#include "interpreter.cpp"
using namespace std;


int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "ERORR: Pass the filename to compile" << endl;
    return 0;
  }
  string filename = argv[1];
  cout << "received filename : " << filename << endl;
  string fileinput, line_text;

  ifstream MyReadFile(filename);

  while (getline(MyReadFile, line_text)) {
    fileinput += line_text;
  }

  MyReadFile.close();

  cout << fileinput << endl;
  Tokenizer tokenizer(fileinput);
  tokenizer.display();
  Parser parser(tokenizer.tokens);
  unique_ptr<Expr> expression = parser.parse();
  
  // cout<<parser.current<<endl;

  Interpreter interpreter(std::move(expression));
  interpreter.evaluate();

  
  return 0;
}