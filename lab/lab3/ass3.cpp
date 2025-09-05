// TEGAN JAIN
// 23CS30065

#include "lex.yy.c"
#include <cstdio>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>

using namespace std;

// ERROR STRUCT
struct Error {
  const string err_msg;
  Error(const string &_msg)
      : err_msg("\t\033[31m*** parser: error in " + _msg + "\n\033[0m") {}
};

// SYMBOL TABLE
class SymbolTable {
private:
  unordered_map<string, string> st{};
  bool contains(const string &ref) { return st.find(ref) != st.end(); }

public:
  void add(const string &ref, const string &value) {
    st[ref] = value;
    cout << "+++ Stored " + ref + " = " + value << endl;
  }

  string get(const string &ref) {
    if (contains(ref)) {
      return st[ref];
    } else {
      Error err{"undefined variable " + ref};
      cerr << endl << err.err_msg << "           ";
      return "";
    }
  }
};

// global vars
SymbolTable st{};
string currID{};
string currEXPR{};
bool is_accepted = false;

// lookahead and matched tokens
struct Token {
  int t;
  string s;
  Token(int _t, string _s) : t(_t), s(_s) {}
};

Token matched{-1, ""}, lookahead{-1, ""};

void nextToken() {
  int t = yylex();
  string s{yytext};
  matched = lookahead;
  lookahead = Token{t, s};
}

// STACK ELEMENT
class stack_node {
private:
  int state;
  variant<int, string> isu;

public:
  stack_node(int _state, variant<int, string> tmp = -1)
      : state{_state}, isu{tmp} {}

  int get_state() { return state; }

  int get_int() { return get<int>(isu); }

  string get_string() { return get<string>(isu); }
};

// STACK
class Stack {
private:
  stack<stack_node> st;

public:
  Stack() : st{} {}

  void push(stack_node node) {
    if (node.get_state() == 0) {
      printf("   %d ", node.get_state());
    } else {
      printf("-> %d ", node.get_state());
    }
    st.push(node);
  }
  stack_node top() { return st.top(); }
  void pop() {
    st.pop();
    printf("<- %d ", st.top().get_state());
  }
  void clear() {
    while (!st.empty())
      st.pop();
  }
};

Stack state_stack{};

// terminal parsers
string parseID() { return matched.s; }

string parseSTR() { return matched.s; }

string parseREF() { return st.get(matched.s.substr(1)); }

int parseNUM() { return stoi(matched.s); }

// rule table

enum NonTerminal { L, E, R, A, B, X };

map<pair<int, NonTerminal>, int> goto_table = {
    {{0, L}, 1},  {{5, E}, 4},  {{5, A}, 6},   {{5, B}, 15},
    {{6, R}, 3},  {{7, E}, 13}, {{7, A}, 6},   {{7, B}, 15},
    {{10, E}, 8}, {{10, A}, 6}, {{10, B}, 15}, {{15, X}, 14}};

// shift/reduce

void reduce(int rule) {
  NonTerminal head;
  variant<int, string> result{""};

  printf("[r%d] ", rule);

  switch (rule) {
  case 1: {
    head = NonTerminal::L;
    currEXPR = state_stack.top().get_string();
    state_stack.pop();
    state_stack.pop();
    currID = state_stack.top().get_string();
    state_stack.pop();
    break;
  }
  case 2: {
    head = NonTerminal::E;
    string rest = state_stack.top().get_string();
    state_stack.pop();
    string arg = state_stack.top().get_string();
    state_stack.pop();
    result = arg + rest;
    break;
  }
  case 3: {
    head = NonTerminal::R;
    break;
  }
  case 4: {
    head = NonTerminal::R;
    result = state_stack.top().get_string();
    state_stack.pop();
    state_stack.pop();
    break;
  }
  case 5: {
    head = NonTerminal::A;
    int num = state_stack.top().get_int();
    state_stack.pop();
    string arg1 = state_stack.top().get_string();
    state_stack.pop();
    string tmp = "";
    while (num--) {
      tmp += arg1;
    }
    result = tmp;
    break;
  }
  case 6:
    head = NonTerminal::X;
    result = 1;
    break;
  case 7: {
    head = NonTerminal::X;
    result = state_stack.top().get_int();
    state_stack.pop();
    state_stack.pop();
    break;
  }
  case 8: {
    head = NonTerminal::B;
    result = state_stack.top().get_string();
    state_stack.pop();
    break;
  }
  case 9: {
    head = NonTerminal::B;
    result = state_stack.top().get_string();
    state_stack.pop();
    break;
  }
  case 10: {
    head = NonTerminal::B;
    state_stack.pop();
    result = state_stack.top().get_string();
    state_stack.pop();
    state_stack.pop();
    break;
  }
  default:
    throw Error("rule_table");
  }

  int goto_state = goto_table[{state_stack.top().get_state(), head}];
  state_stack.push({goto_state, result});
  cout << endl;
  cout << "     ";
}

void shift(int state) {
  printf("[s%d] ", state);
  nextToken();
  variant<int, string> info;
  switch (matched.t) {
  case ID:
    info = parseID();
    break;
  case STR:
    info = parseSTR();
    break;
  case REF:
    info = parseREF();
    break;
  case NUM:
    info = parseNUM();
    break;
  default:
    info = -1;
    break;
  }
  state_stack.push({state, info});
  return;
}

