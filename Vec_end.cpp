#include <iostream>
#include <cmath>
#include<stdio.h>
#include<stdlib.h>

using namespace std;

class Vec{
	int *arr;
	int sz;
public:

	/* CONSTRUCTORS */
	Vec(int sz = 0) : sz(sz){
		arr = new int[sz];
		for (int i = 0; i<sz; ++i){
			arr[i]=0;
		}
	}

	Vec(int sz, int a){
		arr = new int[sz];
		for (int i = 0; i<sz; ++i){
			arr[i]=a;
		}
	}

	Vec(int l, int* vect){
    		sz = l;
    		arr = new int[sz];
    		if (!arr)
        		return;
    		for (int i = 0; i < sz; i++)
        		arr[i] = vect[i];
	}

	/* DESTRUCTOR */
	~Vec(){
		if (arr)
			delete []arr;
	}

	/* METHODS */
	int size() const{
		return sz;
	}

	bool operator==(const Vec& other) const{
		if (other.sz != sz){
			throw 1; 
        	}
		bool res = true;
		if (other.sz != sz)
			return false;
		for (int i = 0; i < sz; ++i){
			res = res && (arr[i] == other.arr[i]);
		}
		return res;
	}

	Vec& operator=(const Vec& other){
		if (this == &other){
			return *this;
		}
		delete[] arr;
		arr=new int[other.sz];
		for (int i =0; i < other.sz; ++i){
			arr[i] = other.arr[i];
		}
		sz = other.sz;
		return *this;
	}

	int &operator[](int ind) const{
		if ((ind>= sz)||(ind<0)){
			throw 2;
		}
        	return arr[ind];
    	}

	/* ARITHMETICAL OPERATORS */
	Vec operator+(const Vec& other) const{
		if (other.sz != sz){
			throw 1; 
        	}
		Vec ans(sz);
		for (int i = 0; i<sz; ++i){
			ans.arr[i] =arr[i] + other.arr[i];
		}
		return ans;
	}


	Vec operator-(const Vec& other) const{
		if (other.sz != sz){
			throw 1; 
        	}
		Vec ans(sz);
		for (int i = 0; i<sz; ++i){
			ans.arr[i] =arr[i] - other.arr[i];
		}
		return ans;
	}

	Vec operator/(int b) const{
		if (b==0){
			throw 0; /*division by zero*/
        	}
		Vec ans(sz);
		for (int i = 0; i < sz; ++i){
			ans.arr[i]=arr[i]/b;
		}
		return ans;
	}

	int maximum() const{
		int m = arr[0];
		for (int i = 1; i<sz; ++i){
			if (arr[i]>m)
				m = arr[i];
		}
		return m;
	}

	int minimum() const{
		int m = arr[0];
		for (int i = 1; i<sz; ++i){
			if (arr[i]<m)
				m = arr[i];
		}
		return m;
	}

	/*OTHER FUNCTIONS*/
	void setting(int arg, int ind){
		if ((ind>= sz)||(ind<0)){
			throw 2; 
        	}
		arr[ind]=arg;
	}
		
	int getting(int ind) const{
		if ((ind>= sz)||(ind<0)){
			throw 2;
        	}
		return arr[ind];
	}

};

Vec operator+(const Vec &other, const int &a) {
    Vec ans(other.size());
    for (int i = 0; i < other.size(); ++i) {
        ans[i] = other[i] + a;
    }
    return ans;
}


Vec operator+(const int &a, const Vec &other) {
    Vec ans(other.size());
    for (int i = 0; i < other.size(); ++i) {
        ans[i] = other[i] + a;
    }
    return ans;
}

Vec operator*(const Vec &other, const int &a) {
    Vec ans(other.size());
    for (int i = 0; i < other.size(); ++i) {
        ans[i] = other[i]*a;
    }
    return ans;
}

Vec operator*(const int &a, const Vec &other) {
    Vec ans(other.size());
    for (int i = 0; i < other.size(); ++i) {
        ans[i] = other[i] * a;
    }
    return ans;
}

ostream& operator<<(ostream& os, const Vec& other){
    for (int i = 0; i < other.size(); i++)
        os << other[i] << " ";
    os << endl;
    return os;
}

void PrintMenu(){
	printf("-------Menu-------\n");
	printf("Press -1- to copy\n");
	printf("Press -2- to add\n");
	printf("Press -3- to subtract\n");
	printf("Press -4- to divide\n");
	printf("Press -5- to multiply\n");
	printf("Press -6- to check for equality\n");
	printf("Press -7- to print the vector\n");
	printf("Press -8- to search for the maximum element\n");
	printf("Press -9- to search for the minimum element\n");
	printf("Press -10- to set the value\n");
	printf("Press -11- to get the value\n");
	printf("Press -111- to exit\n");
}

