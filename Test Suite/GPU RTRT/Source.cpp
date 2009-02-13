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

	getchar();

    return 0;
}