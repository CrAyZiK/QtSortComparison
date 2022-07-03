#pragma once

#include <vector>

class _2p3qSeq
{
public:
	_2p3qSeq();
	int next();
	void generate(int max);
	int popBack();
private:
	std::vector<int> _2p3q;
	int i2;
	int i3;
};