int main(){
	int num=0, c, d, e;
	int y1[4]={1, 2, 3, 4};
	int y2[4]={-1,-2, -3, -4};
	int y3[4]={1, 0, 3, 0};
	PrintMenu();
	Vec v0(4, y1);
	Vec v1(4, y2);
	Vec v2(4, y3);
	Vec **mas= new Vec* [3];
	mas[0]= &v0;
	mas[1]= &v1;
	mas[2]= &v2;
	while(num!=111){
		cin >> num;
		switch(num){
			case 1:{
				printf("Choose the vector that you want to COPY and enter it's number>>\n");
				cin >> c;
				printf("Choose the vector that you want to COPY IN and enter it's number>>\n");
				cin >> d;
				mas [d]= mas[c];
				printf("Vector #%d is copied in vector #%d\n", c, d);
				PrintMenu();
				break;
			}
			case 2:{
				printf("Choose the vector that you want to put the result in and enter it's number>>\n");
				cin >> c;
				printf("Choose the first addendum and enter it's number>>\n");
				cin >> d;
				printf("Choose the second addendum and enter it's number>>\n");
				cin >> e;
				try{
					*mas[c] = *mas[d] + *mas[e];
				}
				catch (int err){
					cout<<"Error #"<<err<<"- arrays must have the same size"<<endl;
				}
				printf("Addition is done\n");
				PrintMenu();
				break;
			}
			case 3:{
				printf("Choose the vector that you want to put the result in and enter it's number>>\n");
				cin >> c;
				printf("Choose the minuend and enter it's number>>\n");
				cin >> d;
				printf("Choose the subtrahend and enter it's number>>\n");
				cin >> e;
				try{
					*mas[c] = *mas[d] - *mas[e];
				}
				catch (int err){
					cout<<"Error #"<<err<<"- arrays must have the same size"<<endl;
				}
				printf("Subtraction is done\n");
				PrintMenu();
				break;
			}
			case 4:{
				printf("Choose the vector that you want to put the result in and enter it's number>>\n");
				cin >> c;
				printf("Choose the dividing and enter it's number>>\n");
				cin >> d;
				printf("Choose the constant\n");
				cin >> e;
				*mas[c]= *mas[d] / e;
				printf("Division is done\n");
				PrintMenu();
				break;
			}
			case 5:{
				printf("Choose the vector that you want to put the result in and enter it's number>>\n");
				cin >> c;
				printf("Choose the multiplied and enter it's number>>\n");
				cin >> d;
				printf("Choose the constant\n");
				cin >> e;
				*mas[c]= *mas[d] * e;
				printf("Multiplying is done\n");
				PrintMenu();
				break;
			}
			case 6:{
				printf("Choose the first vector and enter it's number>>\n");
				cin >> c;
				printf("Choose the second vector and enter it's number>>\n");
				cin >> d;
				try{
					if ( mas[c] == mas[d]){
						printf("Vectors are equal\n");
					}
					else{
						printf("Vectors are different\n");
					}
				}
				catch(int err){
					cout<<"Error #"<<err<<"- arrays must have the same size"<<endl;
				}
				PrintMenu();
				break;
			}
			case 7:{
				printf("Choose the vector that you want to print and enter it's number>>\n");
				cin >> c;
				cout<< *mas[c]<<endl;
				PrintMenu();
				break;
			}
			case 8:{
				printf("Choose the vector that you want to work with and enter it's number>>\n");
				cin >> c;
				cout<<(*mas[c]).maximum()<<endl;
				PrintMenu();
				break;
			}
			case 9:{
				printf("Choose the vector that you want to work with and enter it's number>>\n");
				cin >> c;
				cout<<(*mas[c]).minimum()<<endl;
				PrintMenu();
				break;
			}
			case 10:{
				printf("Choose the vector that you want to work with and enter it's number>>\n");
				cin >> c;
				printf("Enter the setting value>>\n");
				cin >> d;
				printf("Enter the position of the setting value>>\n");
				cin >> e;
				try{
					(*mas[c]).setting(d, e);
				}
				catch(int err){
					cout<<"Error #"<<err<<"- index is out of range"<<endl;
				}
				printf("Setting is done\n");
				PrintMenu();
				break;
			}
			case 11:{
				printf("Choose the vector that you want to work with and enter it's number>>\n");
				cin >> c;
				printf("Enter the position of the value that you want to get>>\n");
				cin >> d;
				try{
					e=(*mas[c]).getting(d);
					cout<<e<<endl;
				}
				catch(int err){
					cout<<"Error #"<<err<<"- index is out of range"<<endl;
				}
				PrintMenu();
				break;
			}
			default:{
                		continue;
           		}
		}
	}
		
	delete[] mas;                            
	return 0;

}


