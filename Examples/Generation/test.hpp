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

class Derived : public Base {
public:
	long method(int d) {
		return a + b + d;
	}
protected:
	Point derived_point;
	char c;
	int e, f;
};
