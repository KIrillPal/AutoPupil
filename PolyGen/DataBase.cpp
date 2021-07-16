#include "DataBase.h"

DataBase::DataBase()
{
}

DataBase::~DataBase()
{
	likeVars.clear();
	variables.clear();
	update_varButtons();
	for (auto b : varBacks) delete b;
	for (auto t : varValues) delete t;
}

double DataBase::pow(double x, int p, double max)
{
    double ans = 1;
    while (p)
    {
        if (p % 2) ans *= x, --p;
        else x *= x, p >>= 1;
    }
    return ans;
}

bool DataBase::isletter(char c)
{
	return 'a' <= c && c <= 'z';
}

void DataBase::partlist_add(char t, std::string var)
{
	inputPartList.push_back({ t, var });
}

bool DataBase::update_varBox(ClrBox * txb)
{
	std::map<std::string, std::string> tmp;
	std::set<std::string> tmp2;
	std::vector<std::pair<char, std::string> > tmp3;
	swap(variables, tmp);
	swap(likeVars, tmp2);
	swap(inputPartList, tmp3);
	fill_color(txb->text.toAnsiString(), txb->colorMap);
	swap(variables, tmp);
	swap(likeVars, tmp2);
	swap(inputPartList, tmp3);
	txb->redrawTexture();
	return tmp3.empty() && txb->text.getSize();
}

void DataBase::delete_varBox(int k)
{
	std::string var = varBacks[k]->mainText.getString();
	var = var.substr(0, var.size() - 3);
	variables.erase(var);
	varBacks.erase(varBacks.begin() + k);
	varValues.erase(varValues.begin() + k);
	for (int i = k; i < varBacks.size(); ++i)
	{
		auto pv = varValues[i]->resultBox.getPosition();
		auto pb = varBacks[i]->control;
		varValues[i]->setPosition(pv.x, pv.y - 40);
		varBacks[i]->setPosition(pb.left, pb.top - 40);
	}
}

std::pair<char, std::pair<int, int> > DataBase::find_operand(std::string& inp, std::string& color, int l, int r, std::vector<int>& to)
{
	std::pair<char, std::pair<int, int> > ans = { '-', {l, r} };
	int mode = 0, lst = l;
	std::string var = "";
	for (int i = l; i < r; ++i)
	{
		if (mode == 0 && isdigit(inp[i]))
			mode = 1, lst = i;
		else if (mode == 0 && isletter(inp[i]))
			mode = 3, var = inp[i], lst = i;
		else if (mode == 1 && isdigit(inp[i])) {}
		else if (mode == 1 && inp[i] == '.') mode = 2;
		else if (mode == 2 && isdigit(inp[i])) {}
		else if (mode == 1 || mode == 2)
		{
			ans = { 'p', {lst, i} };
			if (!isletter(inp[i])) return ans;
			mode = 3, var = inp[i];
		}
		else if (mode == 3 && isletter(inp[i])) var += inp[i];
		else if (mode == 3 && to[i])
		{
			if (functions.find(var) != functions.end())
			{
				partlist_add('f', var);
				bool ex = fill_color(inp, color, i + 1, to[i], to);
				if (ex)
				{
					color[i] = '(', color[to[i]] = ')';
					partlist_add(')', ")");
					return { 'r', {lst, i} };
				}
				inputPartList.pop_back();
			}
			else if (var == "log")
			{
				int bnd = 0, blnc = 0;
				for (int j = i + 1; j < to[i]; ++j)
					if (color[j] == '(') ++blnc;
					else if (color[j] == ')') --blnc;
					else if (blnc == 0 && inp[j] == ',') 
						bnd = j, j = to[i];
				bool ex = 0, ax = 0, bx = 0, kx = inputPartList.size();
				if (!bnd)
				{
					partlist_add('f', "ln");
					ex = fill_color(inp, color, i + 1, to[i], to);
				}
				else
				{
					partlist_add('(', "(");
					ax = fill_color(inp, color, i + 1, bnd, to);
					partlist_add(')', ")");
					partlist_add('(', "(");
					bx = fill_color(inp, color, bnd + 1, to[i], to);
					partlist_add(')', ")");
				}
				if (ex || ax && bx)
				{
					color[i] = '(', color[to[i]] = ')';
					if (ex) partlist_add(')', ")");
					else
					{
						color[bnd] = 's';
						partlist_add('l', "log");
					}
					return { 'r', {lst, i} };
				}
				else while (inputPartList.size() > kx)
					inputPartList.pop_back();
			}
			if (variables.find(var) != variables.end())
				return { 'r', {lst, i} };
			likeVars.insert(var);
			return ans;
		}
		else if (mode == 3)
		{
			if (variables.find(var) != variables.end())
				return { 'r', {lst, i} };
			likeVars.insert(var);
			return ans;
		}
		else if (mode == 0 && to[i])
		{
			partlist_add('(', "(");
			bool ex = fill_color(inp, color, i + 1, to[i], to);
			if (ex)
			{
				color[i] = '(';
				color[to[i]] = ')';
				partlist_add(')', ")");
				return { 'b', {i, to[i]} };
			}
			else inputPartList.pop_back();
		}
		else mode = 0, lst = i;
	}
	if (var != "" && variables.find(var) != variables.end())
		return { 'r', {lst, r} };
	else if (var != "")
	{
		likeVars.insert(var);
		return ans;
	}
	if (isdigit(inp[r - 1])) ans = { 'p', {lst, r} };
	else if (mode == 3) ans = { 'r', {lst, r} };
	return ans;
}

