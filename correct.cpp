#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds;
#define int long long
#define all(v) v.begin(), v.end()
#define vi vector<int>
typedef long long ll;
const int n1 = 1e9 + 7;
const int MX = 1e9;
typedef tree<int, null_type, less_equal<int>, rb_tree_tag, tree_order_statistics_node_update> pbdsMS;
typedef tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> pbds;

void modadd(int &a, int b, int MOD) { a = ((a % MOD) + (b % MOD)) % MOD; }
void modsub(int &a, int b, int MOD) { a = ((a % MOD) - (b % MOD) + MOD) % MOD; }
void modmul(int &a, int b, int MOD) { a = ((a % MOD) * (b % MOD)) % MOD; }
// ================================== take ip/op like vector,pairs directly!==================================
template <typename typC, typename typD>
istream &operator>>(istream &cin, pair<typC, typD> &a) { return cin >> a.first >> a.second; }
template <typename typC>
istream &operator>>(istream &cin, vector<typC> &a)
{
	for (auto &x : a)
		cin >> x;
	return cin;
}
template <typename typC, typename typD>
ostream &operator<<(ostream &cout, const pair<typC, typD> &a) { return cout << a.first << ' ' << a.second; }
template <typename typC, typename typD>
ostream &operator<<(ostream &cout, const vector<pair<typC, typD>> &a)
{
	for (auto &x : a)
		cout << x << '\n';
	return cout;
}
template <typename typC>
ostream &operator<<(ostream &cout, const vector<typC> &a)
{
	int n = a.size();
	if (!n)
		return cout;
	cout << a[0];
	for (int i = 1; i < n; i++)
		cout << ' ' << a[i];
	return cout;
}
// ===================================END Of the input module ==========================================
// === Segment Tree ===
struct Node;
struct Update;

template <typename T, typename Node = Node, typename Update = Update>
class SegmentTree
{
private:
	int size = 0;
	vector<Node> seg;

	void build(int start, int end, int ind, vector<T> &arr)
	{
		if (start == end)
		{
			seg[ind] = Node(arr[start]);
			return;
		}
		int mid = (start + end) / 2;
		int leftInd = 2 * ind + 1, rightInd = 2 * ind + 2;
		build(start, mid, leftInd, arr);
		build(mid + 1, end, rightInd, arr);
		seg[ind].merge(seg[leftInd], seg[rightInd]);
	}

	Node query(int start, int end, int ind, int left, int right)
	{
		if (start > right || end < left)
		{
			return Node();
		}
		if (start >= left && end <= right)
		{
			return seg[ind];
		}
		int mid = (start + end) / 2;
		int leftInd = 2 * ind + 1, rightInd = 2 * ind + 2;
		Node res;
		Node leftItem = query(start, mid, leftInd, left, right);
		Node rightItem = query(mid + 1, end, rightInd, left, right);
		res.merge(leftItem, rightItem);
		return res;
	}

	void update(int start, int end, int ind, int index, Update &u)
	{
		if (start == end)
		{
			u.apply(seg[ind]);
			return;
		}
		int mid = (start + end) / 2;
		int leftInd = 2 * ind + 1, rightInd = 2 * ind + 2;
		if (index <= mid)
			update(start, mid, leftInd, index, u);
		else
			update(mid + 1, end, rightInd, index, u);
		seg[ind].merge(seg[leftInd], seg[rightInd]);
	}

public:
	SegmentTree()
	{
	}

	SegmentTree(int n)
	{
		size = n;
		seg.resize(4 * size + 1);
	}

	SegmentTree(vector<T> &arr)
	{
		size = arr.size();
		seg.resize(4 * size + 1);
		build(arr);
	}

	void build(vector<T> &arr)
	{
		build(0, size - 1, 0, arr);
	}

	Node query(int left, int right)
	{
		return query(0, size - 1, 0, left, right);
	}

	void update(int index, int value)
	{
		Update u = Update(value);
		return update(0, size - 1, 0, index, u);
	}
};

struct Node
{
	long long sum;
	int mx, mn;
	long long andd;

	Node() : sum(0), mx(-MX), mn(MX), andd((1LL << 32) - 1) {}
	Node(int val) : sum(val), mx(val), mn(val), andd(val) {}

	void merge(Node &left, Node &right)
	{
		sum = left.sum + right.sum;
		mx = max(left.mx, right.mx);
		mn = min(left.mn, right.mn);
		andd = left.andd & right.andd;
	}
};

struct Update
{
	int val;

