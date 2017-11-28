// This is a dumb file

//#include <vector>

namespace N5 {
	enum AnEnum {
		AnEnumElement,
		AnotherEnumElement,
		YetAnotherEnumElement
	};
}

namespace N1 {
	struct Point {
		struct WhyNot {
		protected:
			int whynotint;
		};
		double x, y;
		WhyNot wn;
		int WhyNot;
	};
}


class Base {
protected:
	int a;
	double b;
	N5::AnEnum daenum;
public:
	float method(short c) {
		return a + c;
	}
};

namespace N4 {
	template<typename T>
	class ExampleT {
	public:
		ExampleT() {};

	private:
		int a;
		T var;
		char c;
	};
}

class Derived : public Base {
public:
	long method(int d) {
		return a + b + d;
	}
protected:
	N1::Point derived_point;
	char c;
	int e, f;

	N4::ExampleT<float> specified3;
	N4::ExampleT<double> specified4;
	N4::ExampleT<int> specified5;
	N4::ExampleT<N1::Point> specified6;

	//std::vector<int> vecints;
};

namespace N2 {
	typedef N4::ExampleT<int> ExampleTSpecified;
	typedef int defint;
}

namespace N3 {
	class Specified2 : public N4::ExampleT<N1::Point> {
	public:
		int jj;
		N2::defint kk;
		N4::ExampleT<Derived> test;
	};

}