bool DataBase::fill_color(std::string & inp, std::string & color, int l, int r, std::vector<int> & to)
{
	int n = inp.size();
	int p = l;
	bool was = false;
	while (p < r)
	{
		size_t nxt = inp.find_first_of("-+*/%^", p + 1);
		if (nxt > r) nxt = r;
		if (was && color[p] != ')')
			partlist_add('s', std::string({ inp[p] }));
		auto var = find_operand(inp, color, p + was, nxt, to);
		if (was && color[p] != ')' && var.first != '-')
			color[p] = 's';
		if (var.first == 'p' || var.first == 'r')
		{
			int r1 = var.second.first;
			if (inp[p] == '-' && (p == 0 || color[p - 1] == '-'))
				color[p] = 'p';
			for (int i = r1; i < var.second.second; ++i)
				color[i] = isletter(inp[i]) ? 'v' : 'p';
			if (var.first == 'r')
			{
				std::string ps = (!was && inp[p] == '-') ? "-" : "", vs = "";
				while (!isletter(inp[r1]))
					ps += inp[r1++];
				while (r1 < var.second.second)
					vs += inp[r1++];
				if (ps == "-") ps += '1';
				if (functions.find(vs) == functions.end() && vs != "log")
				{
					if (ps != "")
					{
						partlist_add('p', ps);
						partlist_add('s', "*");
					}
					partlist_add('v', vs);
				}
				else
				{
					nxt = to[var.second.second];
					if (ps != "")
					{
						partlist_add('s', "*");
						partlist_add('p', ps);
					}
				}
			}
			else
			{
				std::string ps = (!was && inp[p] == '-') ? "-" : "";
				ps += inp.substr(r1, var.second.second - r1);
				partlist_add('p', ps);
			}
		}
		else if (var.first == 'b' && nxt < var.second.second)
			nxt = var.second.second;
		if (var.first != '-') was = true;
		if (!inputPartList.empty() && inputPartList.back().first == 's')
			inputPartList.pop_back();
		p = nxt;
	}
	return was;
}

void DataBase::fill_color(std::string & inp, std::string & color)
{
	inputPartList.clear();
	likeVars.clear();
	color = std::string(inp.size(), '-');
	std::stack<std::pair<int, char> > braces;
	braces.push({ -1, '0' });
	std::vector<int> to(inp.size());
	for (int i = 0; i < inp.size(); ++i)
		if (inp[i] == ')' && braces.top().second == '(')
		{ 
			color[i] = ')';
			color[braces.top().first] = '(';
			to[braces.top().first] = i;
			braces.pop();
		}
		else if (inp[i] == ']' && braces.top().second == '[')
		{
			color[i] = ')';
			color[braces.top().first] = '(';
			to[braces.top().first] = i;
			braces.pop();
		}
		else if (inp[i] == '(' || inp[i] == '[')
			braces.push({i, inp[i]});
	fill_color(inp, color, 0, inp.size(), to);
}

int DataBase::update_varButtons()
{
	for (auto b : varButtons)
		delete b;
	varButtons.clear();
	int x = 0;
	for (auto var : likeVars)
	{
		if (var.size() < 20 && functions.find(var) == functions.end() && var != "log")
		{
			PxButton * btn = new PxButton(sf::FloatRect(x, 0, 50, 25), sf::Color(226, 0, 0), 10.f, (char *)var.c_str(), xF);
			btn->setCharacterSize(18);
			int w = std::max(16.f, btn->textControl.width) + 9; 
			btn->control.width = w;
			btn->mainText.setFillColor(sf::Color::White);
			btn->setSprite(owt->createBackground(sf::FloatRect(0, 0, w, 25), "textures/white.png", 0));
			btn->backGround->setScale(0, 0);
			btn->setColor(sf::Color(226, 0, 0), 1, 0.9, 0.8);
			btn->offColor = sf::Color(206, 0, 0);
			varButtons.push_back(btn);
			x += w + 5;
		}
	}
	return x - 5;
}

int DataBase::create_Variable(int k, int W)
{
	std::string var = varButtons[k]->mainText.getString().toAnsiString();
	likeVars.erase(var);
	variables[var] = "0";
	delete varButtons[k];
	varButtons.erase(varButtons.begin() + k);
	update_varButtons();

	var += " = ";
	auto tmp = (char*)var.c_str();
	int y = varBacks.size() * 40;
	PxButton * back = new PxButton(sf::FloatRect(0, y, W, 35), sf::Color::White, 10, tmp, xF);
	back->setCharacterSize(18);
	back->setSprite(owt->createBackground(sf::FloatRect(0, 0, W, 35), "textures/white.png", 0));
	back->setColor(sf::Color::White, 1, 0.9, 0.8);
	back->backGround->setScale(0, 0);
	varBacks.push_back(back);

	int left = std::max(45.f, back->mainText.getGlobalBounds().width + 13);
	ClrBox *txb = new ClrBox("0", sf::FloatRect(left, y + 6, W - left + 5, 25), 18, bF);
	txb->isActive = true;
	txb->setMovingView(true);
	txb->colorMap = "p";
	varValues.push_back(txb);
	return y + 40;
}

