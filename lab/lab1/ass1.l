%{
#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

bool after_op{false};

string var_name{};
string argument_string{};
string result_string{};

int range_l{-1};
int range_r{-1};

struct Error {
  const std::string err_msg;
  Error(string _msg) : err_msg("\t\033[31m*** " + _msg + ". Line " + to_string(yylineno) + " cannot be processed\n\033[0m") {};
};

class SymbolTable {
private:
	unordered_map<string, string> st{};
	bool contains(const string& _id) {
		return st.find(_id) != st.end();
	}
public:
	void add_var(string _var_name, string res) {
		string temp = "$";
		temp += _var_name;
		st[temp] = res;	
		// cout << "\t" << temp << " set to " << res << endl;
	} 
	string look_up(string _id) {
		if (contains(_id))	{
			return st[_id];
		} else {
			string temp = "\t\033[31m*** ";
			temp += "Reference to undefined variable \"";
			temp += _id;
			temp += "\"";
			temp += "\033[0m";
			cerr << temp << endl;
			return "";
		}
	}
} symbol_table;

void reset() {
	yylineno++;
	result_string.clear();
	argument_string.clear();
	var_name.clear();
	after_op = false;
}

string remove_ws(string str) {
	string temp{};
	for (auto c: str) {
		if (c != ' ' && c != '\t') 
			temp += c;
	}
	return temp;
}

string handle_range(string str, int l, int r) {
	if (l > r) {
		Error err{"Invalid range"};
		cerr << err.err_msg;
		return "";
	}
	string temp{};
	for (int i = l; i <= r && i < str.size(); i++) {
		temp += str[i];
	}
	// cout << "\tsubstring of " << str << " from " << l << " to " << r << " is " << temp << endl;
	return temp;
}

string handle_exp(string buf, int n) {
	string res{""};
	while(n--) {
		res += buf;
	}
	return res;
}

%}

%x R_VALUE
%x RANGE_SELECTOR
%x EXPONENT
%x RANGE_END
%x RANGE_PREFIX
%x RANGE_SUFFIX
%x ERROR

wsop							[ \t]*
ws								[ \t]+
wsnl							[ \t\n]+
letter_						[a-zA-Z_]
letterdig_				[a-zA-Z0-9_]
nneg_int					[0-9]+
neg_int 					-[0-9]+
nl 								"\n"

caret_op 					"^"
dot_op 						"."
ref_op 						"$"
assignment_op 		"="
range_open 				"["
range_close 			"]"
range_separate 		","
range_prefix 			"<"
range_suffix 			">"

l_val							{letter_}{letterdig_}*
variable 					{ref_op}{l_val}
alphabet_string 	[a-zA-Z]+

%% 

<*>{wsop} {}

exit { 
	printf("exiting\n");
	exit(0);
}

%{/*****************************************************************
 *                                                                 *
 *                     INITIAL STATE (L_VALUE)                     *
 *                                                                 *
 *******************************************************************/%}

^{wsop}{l_val} { 
	var_name = remove_ws(string{yytext});
}

{l_val} { 
	// only one l_val possible
	Error err{"Invalid lvalue"};
	cerr << err.err_msg;
	BEGIN(ERROR);
}

{assignment_op} { 
	if (var_name == "") {
		Error err{"Invalid lvalue"};
		cerr << err.err_msg;
		BEGIN(ERROR);
	} else {
		BEGIN(R_VALUE); 
	}
}

<INITIAL>\n {
	Error err{"Assignment operator missing"}; 
	cerr << err.err_msg;
	reset();
	BEGIN(INITIAL);
}

<INITIAL>[^=] { 
	Error err{"Invalid character '" + string{yytext} + "' found"};
	cerr << err.err_msg;
	BEGIN(ERROR);
}

%{/*****************************************************************
 *                                                                 *
 *                         R_VALUE                                 *
 *                                                                 *
 *******************************************************************/%}

