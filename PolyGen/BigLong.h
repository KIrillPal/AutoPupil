#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

typedef unsigned long long ull;
typedef long long ll;

class BigLong
{
	static const int BASE_C = 9;
	static const ll BASE = 1e9;
public:
	int isnan = 0, inf_size = 1e5;
	BigLong();
	BigLong(std::string decimal);
	BigLong(ll x, int n = 1);
	~BigLong();
	bool get_sign() { return sign_; };
	const int get_length() { return digits_.size(); };
	void set_sign(bool sign);
	const bool operator <(BigLong& b);
	const bool operator ==(BigLong& b);
	BigLong operator -() const;
	BigLong operator +(BigLong& b);
	BigLong operator -(BigLong& b);
	BigLong operator *(BigLong& b);
	BigLong operator /(BigLong& b);
	BigLong operator *(ll x);
	BigLong operator /(ll x);
	BigLong move_copy(int m);
	BigLong move_round(int m, int t = 0);
	void divide(BigLong& b, BigLong& div, BigLong& mod);
	void normalize();
	bool normalize(int n);
	std::vector<ll>* get_digits() { return &digits_; };
	const std::string show();
private:
	bool sign_ = 1;
	std::vector<ll> digits_;
	const bool abs_less(BigLong& b);
};