	Update() : val(0) {}
	Update(int v) : val(v) {}

	void apply(Node &node)
	{
		node.sum = val;
		node.mn = val;
		node.mx = val;
		node.andd = val;
	}
};
typedef unsigned long long ull;
typedef long double lld;

#ifndef ONLINE_JUDGE
#define debug(x...)             \
	cerr << "[" << #x << "] = ["; \
	_print(x)
#else
#define debug(x)
#endif
void __print(long x) { cerr << x; }
void __print(long long x) { cerr << x; }
void __print(unsigned x) { cerr << x; }
void __print(unsigned long x) { cerr << x; }
void __print(unsigned long long x) { cerr << x; }
void __print(float x) { cerr << x; }
void __print(double x) { cerr << x; }
void __print(long double x) { cerr << x; }
void __print(char x) { cerr << '\'' << x << '\''; }
void __print(const char *x) { cerr << '\"' << x << '\"'; }
void __print(const string &x) { cerr << '\"' << x << '\"'; }
void __print(bool x) { cerr << (x ? "true" : "false"); }

template <typename T, typename V>
void __print(const pair<T, V> &x)
{
	cerr << '{';
	__print(x.first);
	cerr << ',';
	__print(x.second);
	cerr << '}';
}
template <typename T>
void __print(const T &x)
{
	int f = 0;
	cerr << '{';
	for (auto &i : x)
		cerr << (f++ ? "," : ""), __print(i);
	cerr << "}";
}
void _print() { cerr << "]\n"; }
template <typename T, typename... V>
void _print(T t, V... v)
{
	__print(t);
	if (sizeof...(v))
		cerr << ", ";
	_print(v...);
}
void primeFactors(int n, vector<int> &v)
{
	while (n % 2 == 0)
	{
		v.push_back(2);
		n = n / 2;
	}
	for (int i = 3; i <= sqrt(n); i = i + 2)
	{
		while (n % i == 0)
		{
			v.push_back(i);
			n = n / i;
		}
	}
	if (n > 2)
		v.push_back(n);
}
int modPower(int x, int y, int p = n1)
{
	unsigned long long res = 1;
	x = x % p;
	while (y > 0)
	{
		if (y & 1)
			res = (res * x) % p;
		y = y >> 1;
		x = (x * x) % p;
	}
	return res;
}
int power(int x, int y)
{
	unsigned long long res = 1;
	x = x;
	while (y > 0)
	{
		if (y & 1)
			res = (res * x);
		y = y >> 1;
		x = (x * x);
	}
	return res;
}

int sz = 1e6 + 5;
bool PrimeSieve[1000005]; // 1e6+5
void buildSieve()
{
	for (int i = 2; i <= sz; i++)
		PrimeSieve[i] = 1;
	PrimeSieve[0] = 0; //
	PrimeSieve[1] = 0; // 1 is neither prime nor composite
	for (int i = 2; i < sz; i++)
	{
		if (PrimeSieve[i] == 0)
			continue; // the current number itself is composite
		for (int j = 2; j * i < sz; j++)
		{
			PrimeSieve[i * j] = 0;
		}
	}
}

bool isPrime(int n)
{
	// O(sqrt(N))
	if (n == 1)
		return 0;
	for (int i = 2; i * i <= n; i++)
	{
		if (n % i == 0)
			return 0;
	}
	return 1;
}
/// ====================================PRIME utility ENDS here==================================================
int modInverse(int n, int p = n1) // using fermats little thm. [p needs to be prime which is mostly the case as mod value generally is 1e9+7]
{
	return modPower(n, p - 2, p);
}
// can also derive this using extended euclidean... however this has a much simpler code....

// =========================================Used to calculate nCr of higher values ===================================
int nCr(int n, int r, int p = n1) // faster calculation..
{
	if (n < r)
		return 0;

	if (r == 0)
		return 1;

	vector<int> fac(n + 1, 0);
	fac[0] = 1;
	for (int i = 1; i <= n; i++)
		fac[i] = (fac[i - 1] * i) % p;

	return (fac[n] * modInverse(fac[r], p) % p * modInverse(fac[n - r], p) % p) % p;
}

