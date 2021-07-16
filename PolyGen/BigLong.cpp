#include "BigLong.h"



BigLong::BigLong()
{

}

BigLong::BigLong(std::string decimal)
{
	if (decimal[0] == '-' || decimal[0] == '+')
	{
		sign_ = (decimal[0] == '+');
		decimal.erase(decimal.begin());
	}
	int m = decimal.size();
	int n = (m + 15) / BASE_C;
	digits_.resize(n);
	for (int i = 0; i < n; ++i)
	{
		for (int j = std::max(0, m - (i + 1) * BASE_C); j < m - i * BASE_C; ++j)
			digits_[i] = digits_[i] * 10 + decimal[j] - '0';
	}
	normalize();
}

BigLong::BigLong(ll x, int n)
{
	if (n <= 0) return;
	if (x < 0)
		sign_ = 0, x = -x;
	digits_.resize(n);
	digits_[0] = x % BASE;
	if (x >= BASE && n > 1) digits_[1] = x / BASE;
	else if (x >= BASE && n <= 1) digits_.push_back(x / BASE);
}


BigLong::~BigLong()
{
}

void BigLong::set_sign(bool sign)
{
	sign_ = sign;
}

const bool BigLong::operator<(BigLong & b)
{
	if (b.isnan == 1 && isnan == 1)
		return b.sign_ > sign_;
	else if (b.isnan) return b.sign_;
	else if (isnan) return sign_;
	if (sign_ != b.sign_)
		return (sign_ < b.sign_);
	return abs_less(b);
}

const bool BigLong::operator==(BigLong & b)
{
	if (sign_ != b.sign_)
		return false;
	if (get_length() == 0) return true;
	int an = get_length(), bn = b.get_length();
	int k = std::max(b.get_length(), get_length()) - 1;
	while (k >= 0)
	{
		if (k >= bn && digits_[k])
			return false;
		else if (k >= an && b.digits_[k])
			return false;
		else if (k < an && k < bn && digits_[k] != b.digits_[k])
			return false;
		--k;
	}
	return true;
}

BigLong BigLong::operator-() const
{
	BigLong a = *this;
	a.sign_ = !a.sign_;
	a.normalize();
	return a;
}

BigLong BigLong::operator+(BigLong & b)
{
	if (isnan || b.isnan)
	{
		isnan = std::max(isnan, b.isnan);
		if (isnan == 1 && b.isnan == 1 && sign_ != b.sign_)
			return BigLong();
		return *this;
	}
	if (sign_ != b.sign_)
	{
		if (sign_ && this->abs_less(b))
			return -(-b + (-(*this)));
		else if (!sign_ && this->abs_less(b))
			return b + *this;
		else if (!sign_)
			return -(-(*this) + (-b));
	}
	int n = std::max(get_length(), b.get_length());
	BigLong c(0, n);
	int odd = 0;
	for (int i = 0; i < n; ++i)
	{
		if (i < digits_.size())
			c.digits_[i] += digits_[i];
		if (i < b.digits_.size() && sign_ == b.sign_)
			c.digits_[i] += b.digits_[i];
		else if (i < b.digits_.size())
			c.digits_[i] -= b.digits_[i];
		c.digits_[i] += odd;
		odd = c.digits_[i] / BASE;
		if (c.digits_[i] < 0)
		{
			c.digits_[i] += BASE;
			odd = -1;
		}
		c.digits_[i] %= BASE;
	}
	if (odd) c.digits_.push_back(odd);
	c.sign_ = sign_;
	if (c.digits_.size() > inf_size)
		c.digits_.clear(), c.isnan = 1;
	c.normalize();
	return c;
}

BigLong BigLong::operator-(BigLong & b)
{
	return *this + (-b);
}

int ccc = 0;

BigLong BigLong::operator*(BigLong & r) // Konosuba algorithm
{
	if (isnan || r.isnan)
	{
		BigLong c(0);
		c.sign_ = (sign_ == r.sign_);
		c.isnan = std::max(isnan, r.isnan);
		return c;
	}
	int n = std::max(get_length(), r.get_length());
	if (n <= 1)
	{
		if (!get_length() || !r.get_length())
			return 0;
		BigLong res = digits_[0] * r.digits_[0];
		res.sign_ = (sign_ == r.sign_);
		return res;
	}
	int part = n / 2;
	BigLong a(0, get_length() - part), c(0, r.get_length() - part);
	BigLong b(0, part), d(0, part);
	for (int i = 0; i < get_length(); ++i)
		if (i < part) b.digits_[i] = digits_[i];
		else a.digits_[i - part] = digits_[i];
	for (int i = 0; i < r.get_length(); ++i)
		if (i < part) d.digits_[i] = r.digits_[i];
		else c.digits_[i - part] = r.digits_[i];
	b.normalize();
	d.normalize();
	BigLong bd = b * d;
	BigLong ac = a * c;
	BigLong s1 = a + b;
	BigLong s2 = c + d;
	BigLong abcd = s1 * s2 - ac - bd;
	bd.normalize(part);
	abcd.normalize(part);
	ac.normalize();
	BigLong res = bd + abcd.move_copy(part);
	res = res + ac.move_copy(2 * part);
	res.sign_ = (sign_ == r.sign_);
	if (res.digits_.size() > inf_size)
		res.digits_.clear(), res.isnan = 1;
	res.normalize();
	return res;
}

