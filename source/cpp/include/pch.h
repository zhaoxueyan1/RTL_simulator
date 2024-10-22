﻿// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

#ifndef PCH_H
#define PCH_H
#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<vector>
#include<map>
#include<string>
#include<stack>
#include<unordered_map>
#include<queue>
#include<set>
#include<fstream>
#include<string.h>
#include<ostream>
#include<algorithm>
#include<assert.h>
#include<ctime>
#include<cmath>
#define UINT_8 unsigned char
#define UINT_16 unsigned short
#define UINT_32 unsigned int
#define UINT_64 unsigned long long 
// TODO: 添加要在此处预编译的标头
#define SINT_8  char
#define SINT_16 short
#define SINT_32 int
#define SINT_64 long long 
#define CALC(w) ((w) <= 8 ? 1 : (w) <= 16 ? 2 : (w) <= 32 ? 4 : (w) <= 64 ? 8 : 8)
#define DEBUG 0
enum {
	BOOL = 1,
	INT_16 = 2,
	INT_32 = 4,
	INT_64 = 8,
};

#endif //PCH_H
