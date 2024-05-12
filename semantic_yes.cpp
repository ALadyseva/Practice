#include <iostream>
#include <cmath>
#include <cstring>
#include <vector>
#include <algorithm>
#include <ctype.h>
#include <stack>
#include <cstdlib>
#include <cstdio>
using namespace std;

enum type_of_lex{
    LEX_NULL,
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
    LEX_NUM,
    LEX_ID,
	LEX_STRCONST
};

class Lex{
	type_of_lex t_lex;
	int v_lex;
	string str_lex;
public:
	Lex(type_of_lex t = LEX_NULL, int v = 0, string s = ""):t_lex(t), v_lex(v), str_lex(s){};
	type_of_lex get_type() const {return t_lex;};
    Lex operator=(const Lex& t){
    	t_lex=t.t_lex;
    	v_lex=t.v_lex;
    	return *this;
	}
	int get_value() const {return v_lex;};
	friend ostream& operator << (ostream&s, Lex l);
};

class Ident{
	string name;
	bool declare;
	type_of_lex type;
	bool assign;
	int value;
public:
	Ident(){
		declare = false;
		assign = false;
	}
	bool operator==(const string& s) const {
        return name == s;
    }
    Ident ( const string n ) {
        name = n;
        declare = false;
        assign = false;
    }
	string get_name() const {
		return name;
	}
	bool get_declare() const {
		return declare;
	}
	void put_declare(){
		declare = true;
	}
	type_of_lex get_type() const {
		return type;
	}
	void put_type(type_of_lex t){
		type = t;
	}
	bool get_assign() const {
		return assign;
	}
	void put_assign(){
		assign = true;
	}
	int get_value() const {
		return value;
	}
	void put_value(int v){
		value= v;
	}
};

vector <Ident> TID;

int put(const string & buf) {
    vector<Ident>::iterator k;
    if ( ( k = find ( TID.begin (), TID.end (), buf ) ) != TID.end () )
        return k - TID.begin();
    TID.push_back ( Ident(buf) );
    return TID.size () - 1;
}

class Scanner {
    FILE *fp;
    char c;
    int look (const string buf, const char ** list) {
        int i = 0;
        while (list[i]) {
            if (buf == list[i])
                return i;
            ++i;
        }
        return 0;
    }
    void gc(){
        c=fgetc(fp);
    }
public:
    static const char * TW [], * TD [];
    Scanner( const char * program ){
        if ( !(fp = fopen ( program, "r" )) )
            throw  "FILE: invalid" ;
    }
    Lex get_lex ();
};