BigLong BigLong::operator/(BigLong & b)
{
	BigLong div, mod;
	divide(b, div, mod);
	return div;
}

BigLong BigLong::operator*(ll x)
{
	int n = get_length();
	BigLong c(0, n + 1);
	c.isnan = isnan;
	c.sign_ = sign_;
	if (x < 0) c.sign_ *= -1, x = -x;
	for (int i = 0; i < n; ++i)
	{
		c.digits_[i] += digits_[i] * x;
		c.digits_[i + 1] += c.digits_[i] / BASE;
		c.digits_[i] %= BASE;
	}
	if (c.digits_.size() > inf_size)
		c.digits_.clear(), c.isnan = 1;
	c.normalize();
	return c;
}

BigLong BigLong::operator/(ll x)
{
	return (*this) / BigLong(x);
}

void BigLong::divide(BigLong & r, BigLong & div, BigLong & mod)
{
	if (isnan || r.isnan)
	{
		BigLong dvd(0);
		dvd.isnan = std::max(isnan, r.isnan);
		div = dvd;
		mod = BigLong();
		return;
	}
	if (r == BigLong())
	{
		div.isnan = 1 + (isnan || (*this == BigLong()));
		div.sign_ = (sign_ == r.sign_);
		mod = BigLong();
		return;
	}
	int n = get_length();
	div = BigLong(0, n);
	div.sign_ = (sign_ == r.sign_);
	bool tsign = r.sign_;
	r.sign_ = 1;
	mod = BigLong();
	for (int i = n - 1; i >= 0; --i)
	{
		ll bc = BASE;
		for (int j = 0; j < BASE_C; ++j)
		{
			bc /= 10;
			mod = mod * 10 + BigLong((digits_[i] / bc) % 10);
			int c = 0;
			while (r < mod || r == mod)
				mod = mod - r, ++c;
			div.digits_[i] = 10 * div.digits_[i] + c;
		}
	}
	if (div.digits_.size() > inf_size)
		div.digits_.clear(), div.isnan = 1;
	r.sign_ = tsign;
	div.normalize();
	mod.normalize();
}

BigLong BigLong::move_copy(int m)
{
	BigLong c(0, std::max(0ULL, digits_.size() + m));
	c.isnan = isnan;
	for (int i = 0; i < digits_.size(); ++i)
		if (i + m >= 0)
			c.digits_[i + m] = digits_[i];
	c.sign_ = sign_;
	if (c.digits_.size() > inf_size)
		c.digits_.clear(), c.isnan = 1;
	return c;
}

BigLong BigLong::move_round(int m, int t)
{
	if (t == -1) return move_copy(m);
	BigLong a = *this;
	const ll check = t == 0 ? 444444444 : 0;
	bool more = false;
	auto digs = a.get_digits();
	for (int i = std::min((int)digs->size(), -m) - 1; i >= 0; --i)
		if ((*digs)[i] != check)
		{
			more = ((*digs)[i] > check);
			break;
		}
	a = a.move_copy(m);
	if (more) a = a + BigLong(-1 + 2 * a.sign_);
	return a;
}

const std::string BigLong::show()
{
	std::string res = sign_ ? "" : "-";
	if (digits_.size() == 0) return "0";
	for (int i = digits_.size() - 1; i >= 0; --i)
	{
		std::string part = std::to_string(digits_[i]);
		if (i != digits_.size() - 1 && part.size() < BASE_C)
			res += std::string(BASE_C - part.size(), '0');
		res += part;
	}
	return res;
}

void BigLong::normalize()
{
	while (digits_.size() && digits_.back() == 0)
		digits_.pop_back();
	if (digits_.empty() && !sign_) sign_ = 1;
}

bool BigLong::normalize(int n)
{
	while (digits_.size() && digits_.back() == 0 && digits_.size() > n)
		digits_.pop_back();
	while (digits_.size() < n)
		digits_.push_back(0);
	return digits_.size() == n;
}

const bool BigLong::abs_less(BigLong & b)
{
	if (get_length() != b.get_length())
		return (get_length() < b.get_length()) == sign_;
	if (get_length() == 0) return 0;
	int k = get_length() - 1;
	while (k && digits_[k] == b.digits_[k])
		--k;
	return digits_[k] < b.digits_[k];
}