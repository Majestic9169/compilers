#include "lex.yy.c"
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

struct Error {
  const string err_msg;
  Error(const string &_msg)
      : err_msg("\t\033[31m*** parser: error in " + _msg + "\n\033[0m") {}
};

class SymbolTable {
private:
  unordered_map<string, string> st{};
  bool contains(const string &ref) { return st.find(ref) != st.end(); }

public:
  void add(const string &ref, const string &value) {
    st[ref] = value;
    cout << "+++ parser: set $" + ref + " = " + value << endl;
  }

  string get(const string &ref) {
    if (contains(ref))
      return st[ref];
    throw Error{"invalid variable " + ref};
  }
};

// global state
int MATCHED_TOKEN{};
int LOOKAHEAD_TOKEN{};
string buf{};
SymbolTable sym{};

int nextToken() {
  MATCHED_TOKEN = LOOKAHEAD_TOKEN;
  buf = string{yytext};
  LOOKAHEAD_TOKEN = yylex();
  // cout << "debug: LOOKAHEAD_TOKEN = " << LOOKAHEAD_TOKEN << endl;
  // cout << "debug: yytext = " << yytext << endl;
  return LOOKAHEAD_TOKEN;
}

void digestor(int token) {
  nextToken();
  if (MATCHED_TOKEN != token) {
    throw Error{"invalid terminal " + to_string(MATCHED_TOKEN)};
  }
}

// function prototypes
string BASE();
int XPNT();
string ARG();
string REST();
string EXPR();
void LINE();

string BASE() {
  // cout << "debug: BASE" << endl;
  switch (LOOKAHEAD_TOKEN) {
  case STR: {
    digestor(STR);
    string s{buf};
    return s;
  }
  case REF: {
    digestor(REF);
    string s{buf};
    return sym.get(s.substr(1));
  }
  case LPN: {
    digestor(LPN);
    string exp = EXPR();
    digestor(RPN);
    return exp;
  }
  default:
    throw Error{"BASE"};
  }
}

int XPNT() {
  // cout << "debug: XPNT" << endl;
  switch (LOOKAHEAD_TOKEN) {
  case RPN:
  case DOT:
  case EOL:
    return 1;
  case CRT: {
    digestor(CRT);
    digestor(NUM);
    return stoi(string{buf});
  }
  default:
    throw Error{"XPNT"};
  }
}

string ARG() {
  // cout << "debug: ARG" << endl;
  switch (LOOKAHEAD_TOKEN) {
  case STR:
  case REF:
  case LPN: {
    string base = BASE();
    int xpnt = XPNT();
    string res;
    while (xpnt--)
      res += base;
    return res;
  }
  default:
    throw Error{"ARG"};
  }
}

string REST() {
  // cout << "debug: REST" << endl;
  switch (LOOKAHEAD_TOKEN) {
  case RPN:
  case EOL:
    return "";
  case DOT:
    digestor(DOT);
    return EXPR();
  default:
    throw Error{"REST"};
  }
}

string EXPR() {
  // cout << "debug: EXPR" << endl;
  switch (LOOKAHEAD_TOKEN) {
  case STR:
  case REF:
  case LPN: {
    string arg = ARG();
    string rest = REST();
    return arg + rest;
  }
  default:
    throw Error{"EXPR"};
  }
}

void LINE() {
  // cout << "debug: LINE" << endl;
  switch (LOOKAHEAD_TOKEN) {
  case ID: {
    digestor(ID);
    string ref{buf};
    digestor(ASG);
    string res = EXPR();
    sym.add(ref, res);
    digestor(EOL);
    break;
  }
  default:
    throw Error{"LINE"};
  }
}

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

  LOOKAHEAD_TOKEN = yylex();
  buf = yytext;

  try {
    while (LOOKAHEAD_TOKEN != END) {
      LINE();
    }
  } catch (const Error &err) {
    cerr << err.err_msg;
    return 1;
  }

  fclose(yyin);
  return 0;
}
