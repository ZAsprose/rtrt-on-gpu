#include <iostream>

#include "Util.h"

#include "Vector2D.h"

using namespace std;

int main()
{
    double a = 7.4;
    int b = 99;

	b = Util::Clamp(b, 0, 1);

	cout << b;

	int aa = 1, bb = 2, cc = 0;

	cc = aa += bb;

	getchar();

    return 0;
}