void accept() {
  nextToken();
  if (matched.t != EOL) {
    throw Error("accept");
  }
  cout << "-> ACCEPT" << endl;
  st.add(currID, currEXPR);
  is_accepted = true;
}

// handlers
void handle0() {
  switch (lookahead.t) {
  case ID:
    shift(2);
    break;
  default:
    throw Error("s0");
    break;
  }
}

void handle1() {
  switch (lookahead.t) {
  case EOL:
    accept();
    break;
  default:
    throw Error("s1");
    break;
  }
}

void handle2() {
  switch (lookahead.t) {
  case ASG:
    shift(5);
    break;
  default:
    throw Error("s2");
  }
}

void handle3() {
  switch (lookahead.t) {
  case RPN:
  case EOL:
    reduce(2);
    break;
  default:
    throw Error("s3");
  }
}

void handle4() {
  switch (lookahead.t) {
  case EOL:
    reduce(1);
    break;
  default:
    throw Error("s4");
  }
}

void handle5() {
  switch (lookahead.t) {
  case STR:
    shift(11);
    break;
  case REF:
    shift(12);
    break;
  case LPN:
    shift(10);
    break;
  default:
    throw Error("s5");
  }
}

void handle6() {
  switch (lookahead.t) {
  case DOT:
    shift(7);
    break;
  case RPN:
  case EOL:
    reduce(3);
    break;
  default:
    throw Error("s6");
  }
}

void handle7() {
  switch (lookahead.t) {
  case STR:
    shift(11);
    break;
  case REF:
    shift(12);
    break;
  case LPN:
    shift(10);
    break;
  default:
    throw Error("s7");
  }
}

void handle8() {
  switch (lookahead.t) {
  case RPN:
    shift(9);
    break;
  default:
    throw Error("s8");
  }
}

void handle9() {
  switch (lookahead.t) {
  case DOT:
  case CRT:
  case RPN:
  case EOL:
    reduce(10);
    break;
  default:
    throw Error("s9");
  }
}

void handle10() {
  switch (lookahead.t) {
  case STR:
    shift(11);
    break;
  case REF:
    shift(12);
    break;
  case LPN:
    shift(10);
    break;
  default:
    throw Error("s10");
  }
}

void handle11() {
  switch (lookahead.t) {
  case DOT:
  case CRT:
  case RPN:
  case EOL:
    reduce(8);
    break;
  default:
    throw Error("s11");
  }
}

void handle12() {
  switch (lookahead.t) {
  case DOT:
  case CRT:
  case RPN:
  case EOL:
    reduce(9);
    break;
  default:
    throw Error("s12");
  }
}

void handle13() {
  switch (lookahead.t) {
  case RPN:
  case EOL:
    reduce(4);
    break;
  default:
    throw Error("s13");
  }
}

void handle14() {
  switch (lookahead.t) {
  case DOT:
  case RPN:
  case EOL:
    reduce(5);
    break;
  default:
    throw Error("s14");
  }
}

void handle15() {
  switch (lookahead.t) {
  case DOT:
  case RPN:
  case EOL:
    reduce(6);
    break;
  case CRT:
    shift(16);
    break;
  default:
    throw Error("s15");
  }
}

void handle16() {
  switch (lookahead.t) {
  case NUM:
    shift(17);
    break;
  default:
    throw Error("s16");
  }
}

void handle17() {
  switch (lookahead.t) {
  case DOT:
  case RPN:
  case EOL:
    reduce(7);
    break;
  default:
    throw Error("s17");
  }
}

// parse table
void parse_table() {
  switch (state_stack.top().get_state()) {
  case 0:
    handle0();
    break;
  case 1:
    handle1();
    break;
  case 2:
    handle2();
    break;
  case 3:
    handle3();
    break;
  case 4:
    handle4();
    break;
  case 5:
    handle5();
    break;
  case 6:
    handle6();
    break;
  case 7:
    handle7();
    break;
  case 8:
    handle8();
    break;
  case 9:
    handle9();
    break;
  case 10:
    handle10();
    break;
  case 11:
    handle11();
    break;
  case 12:
    handle12();
    break;
  case 13:
    handle13();
    break;
  case 14:
    handle14();
    break;
  case 15:
    handle15();
    break;
  case 16:
    handle16();
    break;
  case 17:
    handle17();
    break;
  }
}

// Wrappers
void Line() {
  cout << "+++ Going to parse next statement" << endl;
  is_accepted = false;
  matched = {-1, ""};
  state_stack.clear();
  state_stack.push({0});

  while (is_accepted != true) {
    parse_table();
  }
}

// MAIN
int main(int argc, char **argv) {
  cout << "debug: entered main" << endl;

  if (argc > 1) {
    yyin = fopen(argv[1], "r");
    if (!yyin) {
      cerr << "*** error: could not open " << argv[1] << endl;
      return 1;
    }
    cout << "debug: reading from file " << argv[1] << endl;
  } else {
    yyin = stdin;
  }

  try {
    nextToken();
    while (lookahead.t != END) {
      Line();
    }
  } catch (const Error &err) {
    cerr << endl << err.err_msg;
    return 1;
  }

  fclose(yyin);
  return 0;
}
