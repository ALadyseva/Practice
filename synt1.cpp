#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
using namespace std;


enum type_of_lex{
    LEX_NULL,
    LEX_AND,
    LEX_ID,
    LEX_PROGRAM,
    LEX_INT,
    LEX_STRCONST,
    LEX_STRING,
    LEX_BOOLEAN,
    LEX_IF,
    LEX_NUM,
    LEX_ELSE,
    LEX_WHILE,
    LEX_READ,
    LEX_WRITE,
    LEX_GOTO,
    LEX_OR,
    LEX_NOT,
    LEX_FALSE,
    LEX_TRUE,
    LEX_FIN,
    LEX_FLPAREN,
    LEX_FRPAREN,
    LEX_COMMA,
    LEX_SEMICOLON,
    LEX_COLON,
    LEX_ASSIGN,
    LEX_PLUS,
    LEX_MINUS,
    LEX_TIMES,
    LEX_SLASH,
    LEX_LESS,
    LEX_GR,
    LEX_NEQ,
    LEX_EE,
    LEX_LPAREN,
    LEX_RPAREN,
    LEX_LEQ,
    LEX_GEQ,
};

class Lex{
	enum type_of_lex t_lex;
	int v_lex;
public:
	Lex(type_of_lex t = LEX_NULL, int v = 0):t_lex(t), v_lex(v){};
	type_of_lex get_type() const {return t_lex;};
	int get_value () const {return v_lex;};
	friend ostream& operator << (ostream&s, Lex l);
};

class Ident{
	char* name;
	bool declare;
	type_of_lex type;
	bool assign;
	int value;
public:
	Ident(){
		declare = false;
		assign = false;
	};
	Ident(const Ident& copy){
		delete [] name;
		strcpy(name, copy.name);
		declare = copy.declare;
		type = copy.type;
		assign = copy.assign;
		value = copy.value;
	};
	char *get_name() const {
		return name;
	};
	void put_name(const char* n){
		name = new char [strlen(n) + 1];
		strcpy(name, n);
	};
	bool get_declare() const {
		return declare;
	};
	void put_declare(){
		declare = true;
	};
	type_of_lex get_type() const {
		return type;
	};
	void put_type(type_of_lex t){
		type = t;
	};
	bool get_assign() const {
		return assign;
	};
	void put_assign(){
		assign = true;
	};
	int get_value() const {
		return value;
	};
	void put_value(int v){
		value = v;
	};
	~Ident(){
		delete [] name;
	};
};

class Scanner{
	enum state{H, IDENT, NUM, ASSIGN, EE, SLASH, COM, ALE, CHECK, NEQ, STR_CONST, DELIM};
	static const char *TW[];
	static type_of_lex words[];
	static const char *TD[];
	static type_of_lex dlms[];
	state CS;
	FILE *fp;
	char c;
	char buf[80];
	int buf_top;

	bool empty_buf() const {return buf_top == 0;};

	void clear(){
		buf_top = 0;
		for(int i = 0; i<80; i++){
			buf[i] = '\0';
		};
	};

	void add(){
		buf[buf_top++] = c;
	};

	int look(const char *buf, const char**list){
		int i = 0;
		while(list[i]){
			if(!strcmp(buf, list[i])){
				return i;
			};
			i++;
		};
		return 0;
	};

	void gc(){
		c = fgetc(fp);
	};
	friend ostream& operator << (ostream&s, Lex l);
public:
	Lex get_lex();
	Scanner(const char* program){
		fp = fopen(program, "r");
		gc();
		clear();
	};

};

class tabl_ident{
	Ident *p;
	int size;
	int top;

public:
	tabl_ident(int max_size): size(max_size){
		p = new Ident[max_size];
		top = 0;
	};
	~tabl_ident(){
		delete [] p;
	};
	Ident& operator[](int index) const {return p[index];};
	int put(const char* str){
		for(int i = 0; i < top; i++){
			if(!strcmp(str, p[i].get_name())) return i;
		};
		if (top == size){
			size*=2;
			Ident* new_tabl = new Ident[size];
			for(int i = 0; i < top; i++){
				new_tabl[i] = p[i];
			};
			p = new_tabl;
		};
		p[top].put_name(str);
		top++;
		return top-1;
	};
};

vector <const char*> tabl_str;


tabl_ident TID (100);

