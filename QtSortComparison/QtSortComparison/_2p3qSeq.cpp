#include "_2p3qSeq.h"

_2p3qSeq::_2p3qSeq() 
{
	_2p3q.push_back(1);
	i2 = 0;
	i3 = 0;
}
int _2p3qSeq::next()
{
	int cur = _2p3q.back();
	int n2 = 2 * _2p3q[i2];
	int n3 = 3 * _2p3q[i3];
	_2p3q.push_back(std::min(n2, n3));
	i2 += n2 <= n3;
	i3 += n2 >= n3;
	return cur;
}
void _2p3qSeq::generate(int max)
{
	for (int i = next(); i < max; i = next());
	_2p3q.pop_back();
}

int _2p3qSeq::popBack()
{
	if (!_2p3q.empty())
	{
		int r = _2p3q.back();
		_2p3q.pop_back();
		return r;
	}
	else
	{
		return 0;
	}
}