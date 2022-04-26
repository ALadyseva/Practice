#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
using namespace std;

enum type_of_lex{
        LEX_NULL,
        LEX_AND,
        LEX_BEGIN,
        LEX_BOOLEAN,
        LEX_ELSE,
        LEX_GOTO,
        LEX_IF,
        LEX_FALSE,
        LEX_INT,
        LEX_NOT,
        LEX_NUM,
        LEX_OR,
        LEX_PROGRAM,
        LEX_READ,
        LEX_CSTR,
        LEX_STRING,
        LEX_TRUE,
        LEX_WHILE,
        LEX_WRITE,
        LEX_FIN,
        LEX_SEMICOLON,
        LEX_COMMA,
        LEX_COLON,
        LEX_ASSIGN,
        LEX_LPAREN,
        LEX_RPAREN,
        LEX_FIGLPAREN,
        LEX_FIGRPAREN,
        LEX_EQEQ,
        LEX_LESS,
        LEX_GR,
        LEX_PLUS,
        LEX_MINUS,
        LEX_TIMES,
        LEX_SLASH,
        LEX_LEQ,
        LEX_GEQ,
        LEX_NEQ,
        LEX_ID
};


class Lex{
	type_of_lex t_lex;
	int v_lex;
public:
	Lex(type_of_lex t = LEX_NULL, int v = 0){
        t_lex = t;
        v_lex = v;
    };
	type_of_lex get_type() const {return t_lex;}
	int get_value () const {return v_lex;}
	friend ostream& operator << (ostream&s, Lex l);
};


class Ident{
	char* name;
	bool declare;
	type_of_lex type;
	bool assign;
	int value;
public:
    //CONSTRUCTOR
	Ident(){
		declare = false;
		assign = false;
	};
	//COPY CONSTRUCTOR
	Ident(const Ident& obj){
		delete [] name;
        strcpy(name, obj.name);
		declare = obj.declare;
		type = obj.type;
		assign = obj.assign;
		value = obj.value;
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
	//DESTRUCTOR
	~Ident(){
		delete [] name;
	};
};


class tabl_ident{
	Ident *p;
	int size;
	int top;

public:
	tabl_ident(int max_size): size(max_size){
		p = new Ident[size];
		top = 0;
	};
	~tabl_ident(){
		delete [] p;
	};
	Ident& operator[](int k) const {return p[k];};
	int put(const char* buf){
		for(int i = 1; i < top; ++i)
			if(!strcmp(buf, p[i].get_name()))
                return i;
		p[top].put_name(buf);
		++top;
		return top-1;
	};
};


class Scanner{
	enum state{H, IDENT, NUMB, ALE, ASSIGN, SLASH, EQEQ, NEQ, IS_OK, CSTR, DELIM, COM};
	static const char *TW[];
	static type_of_lex words[];
	static const char *TD[];
	static type_of_lex dlms[];
	state CS;
	FILE *fp;
	char c;
	char buf[80];
	int buf_top;

	void clear(){
		buf_top = 0;
		for(int i = 0; i<80; i++){
			buf[i] = '\0';
		};
	};

	void add(){
		buf[buf_top++] = c;
	};

