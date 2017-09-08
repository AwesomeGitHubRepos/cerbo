#include <cassert>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::to_string;
using std::vector;


const string gmr = R"(
(foo (bar baz))
)";

//const string gmr = "";

enum cell_e { CT_NUL, CT_STR, CT_DBL, CT_VEC };

struct cell_t {
	cell_e type = CT_NUL;
	void *ptr = nullptr;
} cell_t;

//class cell;

void run_tests();
typedef struct Nothing {} Nothing; // not anything

// http://students.washington.edu/levistod/wordpress/2016/09/14/variants-in-c/
class cell;
typedef std::vector<cell> cellvec;
cell eval(const cell& c);

//void blang_assign(cell& lhs, const cell& other);

void unhandled(const string& s)
{
	cerr << "Unhandled switch case: " << s << endl;
	throw 666;
}
void unhandled(const string& s, cell_e type)
{
	unhandled(s + ", type " + to_string(type));
}

class cell {
	public:
		cell(): _dbl(0) {;};
		cell(const cell& other);  
		~cell();
		//cell(string s): value{s} {}
		//std::variant<Nothing, std::string, cellvec, double> value;
		union {
			double _dbl;
			string _str;
			cellvec _cvec;
		};
		cell_e _type = CT_NUL;
		//friend cell& operator=(const cell& other);
		cell& operator=(const cell& other) { 
			if(this != &other) {
				new(this) auto(other);
			}
			//blang_assign(*this, other); 
			return *this; 
		}
		string repr();

		void set_dbl(const double d) {
			_type = CT_DBL;
			_dbl = d;
		}

		void set_str(const string& s) { 
			_type = CT_STR;
			new(&_str) auto(s); 
		}

		void set_vec(const cellvec& vec) {
			_type = CT_VEC;
			new(&_cvec) auto(vec);
		}

		string print_cell() const;
};

		
string
cell::print_cell() const
{
	switch(_type) {
		case CT_NUL:
			return "";
		case CT_DBL:
			return to_string(_dbl);
		case CT_STR:
			return _str;
		case CT_VEC:
			return "<vec>";
		default:
			unhandled("cell::print_cell()", _type);
	}
}

string
cell::repr()
{
	string s = "<cell " + to_string(_type) + " ";
	switch(_type) {
		case CT_NUL:
			s+= "NUL";
			break;
		case CT_STR: 
			s += _str;
			break;
		case CT_DBL:
			s+= to_string(_dbl);
			break;
		case CT_VEC:
			s+= "VEC(" + to_string(_cvec.size()) + ")";
			break;
		default:
			unhandled("cell::repr()");
	}
	s+= ">";
	return s;
}
		

cell::cell(const cell& other)
{
	//cell res;
	//blang_assign(*this, other);
	//return res;
	_type = other._type;
	switch(other._type) {
		case CT_NUL: break;
		case CT_STR: 
			     //cout << "other string is " << other._str << endl;
			     new (&_str)  auto(other._str);
			     break;
		case CT_DBL: _dbl = other._dbl; break;
		case CT_VEC: 
			     new (&_cvec) auto(other._cvec);
			     break;
		default: 
			     unhandled("cell::cell(const cell& other)");
	}
}


// https://stackoverflow.com/questions/30492927/constructor-and-copy-constructor-for-class-containing-union-with-non-trivial-mem
cell::~cell()
{
	switch(_type)
	{
		case CT_NUL:
			break;
		case CT_DBL:
			break;
		case CT_STR:
			_str.~basic_string();
			break;
		case CT_VEC:
			_cvec.~vector();
			break;
		default:
			unhandled("~cell()", _type);
	//		case 
	}
}

cell
to_cell(const cellvec& cvec)
{
	cell c;
	//std::variant<cellvec> value = cvec;
	//c.value = cvec;
	c._cvec = cvec;
	return c;
}

string
get_string(const cell& c)
{
	assert(c._type == CT_STR);
	return c._str;
}


std::map<std::string, cell> vars;

void set_var(string id, cell val)
{
	auto it = vars.find(id);
#if 0
	if(it != vars.end()) vars.erase(it);
	vars[id] =  val;
#else
	if(it == vars.end()) 
		vars[id] =  val;
	else
		it->second = val;
#endif
}


cell get_var(const string& s)
{
	auto it = vars.find(s);
	if(it == vars.end()) 
		unhandled("Unbound variable: " + s);

	return it->second;
}

std::stringstream ss;
constexpr bool iswhite(char c) { return c == '\n' || c=='\t' || c==' ' || c=='\r';}
bool issym(char c) { return !(iswhite(c) || '(' || ')' || ss.eof()); }

int peek() { return ss.peek(); }

int getch() { return ss.get(); }

void
eat_white()
{
	while(iswhite(peek())) 
		getch();
}

cell cell_read();

cell
car(const cell& c)
{
	assert(c._type == CT_VEC);
	return c._cvec[0];
}

