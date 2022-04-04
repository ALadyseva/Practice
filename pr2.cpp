#include <iostream>
#include <string.h>
using namespace std;

/* Программа выполнена Ладысевой Алиной, студенткой 214 группы. */
/* Абстрактный класс - транспорт, производные классы - метро, автобус, трамвай.*/
/* В качестве чисто виртуальных функций рассматриваются функции подсчета стоимости проезда и прибыли за поездку в час пик(тривиальный случай).*/
/* Характеристики для всех видов транспорта: название, вместимость, дата выпуска. Дополнительно для метро - цвет ветки, для автобуса - продолжительность маршрута и район.*/

class Transport{
	char* name;
	int capacity, release_year;
public:
	static float pr;

	int get_cap() const {return capacity;}
	int get_year() const {return release_year;}

	void set_cap(int c) {capacity = c;}
	void set_year(int y) {release_year = y;}

	void make_name(const char* str){
		name = new char [strlen(str) + 1];
		strcpy (name, str);
	}

	char* get_name() const {return name;}

	//constructor
	Transport(const char* str, int cap, int rel): capacity(cap), release_year(rel){
		name = new char [strlen(str)+1];
		strcpy(name, str);
	}

	//destructor
	virtual ~Transport(){
		delete [] name;
	}


	//copy constructor
	Transport(const Transport &other){
		name = new char [strlen(other.get_name())+1];
		strcpy(name, other.get_name());
		capacity = other.get_cap();
		release_year = other.get_year();
	}

	//count price
	virtual float price() const = 0;

	//count income
	virtual float profit() const = 0;
};

class Subway: public Transport{
	char* color; //color of brunch
public:
	char* get_color() const {return color;}

	//constructor
	Subway(const char* str, const char* col, int cap, int rel): Transport(str, cap, rel){
		color = new char [strlen(col) + 1];
		strcpy(color, col);
	}

	//destructor
	~Subway(){
		delete [] color;
	}


	//copy constructor
	Subway(const Subway &other): Transport(other.get_name(), other.get_cap(), other.get_year()){
		color = new char [strlen(other.get_color())+1];
		strcpy(color, other.get_color());
	}


	//assignment
	Subway& operator=(const Subway &other){
		this->make_name(other.get_name());
		this->set_cap(other.get_cap());
		this->set_year(other.get_year());
		delete [] color;
		color = new char [strlen(other.get_color())+1];
		strcpy(color, other.get_color());
		return *this;
	}

	float price() const{
		return 0.6*pr;
	}

	float profit() const{
		return (this->get_cap()*price());
	}

	friend ostream& operator<<(ostream &o, const Subway &obj);
};

ostream& operator<<(ostream &o, const Subway &obj){
    o<<endl;
    o<<"Mean of transport is SUBWAY"<<endl;
	o<<"Name of the train: "<<obj.get_name()<<endl;
	o<<"Capacity of the train: "<<obj.get_cap()<<endl;
	o<<"Year of release of the vehicle: "<<obj.get_year()<<endl;
	o<<"Fare: "<<obj.price()<<endl;
	o<<"Color oh the brunch: "<<obj.get_color()<<endl;
	o<<"Profit in the peak hour: "<<obj.profit()<<endl;
	return o;
}

class Bus: public Transport{
	char* area;
	int length;
public:
	char* get_area() const {return area;}
	int get_len() const {return length;}

	//constructor
	Bus(const char* str, const char* a,int len, int cap, int rel): Transport(str, cap, rel), length(len){
		area = new char [strlen(a) + 1];
		strcpy(area, a);
	}

	//destructor
	~Bus(){
		delete [] area;
	}

	//copy constructor
	Bus(const Bus &other): Transport(other.get_name(), other.get_cap(), other.get_year()){
		area = new char [strlen(other.get_area()) + 1];
		strcpy(area, other.get_area());
		length = other.get_len();
	}

	//assignment
	Bus& operator=(const Bus &other){
		this->make_name(other.get_name());
		this->set_cap(other.get_cap());
		this->set_year(other.get_year());
		delete [] area;
		area = new char [strlen(other.get_area())+1];
		strcpy(area, other.get_area());
		length = other.get_len();
		return *this;
	}

	float price() const{
		return 0.4*pr;
	}

	float profit() const{
		return (this->get_cap()*price());
	}

	friend ostream& operator<<(ostream &o, const Bus &obj);
};

ostream& operator<<(ostream &o, const Bus &obj){
    o<<endl;
	o<<"Mean of transport is BUS"<<endl;
	o<<"Name of the bus: "<<obj.get_name()<<endl;
	o<<"Capacity of the bus: "<<obj.get_cap()<<endl;
	o<<"Year of release of the vehicle: "<<obj.get_year()<<endl;
	o<<"Fare: "<<obj.price()<<endl;
	o<<"Area: "<<obj.get_area()<<endl;
	o<<"Length of the route: "<<obj.get_len()<<" kilometers"<<endl;
	o<<"Profit in the peak hour: "<<obj.profit()<<endl;
	return o;
}

class Tram: public Transport{
public:
    //constructor
	Tram(const char* str, int cap, int rel): Transport(str, cap, rel){}

	//destructor
	~Tram(){}

	//copy constructor
	Tram(const Tram& other):Transport(other.get_name(), other.get_cap(), other.get_year()){}

	//assignment
	Tram& operator=(const Tram& other){
		this->make_name(other.get_name());
		this->set_cap(other.get_cap());
		this->set_year(other.get_year());
		return *this;
	}

	float price() const{
		return 0.2*pr;
	}

	float profit() const{
		return (this->get_cap()*price());
	}

	friend ostream& operator<<(ostream &o, const Tram &obj);
};

ostream& operator<<(ostream &o, const Tram &obj){
    o<<endl;
    o<<"Mean of transport is TRAM"<<endl;
	o<<"Name of the tram: "<<obj.get_name()<<endl;
	o<<"Capacity of the tram: "<<obj.get_cap()<<endl;
	o<<"Year of release of the vehicle: "<<obj.get_year()<<endl;
	o<<"Fare: "<<obj.price()<<endl;
	o<<"Profit in the peak hour: "<<obj.profit()<<endl;
	return o;
}

float Transport::pr=100;

int main(){
	//checking different means of transport
	cout<<"Checking next call>>> Subway s(""Metrobus #1"",""Red"", 555, 2010);"<<endl;
	Subway s("Metrobus #1","Red", 555, 2010);
    cout<<s<<endl;
	cout<<endl;

    cout<<"Checking next call>>> Bus b(""Bus #50 MAZ-203.177:3"", ""Medvedkovo"", 50, 54, 2015);"<<endl;
	Bus b("Bus #50 MAZ-203.177:3", "Medvedkovo", 50, 54, 2015);
	cout<<b<<endl;
	cout<<endl;

	cout<<"Checking next call>>> Tram t(""Tram #46 71-415"", 140, 2020);"<<endl;
	Tram t("Tram #46 71-415", 140, 2020);
	cout<<t<<endl;
	cout<<endl;

	//checking assignment
	cout<<"Checking operator of assignment>>>"<<endl;
	cout<<"The first call"<<endl;
	Subway scopy("Metrobus #999", "Yellow", 496, 2012);
	cout<<scopy<<endl;
	scopy=s;
	cout<<"The second call(after assignment)>>>"<<endl;
	cout<<scopy<<endl;
	return 0;
}
