int modFact(int n, int p = n1)
{
	if (n >= p)
		return 0;

	int result = 1;
	for (int i = 1; i <= n; i++)
		result = (result * i) % p;

	return result;
}
int nPr(int n, int r, int p = n1)
{
	return modFact(n, p) / modFact(n - r, p);
}
bool customComparator(pair<int, int> &a, pair<int, int> &b)
{
	if (a.first == b.first)
	{
		return a.second > b.second;
	}
	return a.first < b.first;
}
int ceil2(int a, int b)
{
	return (a + b - 1) / b;
}
static void run_with_stack_size(void (*func)(void), size_t stsize)
{
	char *stack, *send;
	stack = (char *)malloc(stsize);
	send = stack + stsize - 16;
	send = (char *)((uintptr_t)send / 16 * 16);
	asm volatile(
			"mov %%rsp, (%0)\n"
			"mov %0, %%rsp\n"
			:
			: "r"(send));
	func();
	asm volatile("mov (%0), %%rsp\n" : : "r"(send));
	free(stack);
}
// Sorting
bool sorta(const pair<ll, ll> &a, const pair<ll, ll> &b) { return (a.second < b.second); }
bool sortd(const pair<ll, ll> &a, const pair<ll, ll> &b) { return (a.second > b.second); }
#ifndef ONLINE_JUDGE

#else
#define debug(...)
#define debugArr(...)
#endif
/// ====================================BIT TRICKS==================================================
// TO CHECK IF iTH BIT IS SET OR NOT
// for (int j = 0; j < 31; j++)
//             {
//                 if ((v[i] & (1 << j)))
//                 {
//                     setBit[j]++;
//                 }
//             }
/// ====================================BIT TRICKS ENDS HERE==================================================
/// ====================================Some learning here==================================================

// 1.never erase anything from ds(map,set,vector ,etc )while iterating , store the elements in another ds which to be
//   deleted then run another loop and use erase fxn.

/// ====================================PBDS==================================================
// Finding no of element smaller than x in set->s.order_of_key(x);
// finding element present at xth index in set->*s.find_by_order(x);
// pbds s; // declaration
// s.erase(s.find_by_order(s.order_of_key(v[i]))); -> to erase an item from set
// to convert into multiset change pbdsMS
/// ====================================End==================================================
// upper_bound(a.begin(),a.end(),x)-a.begin(); returns index ->arr[ind] > x
// lower_bound(a.begin(),a.end(),x)-a.begin(); returns index ->arr[ind] >= x
// MUST SORT THE ARRAY FIRST!! BEFORE USING UNIQUE
// n = unique(all(v)) - v.begin(); REMOVE DUPS AND IMP TO STORE NEW VALUE OF N SIZE OF ARRAY
struct custom_hash
{
	static uint64_t splitmix64(uint64_t x)
	{
		// http://xorshift.di.unimi.it/splitmix64.c
		x += 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}

	size_t operator()(uint64_t x) const
	{
		static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
		return splitmix64(x + FIXED_RANDOM);
	}
};

long long rng()
{
	static std::mt19937 gen(
			std::chrono::steady_clock::now().time_since_epoch().count());
	return std::uniform_int_distribution<long long>(0, INT64_MAX)(gen);
}

void main_()
{

	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	ll t = 1;
	cin >> t;
	while (t--)
	{
		int n, k, q;
		cin >> n >> k >> q;
		vector<int> v(n);
		for (int i = 0; i < n; i++)
			cin >> v[i];
		map<int, int> hash;
		for (auto &it : v)
		{
			if (hash.find(it) == hash.end())
			{
				hash[it] = rng();
			}
		}
		vector<int> pre(n), suff(n);
		set<int> seen;
		for (int i = 0; i < n; i++)
		{
			if (!seen.count(v[i]) && (v[i] % k == 0))
			{
				seen.insert(v[i]);
				pre[i] = hash[v[i]];
			}
			if (i != 0)
				pre[i] = pre[i] ^ pre[i - 1];
		}
		seen.clear();
		for (int i = n - 1; i >= 0; i--)
		{
			if (!seen.count(v[i]) && (v[i] % k == 0))
			{
				seen.insert(v[i]);
				suff[i] = hash[v[i]];
			}
			if (i != n - 1)
				suff[i] = suff[i] ^ suff[i + 1];
		}
		// debug(pre, suff);

		while (q--)
		{
			int l, r;
			cin >> l >> r;
			l--;
			r--;
			if (pre[l] == suff[n - 1 - r])
			{
				// outfile << "YES";
				cout << "YES";
			}
			else
			{
				// outfile << "NO";
				cout << "NO";
			}
			// outfile << endl;
			cout << "\n";
		}
		// cout << '\n';
	}
	// cin.close();
}

int32_t main()
{
	// run_with_stack_size(main_, 1024 * 1024 * 1024);
	main_();
	return 0;
}