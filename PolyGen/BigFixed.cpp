#include "BigFixed.h"


int BigFixed::PRECISION = 0;
int BigFixed::PREC_SIZE = 0;
BigFixed BigFixed::PI;
BigFixed BigFixed::E;

void BigFixed::init()
{
	precision = BigFixed::PRECISION;
	prec_size = BigFixed::PREC_SIZE;
	prec_full = prec_size * BASE_C;
	pi = &PI;
	e = &E;
}

BigFixed BigFixed::root(int pwn, int prec)
{
	BigFixed a = *this;
	if (a.core.isnan) return a;
	if (a < BigFixed(0))
	{
		a.core.isnan = 5;
		return a;
	}
	BigLong pow(pwn);
	BigFixed sol(1);
	if (a < sol)
		return sol / (sol / a).root(pwn, prec);
	if (prec == -1) prec = prec_size;
	BigFixed pw(1);
	int k = 0;
	while (!(a < (pw ^ pow)))
	{
		(*pw.core.get_digits())[prec_size + k++] = 0;
		pw.core.get_digits()->push_back(1);
	}
	BigFixed base;
	base.normalize(k + prec_size);
	std::vector<ll> & digs = *base.core.get_digits();
	k += prec_size;
	for (int i = k - 1; i >= prec_size - prec; --i)
	{
		ll l = 0, r = BASE;
		while (r - l > 1)
		{
			digs[i] = (l + r) / 2;
			if (a < (base ^ pow))
				r = digs[i];
			else l = digs[i];
		}
		digs[i] = l;
	}
	return base;
}

void BigFixed::normalize(int n)
{
	if (n == -1) n = prec_size;
	core.normalize(n);
}

void BigFixed::set_sign(bool sign)
{
	core.set_sign(sign);
}

bool BigFixed::get_sign()
{
	return core.get_sign();
}

bool BigFixed::operator>(double x)
{
	if ((x >= 0) != core.get_sign())
		return (core.get_sign());

	ll lx = x * BASE;
	ll p1 = 0;
	if (core.get_length() > prec_size + 1)
		return core.get_sign();
	if (core.get_length() > prec_size)
		p1 = (*core.get_digits())[prec_size];
	ll p2 = (*core.get_digits())[prec_size - 1];
	if (p1 != lx / BASE)
		return core.get_sign() == (p1 > lx / BASE);
	if (p2 != lx % BASE)
		return core.get_sign() == (p2 > lx % BASE);
}

bool BigFixed::operator<(BigFixed & r)
{
	return core < r.core;
}

bool BigFixed::operator==(BigFixed & r)
{
	return core == r.core;
}

BigFixed::BigFixed()
{
	init();
	core = BigLong(0, prec_size);
}

BigFixed::BigFixed(BigLong & b)
{
	init();
	core = b.move_copy(prec_size);
	core.set_sign(b.get_sign());
}

BigFixed::BigFixed(std::string decimal)
{
	init();
	if (decimal.empty())
	{
		*this = BigLong();
		return;
	};
	if (decimal == "NAN")
	{
		*this = BigLong();
		this->core.isnan = 3;
		return;
	}
	bool sign = true;
	if (decimal[0] == '-')
		sign = 0, decimal.erase(decimal.begin());
	ll p = std::min(decimal.size(), decimal.find_first_of('.'));
	std::string a = decimal.substr(0, p), b = "";
	if (p < decimal.size())
		b = decimal.substr(p + 1);
	bool more = false;
	for (int i = prec_full; i < b.size(); ++i)
		if (b[i] != '4')
		{
			more = (b[i] > '4');
			break;
		}
	b = b.substr(0, prec_full);
	if (prec_full >= b.size())
		b += std::string(prec_full - b.size(), '0');
	core = BigLong(a).move_copy(prec_size) + BigLong(b);
	core.set_sign(sign);
	if (more) core = core + BigLong(1);
	normalize();
}