<R_VALUE>{variable}{wsop}/{caret_op} {
	if (after_op) {
		Error err{"Invalid character '" + string{yytext[0]} + "' found"};
		cerr << err.err_msg;
		BEGIN(ERROR);
	} else {
		string identifier{yytext};
		identifier = remove_ws(identifier);
		// cout << "\tidentifier " << identifier << endl;
		argument_string = symbol_table.look_up(identifier);
		BEGIN(EXPONENT);
	}
}

<R_VALUE>{alphabet_string}{wsop}/{caret_op} {
	if (after_op) {
		Error err{"Invalid character '" + string{yytext[0]} + "' found"};
		cerr << err.err_msg;
		BEGIN(ERROR);
	} else {
		string str{yytext};
		str = remove_ws(str);
		argument_string = str;
		// cout << "\tstring literal " << argument_string << endl;
		BEGIN(EXPONENT);
	}
}

<R_VALUE>{variable}{wsop}/{range_open} {
	if (after_op) {
		Error err{"Invalid character '" + string{yytext[0]} + "' found"};
		cerr << err.err_msg;
		BEGIN(ERROR);
	} else {
		string identifier{yytext};
		identifier = remove_ws(identifier);
		// cout << "\tidentifier " << identifier << endl;
		argument_string = symbol_table.look_up(identifier);
		BEGIN(RANGE_SELECTOR);
	}
}

<R_VALUE>{alphabet_string}{wsop}/{range_open} {
	if (after_op) {
		Error err{"Invalid character '" + string{yytext[0]} + "' found"};
		cerr << err.err_msg;
		BEGIN(ERROR);
	} else {
		string str{yytext};
		str = remove_ws(str);
		argument_string = str;
		// cout << "\tstring literal " << argument_string << endl;
		BEGIN(RANGE_SELECTOR);
	}
}

<R_VALUE>{variable}{wsop}/{dot_op} {
	if (after_op) {
		Error err{"Invalid character '" + string{yytext[0]} + "' found"};
		cerr << err.err_msg;
		BEGIN(ERROR);
	} else {
		string identifier{yytext};
		identifier = remove_ws(identifier);
		// cout << "\tidentifier " << identifier << endl;
		argument_string = symbol_table.look_up(identifier);
		result_string += argument_string;
		argument_string.clear();
		after_op = false;
	}
}

<R_VALUE>{alphabet_string}{wsop}/{dot_op} {
	if (after_op) {
		Error err{"Invalid character '" + string{yytext[0]} + "' found"};
		cerr << err.err_msg;
		BEGIN(ERROR);
	} else {
		string str{yytext};
		str = remove_ws(str);
		argument_string = str;
		// cout << "\tstring literal " << argument_string << endl;
		result_string += argument_string;
		argument_string.clear();
		after_op = false;
	}
}

<R_VALUE>{variable} {
	if (after_op) {
		Error err{"Invalid character '" + string{yytext[0]} + "' found"};
		cerr << err.err_msg;
		BEGIN(ERROR);
	} else {
		string identifier{yytext};
		identifier = remove_ws(identifier);
		// cout << "\tidentifier " << identifier << endl;
		argument_string = symbol_table.look_up(identifier);
		after_op = true;
	}
}

<R_VALUE>{alphabet_string} {
	if (after_op) {
		Error err{"Invalid character '" + string{yytext[0]} + "' found"};
		cerr << err.err_msg;
		BEGIN(ERROR);
	} else {
		string str{yytext};
		str = remove_ws(str);
		argument_string = str;
		// cout << "\tstring literal " << argument_string << endl;
		after_op = true;
	}
}

<R_VALUE>{range_open} { 
	if (after_op) {
		Error err{"Invalid range selector"};
		cerr << err.err_msg;
		BEGIN(ERROR);
	} else {
		BEGIN(RANGE_SELECTOR); 
	}
}