	int look(const char *buf, const char **list){
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
	bool Is_Empty() const {
	    return buf_top == 0;
    }
	friend ostream& operator << (ostream&s, Lex l);
public:
	Lex get_lex();
	Scanner(const char* program){
		fp = fopen(program, "r");
		CS=H;
		clear();
		gc();
	};

};


const char * Scanner::TW[] =
{
        ""   //0
        "and", //1
        "boolean", //2
        "else", //3
        "goto", //4
        "if", //5
        "false", //6
        "int", //7
        "not", //8
        "or", //9
        "program", //10
        "read", //11
        "string", //12
        "true", //13
        "while", //14
        "write", //15
};

const char* Scanner:: TD[] =
{
        "" //0
        "@", //1
        ";", //2
        ",", //3
        ":", //4
        "=", //5
        "(", //6
        ")", //7
        "{", //8
        "}", //9
        "==", //10
        "<", //11
        ">", //12
        "+", //13
        "-", //14
        "*", //15
        "/", //16
        "<=", //17
        ">=", //18
        "!=" //19
};

tabl_ident TID (100);

type_of_lex Scanner::words[] = {
    LEX_NULL,
    LEX_AND,
    LEX_BEGIN,
    LEX_BOOLEAN,
    LEX_ELSE,
    LEX_GOTO,
    LEX_IF,
    LEX_FALSE,
    LEX_INT,
    LEX_NOT,
    LEX_OR,
    LEX_PROGRAM,
    LEX_READ,
    LEX_STRING,
    LEX_TRUE,
    LEX_WHILE,
    LEX_WRITE,
};

type_of_lex Scanner::dlms[] = {
	LEX_FIN,
    LEX_SEMICOLON,
    LEX_COMMA,
    LEX_COLON,
    LEX_ASSIGN,
    LEX_LPAREN,
    LEX_RPAREN,
    LEX_FIGLPAREN,
    LEX_FIGRPAREN,
    LEX_EQEQ,
    LEX_LESS,
    LEX_GR,
    LEX_PLUS,
    LEX_MINUS,
    LEX_TIMES,
    LEX_SLASH,
    LEX_LEQ,
    LEX_GEQ,
    LEX_NEQ
};

vector <const char*> strings;

ostream& operator <<(ostream&s, Lex l){
	if(l.t_lex == LEX_ID){
        	s<<"("<<l.t_lex<<", "<<TID[l.v_lex].get_name()<<");";
	}
    else if(l.t_lex == LEX_NUM){
        	s<<"("<<l.t_lex<<", "<<l.v_lex<<");";
    }
	else if(l.t_lex == LEX_CSTR){
		s<<"("<<l.t_lex<<", "<<strings[l.v_lex]<<");";
	}
    else if(l.t_lex >= LEX_FIN){
        	s<<"("<<l.t_lex<<", "<<Scanner::TD[l.v_lex]<<");";
    }
    else{
		s<<"("<<l.t_lex<<", "<<Scanner::TW[l.v_lex]<<");";
	};
    return s;
};

Lex Scanner::get_lex(){
	int d, j;
	CS = H;
	do{
		switch(CS){
		case H:
			if(isspace(c))
                gc();
			else if(isalpha(c)){
                CS = IDENT;
				clear();
				add();
				gc();

			}
			else if(isdigit(c)){
			    CS = NUMB;
				d = c - '0';
				gc();

			}
			else if(c == '<' || c == '>' || c==':'){
			    CS = ALE;
				clear();
				add();
				gc();

			}
			else if(c == '!'){
			    CS = NEQ;
				clear();
				add();
				gc();

			}
			else if(c == '='){
			    CS = ASSIGN;
				clear();
				add();
				gc();

			}
			else if(c == '/'){
			    CS = SLASH;
				clear();
				add();
				gc();

			}
			else if(c == '"'){
			    CS = CSTR;
				clear();
				gc();

			}
			else if(c=='@' || c == ',' || c == ';' || c  == '{' || c == '}' || c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')'){
			    CS = DELIM;
				clear();
				add();
				gc();

			}
			else throw c;
			break;
		case IDENT:
			if(isalnum(c)){
				add();
				gc();
			}
			else{
				if((j = look(buf, TW))!=0)
                    return Lex(words[j], j);
				else {
					j = TID.put(buf);
					return (Lex(LEX_ID, j));
				};
			};
			break;
		case NUMB:
			if(isdigit(c)){
				d = d*10 + c - '0';
				gc();
			}
			else{
				return(Lex(LEX_NUM, d));
			};
			break;
		case ALE:
			if (c == '=') {
				gc();
				add();
			};
            j = look(buf, TD);
            return (Lex(dlms[j], j));
			break;
		case ASSIGN:
			if(c == '='){
				add();
				CS = EQEQ;
			}
			else{
				j = look(buf, TD);
				return(Lex(dlms[j], j));
			};
			break;
		case EQEQ:
			j = look(buf, TD);
			gc();
			return(Lex(dlms[j], j));
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
			if(c == '*')
                CS = IS_OK;
			gc();
			if(c == '@' || c==EOF)
                throw "Close the comment.";
			break;
		case IS_OK:
			if(c == '/') {
				CS = H;
				gc();
			}
			else CS = COM;
			break;
		case CSTR:
			if(c == '"'){
				gc();
				for(long unsigned int i = 0; i < strings.size(); i++){
					if(strcmp(strings[i], buf)==0)
                        return (Lex(LEX_CSTR, i));
				};
				if(!Is_Empty()){
					strings.push_back(buf);
				};
				return(Lex(LEX_CSTR, strings.size() - 1));
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


int main(int argc, char** argv){
	try{
		Scanner prog(argv[1]);
		Lex l;
		while((l = prog.get_lex()).get_type() != LEX_FIN){
			cout<<l<<endl;
		};
	}
	catch (const char* err){
		cout<<"ERROR: "<<err<<endl;
	}
	catch (char c){
		cout<<"WRONG SYMBOL: "<<c<<endl;
	}
	return 0;
};