BigFixed::BigFixed(double inp)
{
	init();
	core.normalize(prec_size);
	if (isinf(inp))
	{
		core.isnan = 1;
		return;
	}
	if (inp < 0) inp *= -1, set_sign(0);
	ll linp = inp * BASE;
	auto digs = core.get_digits();
	(*digs)[prec_size - 1] = linp % BASE;
	digs->push_back(linp / BASE);
	normalize();
}

BigFixed::~BigFixed()
{
}

void BigFixed::set_precision(int nv)
{
	BigFixed::PRECISION = nv;
	BigFixed::PREC_SIZE = (nv + BASE_C - 1) / BASE_C;
}

int BigFixed::get_precision()
{
	return BigFixed::PRECISION;
}

BigFixed BigFixed::operator+(BigFixed & r)
{
	BigFixed c = *this;
	c.core = c.core + r.core;
	c.normalize();
	return c;
}

BigFixed BigFixed::operator-(BigFixed & r)
{
	BigFixed c = *this;
	c.core = c.core + (-r.core);
	c.normalize();
	return c;
}

BigFixed BigFixed::operator*(BigFixed & r)
{
	BigFixed c;
	c.core = (core * r.core).move_round(-prec_size);
	c.normalize();
	return c;
}

BigFixed BigFixed::operator/(BigFixed & r)
{
	BigLong main = core.move_copy(prec_size);
	BigLong div, mod;
	main.divide(r.core, div, mod);
	BigFixed c;
	c.set_sign(get_sign() == r.get_sign());
	c.core = div;
	if (r.core < mod * 2 || r.core == mod * 2)
		c.core = c.core + BigLong(1);
	c.normalize();
	return c;
}

BigFixed BigFixed::operator%(BigFixed & r)
{
	BigFixed div = (*this / r);
	div.core = div.core.move_round(-prec_size, 1 - 2 * this->get_sign());
	div.core = div.core.move_copy(prec_size);
	return *this - div * r;
}

BigFixed BigFixed::operator^(BigLong pow)
{
	BigLong zer();
	pow.normalize();
	BigFixed x(*this), res(1);
	if (pow.isnan || core.isnan)
	{
		res.core.isnan = std::max(pow.isnan, core.isnan);
		return res;
	}
	if (x == BigFixed(0) && (pow < BigLong(0) || pow == BigLong(0)))
	{
		res.core.isnan = 3;
		return res;
	}
	while (pow.get_length())
	{
		if ((*pow.get_digits())[0] % 2 == 1)
			res = res * x;
		x = x * x;
		if (x.core.isnan || pow.isnan)
		{
			res.core.isnan = std::max(x.core.isnan, pow.isnan);
			return res;
		}
		pow = pow / 2;
	}
	return res;
}

BigFixed BigFixed::operator^(BigFixed & pow)
{
	if (this->core.isnan || pow.core.isnan)
		return *this;
	if (!pow.get_sign())
	{
		BigFixed mpw = pow;
		mpw.set_sign(1);
		return BigFixed(1) / ((*this) ^ mpw);
	}
	BigLong c = pow.core.move_copy(-prec_size);
	BigFixed res;
	res = (*this) ^ c;
	BigFixed tres(1);
	for (int i = std::max(0, prec_size - 2); i < prec_size; ++i)
	{
		ll pwm = 1;
		if ((*pow.core.get_digits())[i] && !this->get_sign())
		{
			res.core.isnan = 4;
			return res;
		}
		for (int j = 0; j < BASE_C; ++j)
		{
			int d = ((*pow.core.get_digits())[i] / pwm) % 10;
			pwm *= 10;
			BigFixed t = (*this) ^ d;
			tres = (tres * t).root(10, std::min(prec_size, 2));
		}
	}
	return res * tres;
}

BigFixed BigFixed::sin_()
{
	BigFixed c = *this % ((*pi) * BigFixed(2));
	return BigFixed(sin(c.to_double()));
}

BigFixed BigFixed::cos_()
{
	BigFixed c = *this % ((*pi) * BigFixed(2));
	return BigFixed(cos(c.to_double()));
}