int polRules[8][9] = {
	{ -1, 0, 0, 0, 0, -1, 0, -1, 0 }, //pv
	{ -1, 2, 2, 2, 1, -1, 1, -1, 1 }, //+-
	{ -1, 1, 2, 2, 1, -1, 1, -1, 1 }, //*/%
	{ -1, 1, 1, 1, 1, -1, 1, -1, 1 }, //^
	{ -1, 1, 1, 1, 1, -1, 1, -1, 1 }, // f
	{ -1, 0, 0, 0, 0, -1, 0, -1, 0 }, //log
	{ -1, 1, 1, 1, 1, -1, 1, -1, 1 }, //(
	{ -1, 2, 2, 2, 4, -1, 3, -1, -1 }, //)
};

std::map<char, int> polMt = {
	{'p', 0},
	{'v', 0},
	{'+', 1},
	{'-', 1},
	{'*', 2},
	{'/', 2},
	{'%', 2},
	{'^', 3},
	{'f', 4},
	{'l', 5},
	{'(', 6},
	{')', 7},
	{'e', 8}
};

void DataBase::update_polis(vparts &parts, vparts &polis)
{
 	std::stack<std::pair<char, std::string> > buf;
	polis.clear();
	for (int i = 0; i < parts.size();)
	{
		char t = parts[i].first;
		if (t == 's') t = parts[i].second[0];
		char t2 = buf.empty() ? 'e' : buf.top().first;
		if (t2 == 's') t2 = buf.top().second[0];
		int rt = polRules[polMt[t]][polMt[t2]];
		if (rt == 0)
		{
			polis.push_back(parts[i]);
			++i;
		}
		else if (rt == 1)
		{
			buf.push(parts[i]);
			++i;
		}
		else if (rt == 2 && !buf.empty())
		{
			polis.push_back(buf.top());
			buf.pop();
		}
		else if (rt == 3)
		{
			buf.pop();
			++i;
		}
		else if (rt == 4)
		{
			polis.push_back(buf.top());
			buf.pop();
			++i;
		}
	}
	while (!buf.empty())
	{
		polis.push_back(buf.top());
		buf.pop();
	}
}

std::string DataBase::compute_all()
{
	std::map<std::string, std::string> reals;
	reals["pi"] = BigFixed::PI.show();
	reals["e"] = BigFixed::E.show();
	for (auto vrp : variables)
	{
		if (vrp.first != "e" && vrp.first != "pi")
		{
			std::map<std::string, std::string> vt;
			std::set<std::string> lv;
			vparts parts, polis;
			swap(variables, vt);
			swap(likeVars, lv);
			swap(inputPartList, parts);
			std::string colorMap = "";
			fill_color(vrp.second, colorMap);
			swap(variables, vt);
			swap(likeVars, lv);
			swap(inputPartList, parts);
			update_polis(parts, polis);
			reals[vrp.first] = compute(polis).show();
		}
	}
	for (int i = 0; i < inputPolis.size(); ++i)
		if (inputPolis[i].first == 'v')
		{
			inputPolis[i].first = 'p';
			inputPolis[i].second = reals[inputPolis[i].second];
		}
	return compute(inputPolis).show();
}

BigFixed DataBase::compute(vparts& polis)
{
	std::stack<BigFixed> buf;
	for (int i = 0; i < polis.size(); ++i)
	{
		std::string t = polis[i].second;
		if (polis[i].first == 'p')
			buf.push(BigFixed(t));
		else if (polis[i].first != 'f')
		{
			BigFixed b = buf.top();
			buf.pop();
			BigFixed a = buf.top();
			buf.pop();
			if (t == "+") buf.push(a + b);
			else if (t == "-") buf.push(a - b);
			else if (t == "*") buf.push(a * b);
			else if (t == "/") buf.push(a / b);
			else if (t == "%") buf.push(a % b);
			else if (t == "^") buf.push(a ^ b);
			else if (t == "log") buf.push(a.log_(b));
		}
		else
		{
			BigFixed a = buf.top();
			buf.pop();
			if (t == "sin") buf.push(a.sin_());
			else if (t == "cos") buf.push(a.cos_());
			else if (t == "sqrt") buf.push(a.root(2));
			else if (t == "ln") buf.push(a.e->log_(a));
			else if (t == "lg") buf.push(a.log10(a));
		}
	}
	if (buf.empty()) buf.push(BigFixed(0));
	return buf.top();
}

void DataBase::update_variable(int k)
{
	std::string var = varBacks[k]->mainText.getString();
	var = var.substr(0, var.size() - 3);
	variables[var] = varValues[k]->text;
}
