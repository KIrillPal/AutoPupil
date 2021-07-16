#pragma once
#include "BigLong.h";

class BigFixed
{
public:
	static const int BASE_C = 9;
	static const ll BASE = 1e9;
	static int PREC_SIZE, PRECISION;
	static BigFixed PI, E;
	static void set_precision(int nv);
	static int get_precision();
	int precision = 0, prec_size = 0, prec_full = 0;
	BigFixed();
	BigFixed(BigLong& b);
	BigFixed(std::string decimal);
	BigFixed(double inp);
	~BigFixed();
	BigFixed* pi, * e;
	void normalize(int n = -1);
	void set_sign(bool sign);
	bool get_sign();
	bool operator > (double x);
	bool operator < (BigFixed& r);
	bool operator == (BigFixed& r);
	BigFixed operator+ (BigFixed& r);
	BigFixed operator- (BigFixed& r);
	BigFixed operator* (BigFixed& r);
	BigFixed operator/ (BigFixed& r);
	BigFixed operator% (BigFixed& r);
	BigFixed operator^ (BigFixed& pow);
	BigFixed operator^ (BigLong pow);
	BigFixed sin_();
	BigFixed cos_();
	BigFixed tan_();
	BigFixed cot_();
	BigFixed log_(BigFixed& a);
	double to_double();
	std::string show();
	BigFixed root(int pow, int prec = -1);
	BigFixed BigFixed::log10(BigFixed b, int prec = -1);
private:
	BigLong core;
	void init();
};