string
car_string(const cell& c)
{
	cell c1 = car(c);
	assert(c1._type == CT_STR);
	return c1._str;
}

cell
cdr(const cell& c)
{
	assert(c._type== CT_VEC);
	cell res = c;
	res._cvec.erase(res._cvec.begin());
	//res.resize(cvec.size()-1);
	//std::copy(cvec.begin()+1, cvec.end(), res);
	return res;
}

cell
blang_apply(const std::string& id, const cell& c)
{
	cell res;
	if(id== "define") {
		cell c1 = car(c);
		assert(c1._type == CT_STR);
		string id = c1._str;
		cell args = eval(car(cdr(c)));
		//cout << "apply/define " << args._dbl << endl;
		set_var(id, args);
	} else if(id == "+") {
		//cout << "blang_apply() found '+'\n";
		double d =0;
		for(const auto& a:c._cvec) {
			cell c1 = eval(a);
			assert(c1._type == CT_DBL);
			//cout << "adding " << c1._dbl << endl;
			d += c1._dbl;
		}
		res.set_dbl(d);
	}

	return res;
}



cellvec parse_list()
{
	//cout << "parse_list()...\n";

	cellvec cvec;
	//cells._type = CT_VEC;
	while(char c = getch()) {
		//cout << c ;
		if(ss.eof())
			throw std::runtime_error("parse_list(): unmatched parenthesis");
		if(c==')') 
			break;
		else {
			ss.unget();
			cell a_cell = cell_read();
			//cout << a_cell.repr() << endl;
			cvec.push_back(a_cell);
		}
	}

	//cell res;
	//res.value = cells;
	//cout << "... parse_list(): index= " << res.value.index() << "\n";
	return cvec;
}

cell
parse_atom()
{
	string str;
	int c;
	while(c=getch()){
		if(ss.eof()) break;
		if(iswhite(c)) break;
		if(c == '(' || c == ')') {ss.unget(); break;}
		str +=c;
	}

	cell res;
	try {
		double v = std::stod(str);
		res.set_dbl(v);
	} catch(const std::invalid_argument& e) {
		res.set_str(str);
	} catch(const std::out_of_range& e) {
		res.set_str(str);
	}

	//cout << "parse_atom(): <" + str + ">\n";
	return res;
}

cell
eval_string(const std::string& s)
{
	return get_var(s);
}

cell
eval_cellvec(const cell& c)
{

	cell c1 = car(c);
	assert(c1._type == CT_STR);
	string id = c1._str;
	cell c2 = cdr(c);
	cell res = blang_apply(id, c2);
	return res;
}

cell
eval(const cell& c)
{
	cell result;

	switch(c._type) {
		case CT_NUL:
			break;
		case CT_STR: 
			//cout << "TODO eval():CT_STR " << eval_string(c._str) << endl;
			result = eval_string(c._str);
			break;
		case CT_DBL: 
			result = c;
			//cout << "TOD eval()CT_DBL " << c._dbl << endl;
			break;
		case CT_VEC:
			{				  
				string id = car_string(c);
				result = blang_apply(id, cdr(c));
			}
			break;
		default:
			unhandled("eval()");
	}

	return result;

}

cell
cell_read()
{
	/*
	while(char c = getch()  ) {
		if(ss.eof()) break;
		cout << c;
	}
	*/

	cell res; // = "TODO";
	eat_white();
	char c = getch();
       	if(ss.eof()) {
		// nothing
	} else if(c=='(') {
		//res._type = CT_VEC;
		res.set_vec(parse_list());
	} else {
		ss.unget();
		res = parse_atom();
		//res._type = CT_STR;
		//res.set_str(s);
		//res.value.copy<std::string>(parse_symbol());
		}


	//cout << "cell_read(): " << res.repr() << "\n";
	return res;

}


int main()
{

	run_tests();
	//reader();
}


void
run_test(string input)
{
	cout << "=========================\n";
	cout << "Run test on: " << input << "\n";
	ss.clear();
	ss << input;
	//std::stringstream ssnew;
	//ssnew.str(input);
	//ss = ssnew;

	while(!ss.eof()){
		cell c = cell_read();
		cell e = eval(c);
		if(e._type != CT_NUL)
			cout << "Result: " << e.print_cell() << "\n";
	}

}

void run_tests()
{
	//ss << "(define (foo  bar) 12) foo   ";
	//ss << "(+ 5 6 7) (define  foo 2.2) foo ";
	run_test("(+ 5 )");
	run_test("(+ 5 6 7 )");
	run_test("(define foo 12) foo");
	run_test("(define foo (+ 12 2)) foo");
	run_test("(define foo (+ 12 2)) (+ foo 3)");
	run_test("(+ 1 2 (+ 3 4 (+ 5 6)))");
	run_test("(define foo 10) (define bar (+ foo 1)) (+ foo bar)");
	
	//cout << "type of foo is " << get_var("foo")._type << "\n";
}
