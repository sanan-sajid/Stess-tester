#include <iostream>
#include <random>
#include <vector>
#include <fstream>

using namespace std;

int getRandomNumber(int min, int max)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(min, max);
	return dis(gen);
}
int getrandom(int min, int max, int k)
{
	int x = getRandomNumber(min, max);
	return x - x % k;
}

// n*log(n) + q*log(n)
void generateTestCase()
{
	int t = getRandomNumber(1000, 1000);
	cout << t << endl;

	while (t--)
	{
		int n = getRandomNumber(100000, 100000);
		int k = getRandomNumber(2, 30);
		int q = getRandomNumber(10000, 10000);

		cout << n << " " << k << " " << q << '\n';

		vector<int> A(n);
		for (int i = 0; i < n; i++)
		{
			A[i] = getRandomNumber(1, 10000);

			cout << A[i] << " ";
		}
		cout << '\n';

		for (int i = 0; i < q; i++)
		{
			int type = getRandomNumber(1, 2);
			// if (i >= q - 100)
			// {
			// 	type = 1;
			// }
			// else
			// {
			// 	type = 2;
			// }
			if (type == 1)
			{
				int l1 = getRandomNumber(1, n);
				int r1 = getRandomNumber(1, n);
				int l2 = getRandomNumber(1, n);
				int r2 = getRandomNumber(1, n);

				if (l1 > r1)
					swap(l1, r1);
				if (l2 > r2)
					swap(l2, r2);
				cout << type << " " << l1 << " " << r1 << " " << l2 << " " << r2 << '\n';
			}
			else
			{
				int li = getRandomNumber(1, n);
				int ri = getRandomNumber(1, n);
				if (li > ri)
					swap(li, ri);
				// li = ri;
				int x = getRandomNumber(1, 1000000);
				cout << type << " " << li << " " << ri << " " << x << '\n';
			}
		}
	}
}

int main()
{
	generateTestCase();
	return 0;
}