BigFixed BigFixed::tan_()
{
	BigFixed c = *this % ((*pi) * BigFixed(2));
	return BigFixed(tan(c.to_double()));
}

BigFixed BigFixed::cot_()
{
	BigFixed c = *this % ((*pi) * BigFixed(2));
	return BigFixed(1.0 / tan(c.to_double()));
}

BigFixed BigFixed::log_(BigFixed & b)
{
	BigFixed a = *this;
	if (a.core.isnan || b.core.isnan)
	{
		a.core.isnan = 3;
		return a;
	}
	if (a < BigFixed(0) || b < BigFixed(0) || a == BigFixed(0) || b == BigFixed(0))
	{
		a.core.isnan = 3;
		return a;
	}
	if (a == BigFixed(1))
	{
		if (b == BigFixed(1))
			return BigFixed();
		a.core.isnan = 3;
		return a;
	}
	BigFixed sol(1);
	bool az = 0, bz = 0;
	if (a < sol) az = 1, a = sol / a;
	if (b < sol) bz = 1, b = sol / b;
	BigFixed lg = a.log10(b) / a.log10(a);
	if (az != bz)
		lg.set_sign(!lg.get_sign());
	return lg;
}

BigFixed BigFixed::log10(BigFixed b, int prec)
{
	if (prec == -1) prec = prec_size;
	int k = prec * 9;
	BigFixed pow;
	BigLong ten(10);
	std::vector<ll> * digs = pow.core.get_digits();
	if (digs->size() == prec_size)
		digs->push_back(0);
	b.normalize();
	ll mbs = 10;
	for (int i = -1; i < k; ++i)
	{
		if (i % 9 == 0) mbs = BASE;
		int pc = (b.core.get_length() - prec_size) * 9;
		if (pc > 0)
		{
			pc -= 9;
			ll pd = 10, lstd = b.core.get_digits()->back();
			while (pd <= lstd)
				pd *= 10, ++pc;
		}
		mbs /= 10;
		(*digs)[prec_size - (i + 9) / 9] += pc * mbs;
		if ((*digs)[prec_size - i / 9] > BASE)
		{
			ll d = (*digs)[prec_size - i / 9];
			(*digs)[prec_size - i / 9 + 1] += d / BASE;
			(*digs)[prec_size - i / 9] = d % BASE;
		}
		b.core = b.core.move_round(-(pc / 9));
		b.normalize();
		ll pwd = 1, pwi = 0;
		while (pwi < pc % 9)
			pwd *= 10, pwi++;
		b = b / BigFixed(pwd);
		b = b ^ ten;
	}
	return pow;
}

double BigFixed::to_double()
{
	auto digs = this->core.get_digits();
	double x = (*digs)[prec_size - 1] * 1.0 / BASE;
	if (digs->size() > prec_size)
		x += (*digs)[prec_size];
	if (!this->get_sign())
		x *= -1;
	return x;
}


std::string BigFixed::show()
{
	if (core.isnan == 1 && core.get_sign()) return "INF";
	else if (core.isnan == 1) return "-INF";
	else if (core.isnan) return "NAN";
	ll lst = (*core.get_digits())[0];
	ll lft = (BASE_C - precision % BASE_C) % BASE_C;
	(*core.get_digits())[0] -= lst;
	ll pw = 1;
	for (int i = 0; i < lft; ++i)
		pw *= 10;
	double nv = lst / (double)pw;
	(*core.get_digits())[0] = round(nv) * pw;
	bool sign = core.get_sign();
	core.set_sign(1);
	std::string res = core.show();
	if (res.size() < prec_full)
		res = std::string(prec_full - res.size(), '0') + res;
	(*core.get_digits())[0] = lst;
	if (!sign) res = "-" + res;
	int p = res.size() - prec_full;
	res.insert(p, p == (res[0] == '-') ? "0." : ".");
	while (res.size() && res.back() == '0')
		res.pop_back();
	if (res.size() && res.back() == '.')
		res.pop_back();
	return res;
}
/*
*/