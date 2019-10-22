#include "des.h"

namespace info_secu
{
const size_t Des::_ipTable[64] =
	{
		58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
		62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
		57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
		61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7};
const size_t Des::_ipInTable[64] =
	{
		40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
		38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
		36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
		34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25};

const size_t Des::_keyTable[56] =
	{
		57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18,
		10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36,
		63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22,
		14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4};

const size_t Des::_shiftTable[16] =
	{
		1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

const size_t Des::_extendTable[48] =
	{
		32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9,
		8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
		16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
		24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1};

const size_t Des::_keyCompressTable[48] =
	{
		14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10,
		23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2,
		41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
		44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32};

const size_t Des::_permuteBox[32] =
	{
		16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
		2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25};

Des::Des(BITS64 bs)
{
	keyPermute(bs);
	initShiftKeys();
	//init SBox
	_SBox.push_back({14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
					 0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
					 4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
					 15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13});
	_SBox.push_back({15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
					 3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
					 0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
					 13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9});
	_SBox.push_back({10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
					 13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
					 13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
					 1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12});
	_SBox.push_back({7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
					 13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
					 10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
					 3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14});
	_SBox.push_back({2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
					 14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
					 4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
					 11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3});
	_SBox.push_back({12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
					 10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
					 9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
					 4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13});
	_SBox.push_back({4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
					 13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
					 1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
					 6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12});
	_SBox.push_back({13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
					 1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
					 7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
					 2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11});
}

void Des::encrypt(std::vector<DTYPE> &plain)
{
	int nleft = plain.size();
	int ndone = 0;
	while (nleft >= 4)
	{
		auto datapointer = CAST64P(plain.data() + ndone);
		TYPE64 unit = *datapointer;
		BITS64 l_r(unit);

		initPermute(l_r); //initial permutation

		unit = static_cast<TYPE64>(l_r.to_ullong());
		BITS32 left(static_cast<TYPE32>(unit >> 32));
		BITS32 right(static_cast<TYPE32>((unit << 32) >> 32));

		for (int i = 0; i < 16; ++i) //16 rounds. i is key index.
		{
			renewed(left, right, i);
			if (i != 15)
			{
				std::swap(left, right);
			}
		}
		TYPE64 result = (static_cast<TYPE64>(left.to_ullong()) << 32) + static_cast<TYPE64>(right.to_ullong());
		l_r = BITS64(result);
		invePermute(l_r); //inverse permutation
		result = static_cast<TYPE64>(l_r.to_ullong());
		*datapointer = result;

		nleft -= SIZE64;
		ndone += SIZE64;
	}
}

void Des::decrypt(std::vector<DTYPE> &cipher)
{
	int nleft = cipher.size();
	int ndone = 0;
	while (nleft >= 4)
	{
		auto datapointer = CAST64P(cipher.data() + ndone);
		TYPE64 unit = *datapointer;
		BITS64 l_r(unit);

		initPermute(l_r); //initial permutation

		unit = static_cast<TYPE64>(l_r.to_ullong());
		BITS32 left(static_cast<TYPE32>(unit >> 32));
		BITS32 right(static_cast<TYPE32>((unit << 32) >> 32));

		for (int i = 15; i >= 0; --i) //16 rounds. i is key index.
		{
			renewed(left, right, i);
			if (i != 0)
			{
				std::swap(left, right);
			}
		}
		auto ttt = static_cast<TYPE64>(left.to_ullong()) << 32;
		TYPE64 result = (static_cast<TYPE64>(left.to_ullong()) << 32) + static_cast<TYPE64>(right.to_ullong());
		l_r = BITS64(result);
		invePermute(l_r); //inverse permutation
		result = static_cast<TYPE64>(l_r.to_ullong());
		*datapointer = result;

		nleft -= SIZE64;
		ndone += SIZE64;
	}
}

void Des::keyPermute(BITS64 &key)
{
	PERMUTE(_key, key, _keyTable, 56, -1);
}

void Des::initShiftKeys() //keypermute and key compress permute
{
	//divide key into left and right
	std::string str = _key.to_string();
	std::reverse(str.begin(), str.end());
	std::string leftstr(str.begin(), str.begin() + 28);
	std::string rightstr(str.begin() + 28, str.end());
	for (int i = 0; i < 16; ++i)
	{
		if (_shiftTable[i] == 1)
		{
			CYCLESHIFT(leftstr);
			CYCLESHIFT(rightstr);
		}
		else
		{
			CYCLESHIFT(leftstr);
			CYCLESHIFT(leftstr);
			CYCLESHIFT(rightstr);
			CYCLESHIFT(rightstr);
		}
		std::string lrstr = leftstr + rightstr;
		std::reverse(lrstr.begin(), lrstr.end());
		BITS56 bits(lrstr);

		//compress permute
		std::bitset<48> cmbits;
		PERMUTE(cmbits, bits, _keyCompressTable, 48, -1);
		_shiftKeys.push_back(cmbits);
	}
}

} // namespace info_secu