<R_VALUE>{wsop}{caret_op} { 
	if (after_op) {
		Error err{"Invalid exponent specifier"};
		cerr << err.err_msg;
		BEGIN(ERROR);
	} else {
		BEGIN(EXPONENT); 
	}
}

<R_VALUE>{dot_op} {
	result_string += argument_string;
	argument_string.clear();
	after_op = false;
}

<R_VALUE>. { 
	Error err{"Invalid character '" + string{yytext} + "' found"};
	cerr << err.err_msg;
	BEGIN(ERROR);
}

<R_VALUE>\n { 
	result_string += argument_string;
	if (result_string == "") {
		Error err{"No r-value"};
		cerr << err.err_msg;
		reset();
		BEGIN(INITIAL);
	} else {
		symbol_table.add_var(var_name, result_string);
		cout << "+++ Line " << yylineno << " processed: " << var_name << " is set to " << remove_ws(result_string) << endl;
		reset();
		BEGIN(INITIAL); 
	}
}

%{/*****************************************************************
 *                                                                 *
 *                         RANGE_SELECTOR                          *
 *                                                                 *
 *******************************************************************/%}

<RANGE_SELECTOR>{range_prefix} { BEGIN(RANGE_PREFIX); }
<RANGE_SELECTOR>{range_suffix} { BEGIN(RANGE_SUFFIX); }

<RANGE_SELECTOR,RANGE_PREFIX,RANGE_SUFFIX,RANGE_END>{neg_int} {
	Error err{"Invalid character '-' found"};
	cerr << err.err_msg;
	BEGIN(ERROR);
}

<RANGE_PREFIX>{nneg_int} { 
	range_l = atoi(yytext);
	argument_string = handle_range(argument_string, 0, range_l - 1);
}
<RANGE_PREFIX>{range_close} { BEGIN(R_VALUE); }

<RANGE_SUFFIX>{nneg_int} {
	range_l = atoi(yytext);
	argument_string = handle_range(argument_string, argument_string.size()-range_l, argument_string.size() - 1);
}
<RANGE_SUFFIX>{range_close} { 
	if (range_l == -1) {
		Error err{"Invalid character '" + string{yytext} + "' found"};
		cerr << err.err_msg;
		BEGIN(ERROR);
	} else {
		BEGIN(R_VALUE); 
	}
}

<RANGE_SELECTOR>{nneg_int} { 
	range_l = atoi(yytext);
	BEGIN(RANGE_END); 
} 

<RANGE_END>{range_separate} {}
<RANGE_END>{nneg_int} { range_r = atoi(yytext); }
<RANGE_END>{range_close} {
	if (range_r != -1)
		argument_string = handle_range(argument_string, range_l, range_r);
	else 
		argument_string = handle_range(argument_string, range_l, range_l);
	range_r = -1;
	BEGIN(R_VALUE);
}

%{/*****************************************************************
 *                                                                 *
 *                         EXPONENT                                *
 *                                                                 *
 *******************************************************************/%}

<EXPONENT>"-" {
	Error err{"Invalid character '-' found"};
	cerr << err.err_msg;
	BEGIN(ERROR);
}

<EXPONENT>{nneg_int} {
	int n = atoi(yytext);
	argument_string = handle_exp(argument_string, n);
	after_op = true;
	BEGIN(R_VALUE);
}

%{/*****************************************************************
 *                                                                 *
 *                           ERROR                                 *
 *                                                                 *
 *******************************************************************/%}

<ERROR>. {}
<ERROR>\n { 	
	reset();
	BEGIN(INITIAL); 
}

<*>. {}

<*>{nl} {
	reset();
	BEGIN(INITIAL); 
}

<<EOF>> { exit(0); }

%% 

int yywrap() {
	return 1;
}

int main(int argc, char** argv) {
	if (argc >1) {
		yyin = (FILE*) fopen(argv[1], "r");
	} else {
		yyin = stdin;
	}
	yylex();
	fclose(yyin);
	return 0;
}
