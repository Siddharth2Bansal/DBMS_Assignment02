#include <bits/stdc++.h>
using namespace std;

// Bucket class
class bucket
{
	public:
		int size;
		vector<int> value;
		int occupancy;
		int ld;
		bucket(int bucketSize, int depth);
		~bucket();
		void insert(int key);
		void printBucket();
};

// Directory class
class directory
{
	public:
		int gd;
		int bucketSize;
		int hash(int number);
		int numdir;
		map<int, bucket*> table;
		directory(int depth, int buckSize);
		~directory();
		void insert(int key);
		void insertBucket(int key);
		void multiply();
		void split(int key);
		void print();
};


int main()
{
	int initialDepth;
	int bucketSize;
	cin >> initialDepth >> bucketSize;
	directory extendibleHash(initialDepth, bucketSize);
	extendibleHash.insert(0);
	extendibleHash.insert(1);
	extendibleHash.insert(2);
	extendibleHash.insert(3);
	extendibleHash.print();
	cout << "\n\n";
	extendibleHash.insert(4);
	extendibleHash.print();
	cout << "\n\n";
	extendibleHash.insert(5);
	extendibleHash.print();
	cout << "\n\n";
	extendibleHash.insert(6);
	extendibleHash.print();
	cout << "\n\n";
	extendibleHash.insert(7);
	extendibleHash.print();
	cout << "\n\n";
	extendibleHash.insert(8);
	extendibleHash.print();
	cout << "\n\n";
}

//////////////////////////////////////////////// Bucket Functions //////////////////////////////////////////////////

// Bucket Constructor
bucket::bucket(int bucketSize, int depth)
{
	size = bucketSize;
	ld = depth;
	occupancy = 0;
}

// Bucket Deconstructor
bucket::~bucket()
{
}

// Insert Function
void bucket::insert(int key)
{
	value.push_back(key);
	occupancy++;
}

void bucket::printBucket()
{
	cout << "Local Depth: " << ld << ", Occupancy: " << occupancy << '\n';
	for(auto j : value)
	{
		cout << j << ' ';
	} 
	cout << '\n';
}

///////////////////////////////////////////////// Directory Functions ////////////////////////////////////////

// Directory Constructor
directory::directory(int depth, int buckSize)
{
	gd = depth;
	bucketSize = buckSize;
	numdir = pow(2, gd);
	for(int i=0; i<numdir; i++)
	{
		table[i] = new bucket(bucketSize, gd);
	}
}

// Hash function
int directory::hash(int number)
{
	return number % numdir;
}

// Directory Deconstructor 
directory::~directory()
{
}

// Function for inserting into hash table
void directory::insert(int key)
{
	insertBucket(key);
	int hashKey = hash(key);
	if(table[hashKey]->occupancy > bucketSize)
	{
		if(table[hashKey]->ld == gd)
		{
			multiply();
		}
		split(hashKey);
	}
}

void directory::multiply()
{
	int oldSize = numdir;
	numdir *= 2;
	gd++;
	for(int i=oldSize; i<numdir; i++)
	{
		table[i] = table[i-oldSize];
	}
}

void directory::split(int key)
{
	table[key]->ld++;
	int newSize = table[key]->ld;
	newSize = pow(2, newSize);
	int oldSize = newSize/2;
	int k1 = key % oldSize;
	bucket* ptr1 = table[key];
	bucket* ptr2 = new bucket(bucketSize, ptr1->ld);
	for(int i=0; i<numdir; i++)
	{
		if((i % oldSize == k1) && (i % newSize != k1))
		{
			table[i] = ptr2;
		}
	}
	vector<int> v1;
	for(auto j : ptr1->value)
	{
		v1.push_back(j);
	}
	ptr1->value.clear();
	ptr1->occupancy = 0;
	for(auto j : v1)
	{
		insert(j);
	}
	// table[key]->ld ++;
	// int n = table[key]->ld;
	// bucket* ptr1 = table[key];
	// bucket* ptr2 = new bucket(bucketSize, n);
	// int n1 = n/2;
	// int k = key % n1;
	// for(int i=0; i<numdir; i++)
	// {
	// 	if( (i % n1 == k) && (i % n != k))
	// 	{
	// 		table[i] = ptr2;
	// 	}
	// }
}

void directory::print()
{
	set<bucket*> list;
	for(int i=0; i<numdir; i++)
	{
		list.insert(table[i]);
	}
	for(auto j : list)
	{
		j->printBucket();
	}
}

void directory::insertBucket(int key)
{
	int hashKey = hash(key);
	table[hashKey]->insert(key);
}