Lex Scanner::get_lex(){
	int d, j;
	CS = H;
	do{
		switch(CS){
		case H:
			if(isspace(c)) gc();
			else if(isalpha(c)){
				clear();
				add();
				gc();
				CS = IDENT;
			}
			else if(isdigit(c)){

				d = c - '0';
				gc();
				CS = NUM;
			}
			else if(c == '='){

				clear();
				add();
				gc();
				CS = ASSIGN;
			}
			else if(c == '/'){

				clear();
				add();
				gc();
				CS = SLASH;
			}
			else if(c == '<' || c == '>'){

				clear();
				add();
				gc();
				CS = ALE;
			}
			else if(c == '!'){

				clear();
				add();
				gc();
				CS = NEQ;
			}
			else if(c == '"'){

				clear();
				gc();
				CS = STR_CONST;
			}
			else if(c==EOF||c == '@'|| c == ',' || c  == '{' || c == '}' || c == '+' || c == '-' || c == '*' || c == '/' || c==':'||c == ';' || c == '(' || c == ')'){

				clear();
				add();
				gc();
				CS = DELIM;
			}
			else throw c;
			break;
		case IDENT:
			if(isalnum(c)){
				add();
				gc();
			}
			else{
				j = look(buf, TW);
				if (j) return (Lex(words[j], j));
				else {
					j = TID.put(buf);
					return (Lex(LEX_ID, j));
				};
			};
			break;
		case NUM:
			if(isdigit(c)){
				d = d*10 + c - '0';
				gc();
			}
			else{
				return(Lex(LEX_NUM, d));
			};
			break;
		case ASSIGN:
			if(c == '='){
				add();
				CS = EE;
			}
			else{
				j = look(buf, TD);
				return(Lex(dlms[j], j));
			};
			break;
		case EE:
			j = look(buf, TD);
			gc();
			return(Lex(dlms[j], j));
			break;
		case SLASH:
			if(c == '*'){
				CS = COM;
				gc();
			}
			else{
				j = look(buf,TD);
				return(Lex(dlms[j], j));
			};
			break;
		case COM:
			if(c == '*') CS = CHECK;
			gc();
			if(c == EOF) throw "Premature end of file";
			break;
		case CHECK:
			if(c == '/') {
				CS = H;
				gc();
			}
			else CS = COM;
			break;
		case ALE:
			if (c == '=') {
				gc();
				add();
			};
                        j = look(buf, TD);
                        return (Lex(dlms[j], j));
			break;
		case NEQ:
			if(c == '=') {
				add();
				gc();
				j = look(buf, TD);
				return(Lex(dlms[j], j));
			}
			else throw c;
			break;
		case STR_CONST:
			if(c == '"'){
				gc();
				for(long unsigned int i = 0; i < tabl_str.size(); i++){
					if(!strcmp(tabl_str[i], buf)) return (Lex(LEX_STRCONST, i));
				};
				if(!empty_buf()){
					tabl_str.push_back(buf);
				};
				return(Lex(LEX_STRCONST, tabl_str.size() - 1));
			}
			else{
				add();
				gc();
			};
			break;
		case DELIM:
			j = look(buf, TD);
			return (Lex(dlms[j],j));
			break;
	};
	} while(true);

};


type_of_lex Scanner::words[] = {
	LEX_FIN,
	LEX_AND,
	LEX_PROGRAM,
	LEX_INT,
	LEX_STRING,
	LEX_BOOLEAN,
	LEX_IF,
	LEX_ELSE,
	LEX_WHILE,
	LEX_READ,
	LEX_WRITE,
	LEX_GOTO,
	LEX_OR,
	LEX_NOT,
	LEX_FALSE,
	LEX_TRUE
};

type_of_lex Scanner::dlms[] = {
	LEX_FIN,
	LEX_FLPAREN,
	LEX_FRPAREN,
	LEX_COMMA,
	LEX_SEMICOLON,
	LEX_COLON,
	LEX_ASSIGN,
	LEX_PLUS,
	LEX_MINUS,
	LEX_TIMES,
	LEX_SLASH,
	LEX_LESS,
	LEX_GR,
	LEX_NEQ,
	LEX_EE,
	LEX_LPAREN,
	LEX_RPAREN,
	LEX_LEQ,
	LEX_GEQ,
};

const char * Scanner::TW[] =
{
    "@",
    "and",
    "program",
    "int",
    "string",
    "boolean",
    "if",
    "else",
    "while",
    "read",
    "write",
    "goto",
    "or",
    "not",
    "false",
    "true",
    NULL
};