const char * Scanner::TW[] =
{
    "",
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

Lex Scanner::get_lex(){
    enum state{H, IDENT, NUM, SLASH, COM, ALE, NEQ, STR_CONST};
	int d, j;
	string buf;
	state CS = H;
	do{
        gc();
		switch(CS){
		case H:
			if(isspace(c));
			else if(isalpha(c)){
				buf.push_back(c);
				CS = IDENT;
			}
			else if(isdigit(c)){
				d = c - '0';
				CS = NUM;
			}
			else if(c == '/'){
                buf.push_back(c);
				CS = SLASH;
			}
			else if(c == '<' || c == '>' || c == '='){
                buf.push_back(c);
				CS = ALE;
			}
			else if(c == '!'){
                buf.push_back(c);
				CS = NEQ;
			}
			else if(c == '"'){
				CS = STR_CONST;
			}
			else if (c==EOF)
                return Lex(LEX_FIN);
			else {
                buf.push_back(c);
                if (( j = look ( buf, TD) ))
                    return Lex ( (type_of_lex)( j + (int) LEX_FIN ), j );
                else
                    throw c;
			}
			break;
		case IDENT:
			if ( isalpha (c) || isdigit (c) ) {
                buf.push_back (c);
            }
            else {
                ungetc ( c, fp );
                if ( (j = look ( buf, TW) ) ) {
                    return Lex ( (type_of_lex) j, j );
                }
                else {
                    j = put ( buf );
                    return Lex (LEX_ID, j);
                }
            }
			break;
		case NUM:
			if(isdigit(c)){
				d = d*10 + c - '0';
			}
			else{
				return(Lex(LEX_NUM, d));
			};
			break;
		case SLASH:
			if(c == '*'){
                buf="";
				CS = COM;
			}
			else{
                ungetc(c, fp);
                j = look ( buf, TD );
                return Lex( (type_of_lex) ( j + (int) LEX_FIN ), j );
			}
			break;
		case COM:
			if ( c == '*'){
                gc();
                if (c == '/')
                    CS = H;
                else
                    ungetc(c, fp);
            }
            break;
		case ALE:
            if ( c == '=' ) {
                buf.push_back ( c );
                j   = look ( buf, TD );
                return Lex ( (type_of_lex) ( j + (int) LEX_FIN ), j );
            }
            else {
                ungetc ( c, fp );
                j   = look ( buf, TD );
                return Lex ( (type_of_lex) ( j + (int) LEX_FIN ), j );
            }
            break;
		case NEQ:
			if(c == '=') {
				buf.push_back(c);
                j   = look ( buf, TD );
                return Lex ( LEX_NEQ, j );
			}
			else throw c;
			break;
		case STR_CONST:
			if(c == '"')
				return Lex(LEX_STRCONST, 0, buf);
			else
                buf.push_back(c);
			break;
        }
	} while(true);
}

ostream & operator<<( ostream &s, Lex l ) {
    string t;
    if ( l.t_lex <= LEX_TRUE )
        t = Scanner::TW[l.t_lex];
    else if ( l.t_lex >= LEX_FIN && l.t_lex <= LEX_GEQ )
        t = Scanner::TD[ l.t_lex - LEX_FIN ];
    else if ( l.t_lex == LEX_NUM )
        t = "NUMB";
    else if ( l.t_lex == LEX_ID )
        t = TID[l.v_lex].get_name ();
    else
        throw l;
    s << '(' << t << ',' << l.v_lex << ");" << endl;
    return s;
}

template <class T, class T_ELEM>

void from_st (T &t, T_ELEM &x){
    x = t.top();
    t.pop();
}

class Parser{
    Lex curr_lex;
    type_of_lex c_type;
    int c_val;
    bool c_l_val, op_exp;
    bool flag, FLAG;
    Scanner scan;
    void Program();
    void Descriptions();
    void Description();
    void Type();
    void Variable(type_of_lex tp);
    void PredConstant(type_of_lex tp);
    void Constant(type_of_lex tp);
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
    void Number();
    void gl(){
        curr_lex = scan.get_lex();
        c_type = curr_lex.get_type();
        c_val = curr_lex.get_value();
    }
    stack <type_of_lex> st_lex;
    stack <int> st_int;
    vector<int> label_vector;
    vector<int> goto_vector;
    void dec(type_of_lex type); //done
    void check_id(); //done
    void check_op(); //done
    void check_not(); //done
    void eq_type_dec(); //done
    void eq_type(); //done
    void eq_bool(); //done
    void check_read();// done
    void check_un_minus(); //done
    void check_l_val(); //done
    void check_label();
public:
    Parser (const char *program): scan(program){
    }
    void analyze();
};

void Parser::dec(type_of_lex type){
    int i;
    printf("dec\n");
    while(!st_int.empty()){
        from_st(st_int,i);
        if (TID[i].get_declare())
            throw "twice declared";
        else{
            TID[i].put_declare();
            TID[i].put_type(type);
        }
    }
}

void Parser::check_id(){
    printf("checkid\n");
    if(TID[c_val].get_declare())
        st_lex.push(TID[c_val].get_type());
    else
        throw "not declared";
}


void Parser::check_op(){
    printf("check_op\n");
    type_of_lex t1, t2, op, res;
    from_st(st_lex, t2);
    from_st(st_lex, op);
    from_st(st_lex, t1);
    if (op==LEX_PLUS||op==LEX_MINUS||op==LEX_TIMES||op==LEX_SLASH){
        res=t1;
        if (t1==t2 && res==LEX_INT)
            st_lex.push(res);
        else if (op==LEX_PLUS && res==LEX_STRING && t1==t2)
            st_lex.push(res);
    }
    else if(op==LEX_EE||op==LEX_NEQ||op==LEX_LESS||op==LEX_GR||op==LEX_GEQ||op==LEX_LEQ){
        res=LEX_BOOLEAN;
        if (t1==t2 && t1==LEX_INT)
            st_lex.push(res);
        else if ((op==LEX_EE||op==LEX_NEQ||op==LEX_LESS||op==LEX_GR)&&t1==t2&&t1==LEX_STRING)
            st_lex.push(res);
        else if((op==LEX_AND||op==LEX_OR)&&t1==t2&&t1==LEX_BOOLEAN){
            res = LEX_BOOLEAN;
            st_lex.push(res);
        }
    }
    else throw "OPERATOR: wrong type";
}

void Parser::check_not(){
	printf("check_not\n");
    if (st_lex.top()!=LEX_BOOLEAN)
        throw "NOT: wrong type";
}

void Parser::eq_type_dec(){
	printf("eq_type_dec\n");
    type_of_lex t;
    from_st(st_lex, t);
    if (t!=st_lex.top())
        throw "DECLARATION: wrong type";
}

void Parser::eq_type(){
	printf("eq_type\n");
    type_of_lex t1, t2;
    from_st(st_lex, t2);
    from_st(st_lex, t1);
    if (t1!=t2)
        throw "EXPRESSION: wrong type";
}

void Parser::eq_bool(){
	printf("eq_bool\n");
    type_of_lex t;
    from_st(st_lex, t);
    if (t!=LEX_BOOLEAN)
        throw "EXPRESSION: is not boolean";
}

void Parser::check_read(){
	printf("check_read\n");
    if (!TID[c_val].get_declare())
        throw "NO DECLARATION";
    if (TID[c_val].get_type()==LEX_BOOLEAN)
        throw "READ: wrong type";
}

void Parser::check_un_minus(){
	printf("check_un_minus\n");
    if (st_lex.top()!= LEX_INT)
        throw "UNARY MINUS: wrong type";
}


void Parser::check_label(){
    int index;
	printf("check_label\n");
    for (unsigned i; i<label_vector.size(); i++){
        index = label_vector[i];
        if (TID[index].get_declare())
            throw "twice declared";
        else
            TID[index].put_declare();
    }
    vector<int>::iterator it;
    for (unsigned j=0; j<goto_vector.size(); j++){
        index = goto_vector[j];
        it = find(label_vector.begin(), label_vector.end(), index);
        if (it==label_vector.end())
            throw "not declared";
    }
}

void Parser::analyze(){
	printf("analyze\n");
    gl();
    Program();
    check_label();
    cout<<"Done!"<<endl;
}

void Parser::Program(){
	printf("program\n");
    if (c_type == LEX_PROGRAM){
        printf("program1\n");
        gl();}
    else{
        printf("program2\n");
        throw curr_lex;}
    if (c_type == LEX_FLPAREN){
        printf("program3\n");
        gl();}
    else{
        printf("program\n");
        throw curr_lex;}
    Descriptions();
    Operators();
    printf("program\n");
    if (c_type!=LEX_FRPAREN){
        printf("program\n");
        throw curr_lex;}
}

void Parser::Descriptions(){
	printf("descriptions\n");
    while ((c_type==LEX_INT)||(c_type==LEX_BOOLEAN)||(c_type==LEX_STRING)){
        printf("descriptions1\n");
        Description();
        if (c_type==LEX_SEMICOLON){
            printf("descriptions2\n");
            gl();
        }
        else{
            printf("descriptions3\n");
            throw curr_lex;}
    }
}

void Parser::Description(){
	printf("description\n");
    type_of_lex tp = curr_lex.get_type();
    Type();
    Variable(tp);
    printf("description1\n");
    while (c_type==LEX_COMMA){
        printf("description3\n");
        gl();
        Variable(tp);
        printf("description4\n");

    }
    printf("description2\n");

}

void Parser::Type(){
	printf("type\n");
    if ((c_type==LEX_INT)||(c_type==LEX_BOOLEAN)||(c_type==LEX_STRCONST)){
        gl();
        printf("type1\n");
    }
    else{
        printf("type2\n");
        throw curr_lex;
	}
}

void Parser::Variable(type_of_lex tp){
	printf("var\n");
        if (c_type==LEX_ID){
            if ( TID[curr_lex.get_value()].get_declare () ){
                printf("var1\n");
                throw "twice";}
            else{
                printf("var2\n");
                TID[curr_lex.get_value()].put_declare ();
                TID[curr_lex.get_value()].put_type (tp);
            }
            printf("var3\n");
            gl();
          /*PredConstant(tp);*/
            printf("var4\n");
        }
        else{
            printf("var5\n");
            throw curr_lex;}
	if (c_type==LEX_ASSIGN){
		gl();
		Constant(tp);
	}
}

void Parser::PredConstant(type_of_lex tp){
	printf("predconst\n");
    if (c_type==LEX_ASSIGN){
        gl();
        Constant(tp);
    }
    printf("predconst1\n");
}

void Parser::Constant(type_of_lex tp){
	printf("const\n");
    if (c_type==LEX_STRING){
        gl();
    }
    else if (c_type==LEX_FALSE || c_type==LEX_TRUE){
        gl();
        if (tp!=LEX_BOOLEAN)
            throw "NO MATCH: constant";
    }
    else{
	printf("const1\n");
        Integer();
        printf("const2\n");
    }
}

void Parser::Integer(){
	printf("int\n");
    Sign();
    if (c_type==LEX_NUM){
        printf("int1\n");
        gl();
        printf("int2\n");
    }
    else{
        printf("int3\n");
        throw curr_lex;}
}

void Parser::Sign(){
	printf("sign\n");
    if (c_type==LEX_PLUS||c_type==LEX_MINUS)
        gl();
    printf("sign2\n");
}

void Parser::Operators(){
	printf("ops\n");
    while(c_type==LEX_IF||c_type==LEX_WHILE||c_type==LEX_READ||c_type==LEX_WRITE||c_type==LEX_GOTO||c_type==LEX_ID||c_type==LEX_FLPAREN||c_type==LEX_NOT||c_type==LEX_NUM||c_type==LEX_MINUS||c_type==LEX_PLUS||c_type==LEX_TRUE||c_type==LEX_FALSE||c_type==LEX_LPAREN||c_type==LEX_STRCONST){
            Operator();
    }
}

void Parser::Operator(){
	printf("op\n");
    if (c_type==LEX_IF){
        gl();
        if(c_type==LEX_LPAREN)
            gl();
        else throw curr_lex;
        Expression();

        eq_bool();

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

        eq_bool();

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
        if (c_type==LEX_ID){
            check_read();
            gl();
        }
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
        st_lex.pop();
        while (c_type==LEX_COMMA){
            gl();
            Expression();
            st_lex.pop();
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
        if (c_type==LEX_ID){
            goto_vector.push_back(c_val);
            gl();
        }
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
    else if ( c_type == LEX_NOT || c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_ID || c_type == LEX_NUM|| c_type == LEX_STRCONST){
        flag = 1;
        FLAG = 0;
        ExpressionOperator();
        if (FLAG){
            gl();
            Operator();
        }
        else{
            st_lex.pop();
            if (c_type == LEX_SEMICOLON)
                gl();
            else
                throw curr_lex;
        }
    }
    else
        throw curr_lex;
}

void Parser::ElseOperator(){
	printf("else\n");
    if (c_type==LEX_ELSE){
        gl();
        Operator();
    }
}

void Parser::CompoundOperator(){
	printf("comp\n");
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
	printf("exprop\n");
    Expression();
    while(c_type==LEX_ASSIGN){
        gl();
        Expression();
        eq_type_dec();
    }
        if (c_type==LEX_SEMICOLON)
            gl();
        else
            throw curr_lex;

}

void Parser::Expression(){
	printf("expr\n");
    Expr1();
    while(c_type==LEX_OR){
        st_lex.push(c_type);
        gl();
        Expr1();
        check_op();
    }
}

void Parser::Expr1(){
	printf("ex1\n");
    Expr2();
    while(c_type==LEX_AND){
        st_lex.push(c_type);
        gl();
        Expr2();
        check_op();
    }
}

void Parser::Expr2(){
	printf("ex2\n");
    Expr3();
    Relations();
}

void Parser::Relations(){
	printf("rel\n");
    if (c_type==LEX_EE||c_type==LEX_NEQ||c_type==LEX_GR||c_type==LEX_LESS||c_type==LEX_GEQ||c_type==LEX_LEQ){
        st_lex.push(c_type);
        gl();
        Expr3();
        check_op();
    }
}

void Parser::Expr3(){
	printf("exp3\n");
    Term();
    while(c_type==LEX_PLUS||c_type==LEX_MINUS){
        st_lex.push(c_type);
        gl();
        Term();
        check_op();
    }
}

void Parser::Term(){
	printf("term\n");
    Factor();
    flag = 0;
    while(c_type==LEX_TIMES||c_type==LEX_SLASH){
        st_lex.push(c_type);
        gl();
        Factor();
        check_op();
    }
}

void Parser::Factor(){
	printf("fac\n");
    if (c_type==LEX_MINUS){
        gl();
        Higher();
        check_un_minus();
    }
    else if (c_type==LEX_NOT){
        gl();
        Higher();
        check_not();
    }
    else
        Higher();
}

void Parser::Higher(){
	printf("Higher\n");
    if (c_type==LEX_ID){
        if(TID[c_val].get_declare()){
            st_lex.push(TID[c_val].get_type());
            gl();
        }
        else{
            label_vector.push_back(c_val);
            gl();
            if (!flag||c_type!=LEX_COLON)
                throw "not declared";
            else
                FLAG= 1;
        }
    }
    else if (c_type==LEX_STRCONST){
        st_lex.push(LEX_STRING);
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
    else if (c_type==LEX_TRUE|| c_type==LEX_FALSE){
        st_lex.push(LEX_BOOLEAN);
        gl();
    }
    else{
        Integer();
        st_lex.push(LEX_INT);
    }
}

int main(int argc, char *argv[]){
	try{
		if (argc==2){
			Parser prog(argv[1]);
            prog.analyze();
		}
		else
			throw "Invalid number of arguments";
	}
	catch (const char* err){
		cout<<"ERROR: "<<err<<endl;
	}
	catch (char c){
		cout<<"unexpected symbol: "<<c<<endl;
	}
	catch (Lex l){
		cout<<"unexpected lexeme:"<<l<< endl;
	}
	return 0;
}
