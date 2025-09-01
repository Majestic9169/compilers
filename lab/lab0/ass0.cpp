#include <iostream>
#include <string>
#include <vector>
#include <map>

class SymTable {
private:
    std::map<std::string, std::string> map;

public:
    SymTable() : map{} {};
    void insert(std::string var_name, std::string var_val) {
        map[var_name] = var_val;
    }
    std::string access(std::string var) {
        return map[var];
    }
};

class LineShell {
private:
    std::string line;
    std::string l_value;
    std::string r_value;
    SymTable sym_table{};

    void throwError(const char* err_msg, bool toExit=false) const {
        // POSIX standard escape sequence lol
        std::cerr << "\033[31mline: " << err_msg << "\033[0m" << std::endl;
        if (toExit) {
            exit(1);
        }
    }

    void removeWhitespace() {
        // remove whitespace
        std::string temp{line};
        line.clear();
        line.reserve(temp.size());
        for (auto ch: temp) {
            if (ch != '\t' && ch != ' ') {
                line.push_back(ch);
            }
        }
    }

    bool isExit() const {
        // check for exit (you can also exit with ctrl+d and ctrl+c)
        if (!line.compare("exit")) {
            return true;
        } else {
            return false;
        }
    }

    bool validateString(std::string res) const {
        for (auto ch: res) {
            // im sorry this is so convoluted
            if (!((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))) {
                throwError("invalid string reached, must only contain alphabets");
                return false;
            }
        }
        return true;
    }

    std::string caret_operator(std::string str_arg) {
        if (str_arg.find('^') != std::string::npos) {
            std::string l{str_arg.substr(0, str_arg.find('^'))};
            int r{std::stoi(str_arg.substr(str_arg.find('^') + 1, std::string::npos))};

            if (l[0] == '$') {
                l = sym_table.access(l.substr(1, std::string::npos));
            }

            // TODO: symbol lookup
            std::string res{};
            while(r--) {
                res+=l;
            }
            validateString(res);
            return res;
        } else {
            return str_arg;
        }
    }

    void concat_operator() {
        std::vector<std::string> str_args{""};
        for (size_t i = 0; i < r_value.size(); i++) {
            if (r_value.at(i) == '.') {
                str_args.push_back("");
            } else {
                str_args.back() += r_value[i];
            }
        }

        r_value.clear();
        for (std::string s: str_args) {
            r_value += caret_operator(s);
        }    
    }

    void processLine() {
        removeWhitespace();

        if (isExit()) {
            std::cout << "line: exiting\n";
            exit(0);
        }

        if (line.find('=') == std::string::npos) {
            throwError("invalid input, should be lval=rval");
            return;
        }

        // read lval and rval
        l_value = line.substr(0, line.find('='));
        r_value = line.substr(line.find('=') + 1, std::string::npos);

        // eval concat
        concat_operator(); // calls caret internally

        // insert to symtable
        sym_table.insert(l_value, r_value);
    }


    void readLine() {
        l_value.clear();
        r_value.clear();

        processLine();

        std::cout << r_value << std::endl;
    }

public:
    LineShell() : line{}, l_value{}, r_value{} {};

    void run() {
        std::cout << "line> ";
        while(std::getline(std::cin, line)) {
            readLine();
            std::cout << "line> ";
        }
        // add new line if EOF called so that next prompt displays properly
        std::cout << "\n";
    }

    
};

class Variable {
private:
    std::string var_name;
    std::string var_value;

public:
    Variable(const std::string& _var_name, const std::string& _expr) : var_name{_var_name}, var_value{} {
        
    };
};

int main() {
    LineShell line{};
	line.run();
}