const char* Scanner:: TD[] =
{
    "@",
    "{",
    "}",
    ",",
    ";",
	":",
    "=",
    "+",
    "-",
    "*",
    "/",
    "<",
    ">",
    "!=",
    "==",
    "(",
    ")",
    "<=",
    ">=",
	NULL
};

ostream& operator << (ostream&s, Lex l){
	if(l.t_lex == LEX_ID){
        	s<<'('<<l.t_lex<<", "<<TID[l.v_lex].get_name()<<" );";
        }
	else if(l.t_lex == LEX_STRCONST){
		s<<'('<<l.t_lex<<", "<<tabl_str[l.v_lex]<<" );";
	}
        else if(l.t_lex >= LEX_FIN){
        	s<<'('<<l.t_lex<<", "<<Scanner::TD[l.v_lex]<<" );";
       	}
        else if(l.t_lex == LEX_NUM){
        	s<<'('<<l.t_lex<<", "<<"#"<<l.v_lex<<" );";
        }
        else{
		s<<'('<<l.t_lex<<", "<<Scanner::TW[l.v_lex]<<" );";
	};
        return s;
}


class Parser{
    Lex curr_lex;
    type_of_lex c_type;
    int c_val;
    Scanner scan;
    void Program();
    void Descriptions();
    void Description();
    void Type();
    void Variable();
    void PredConstant();
    void Constant();
    void Integer();
    void Sign();
    void Identifier();
    void Operators();
    void Operator();
    void Expression();
    void ElseOperator();
    void CompoundOperator();
    void ExpressionOperator();
    void Expr1();
    void Expr2();
    void Expr3();
    void Relations();
    void Term();
    void Factor();
    void Higher();
    void Labeled();
    void Alternatives();
    void gl(){
        curr_lex = scan.get_lex();
        c_type = curr_lex.get_type();
        c_val = curr_lex.get_value();
    }
public:
    Parser (const char *program): scan(program){}
    void analyze();
};

void Parser::analyze(){
    gl();
    Program();
    cout<<"Done!"<<endl;
}

void Parser::Program(){
    if (c_type == LEX_PROGRAM)
        gl();
    else
        throw curr_lex;
    if (c_type == LEX_FLPAREN)
        gl();
    else
        throw curr_lex;
    Descriptions();
    Operators();
    if (c_type!=LEX_FRPAREN)
        throw curr_lex;
}

void Parser::Descriptions(){
    while ((c_type==LEX_INT)||(c_type==LEX_BOOLEAN)||(c_type==LEX_STRING)){
        Description();
        if (c_type==LEX_SEMICOLON)
            gl();
        else
            throw curr_lex;
    }
}

void Parser::Description(){
    Type();
    Variable();
    while (c_type==LEX_COMMA){
        gl();
        Variable();
    }
}

void Parser::Type(){
    if ((c_type==LEX_INT)||(c_type==LEX_BOOLEAN)||(c_type==LEX_STRING))
        gl();
    else
        throw curr_lex;
}

void Parser::Variable(){
        Identifier();
        PredConstant();
}

void Parser::PredConstant(){
    if (c_type==LEX_ASSIGN){
        gl();
        Constant();
    }
}

void Parser::Constant(){
    if ((c_type==LEX_STRCONST)||(c_type==LEX_TRUE)||(c_type==LEX_FALSE))
        gl();
    else
        Integer();
}

void Parser::Integer(){
    Sign();
    if (c_type==LEX_NUM)
        gl();
    else
        throw curr_lex;
}

void Parser::Sign(){
    if (c_type==LEX_PLUS||c_type==LEX_MINUS)
        gl();
}

void Parser::Operators(){
    while(c_type==LEX_IF||c_type==LEX_WHILE||c_type==LEX_READ||c_type==LEX_WRITE||c_type==LEX_GOTO||c_type==LEX_ID||c_type==LEX_FLPAREN||c_type==LEX_NOT||c_type==LEX_NUM||c_type==LEX_MINUS||c_type==LEX_PLUS||c_type==LEX_TRUE||c_type==LEX_FALSE||c_type==LEX_LPAREN||c_type==LEX_STRCONST){
            Operator();
    }
}

