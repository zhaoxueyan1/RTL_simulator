#pragma once
#include"pch.h"

using namespace std;

enum {
	BOOL = 1,
	INT_16 = 2,
	INT_32 = 4,
	INT_64 = 8,
};

map<string, void *>updateSet; // to storge the val of this cycle
