struct Point {
	double x, y;
};

class Base {
protected:
	int a;
	double b;
public:
	float method(short c) {
		return a + c;
	}
};

template<typename T>
class ExampleT {
public:
	ExampleT() {};
	
private:
	int a;
	T var;
	char c;
};

class Derived : public Base {
public:
	long method(int d) {
		return a + b + d;
	}
protected:
	Point derived_point;
	char c;
	int e, f;
	
	ExampleT<float> specified3;
	ExampleT<double> specified4;
	ExampleT<int> specified5;
	ExampleT<Point> specified6;
};

typedef ExampleT<int> ExampleTSpecified;


class Specified2 : public ExampleT<Point> {
	public:
		int jj;
		ExampleT<Derived> test;
};


typedef int defint;