void Parser::Operator(){
    if (c_type==LEX_IF){
        gl();
        if(c_type==LEX_LPAREN)
            gl();
        else throw curr_lex;
        Expression();
        if(c_type==LEX_RPAREN)
            gl();
        else throw curr_lex;
        Operator();
        ElseOperator();
    }//end if
    else if (c_type==LEX_WHILE){
        gl();
        if (c_type==LEX_LPAREN)
            gl();
        else
            throw curr_lex;
        Expression();
        if (c_type==LEX_RPAREN)
            gl();
        else
            throw curr_lex;
        Operator();
    }//end while
    else if (c_type==LEX_READ){
        gl();
        if (c_type==LEX_LPAREN)
            gl();
        else
            throw curr_lex;
        Identifier();
        if (c_type==LEX_RPAREN)
            gl();
        else
            throw curr_lex;
        if (c_type==LEX_SEMICOLON)
            gl();
        else
            throw curr_lex;
    }//end read
    else if (c_type==LEX_WRITE){
        gl();
        if (c_type==LEX_LPAREN)
            gl();
        else
            throw curr_lex;
        Expression();
        while (c_type==LEX_COMMA){
            gl();
            Expression();
        }
        if (c_type==LEX_RPAREN)
            gl();
        else
            throw curr_lex;
        if (c_type==LEX_SEMICOLON)
            gl();
        else
            throw curr_lex;
    }//end write
    else if (c_type==LEX_GOTO){
        gl();
        if (c_type==LEX_ID)
            gl();
        else
            throw curr_lex;
        if (c_type==LEX_SEMICOLON)
            gl();
        else
            throw curr_lex;
    }//end goto
    else if (c_type==LEX_FLPAREN){
        CompoundOperator();
    }
    else if(c_type==LEX_ID){
        gl();
        if (c_type==LEX_COLON){
            gl();
            Operator();
        }
        else if (c_type==LEX_ASSIGN){
            gl();
            Expression();
            while(c_type==LEX_ASSIGN){
       			gl();
        		Expression();
            }
            if (c_type==LEX_SEMICOLON)
        		gl();
            else
                throw curr_lex;
        }
        else if (c_type==LEX_SEMICOLON)
            gl();
        else
            throw curr_lex;
    }//end labeled operator
    else
        throw curr_lex;
}

void Parser::ElseOperator(){
    if (c_type==LEX_ELSE){
        gl();
        Operator();
    }
}

void Parser::CompoundOperator(){
    if (c_type==LEX_FLPAREN){
        gl();
        Operators();
        if(c_type==LEX_FRPAREN)
            gl();
        else
            throw curr_lex;
    }
    else
        throw curr_lex;
}

void Parser::ExpressionOperator(){
    Expression();
    while(c_type==LEX_ASSIGN){
        gl();
        Expression();
    }
    if (c_type==LEX_SEMICOLON)
        gl();
    else
        throw curr_lex;
}

void Parser::Expression(){
    Expr1();
    while(c_type==LEX_OR){
        gl();
        Expr1();
    }
}

void Parser::Expr1(){
    Expr2();
    while(c_type==LEX_AND){
        gl();
        Expr2();
    }
}

void Parser::Expr2(){
    Expr3();
    Relations();
}

void Parser::Relations(){
    if (c_type==LEX_EE||c_type==LEX_NEQ||c_type==LEX_GR||c_type==LEX_LESS||c_type==LEX_GEQ||c_type==LEX_LEQ){
        gl();
        Expr3();
    }
}

void Parser::Expr3(){
    Term();
    while(c_type==LEX_PLUS||c_type==LEX_MINUS){
        gl();
        Term();
    }
}

void Parser::Term(){
    Factor();
    while(c_type==LEX_TIMES||c_type==LEX_SLASH){
        gl();
        Factor();
    }
}

void Parser::Factor(){
    if (c_type==LEX_MINUS){
        gl();
        Higher();
    }
    else if (c_type==LEX_NOT){
        gl();
        Higher();
    }
    else
        Higher();
}

void Parser::Higher(){
    if (c_type==LEX_NUM||c_type==LEX_ID||c_type==LEX_STRCONST||c_type==LEX_TRUE||c_type==LEX_FALSE){
        gl();
    }
    else if(c_type==LEX_LPAREN){
        gl();
        Expression();
        if (c_type==LEX_RPAREN)
            gl();
        else
            throw curr_lex;
    }
    else
        Integer();
}

void Parser::Identifier(){
    if (c_type==LEX_ID)
        gl();
    else
        throw curr_lex;
}


int main(int argc, char** argv){
	try{
		Parser prog(argv[1]);
		prog.analyze();
	}
	catch (const char* err){
		cout<<"ERROR: "<<err<<endl;
	}
	catch (char c){
		cout<<"unexpected symbol: "<<c<<endl;
	}
	catch (Lex l){
        cout<<"unexpected lexeme: "<<l<<endl;
    }
	return 0;
};

