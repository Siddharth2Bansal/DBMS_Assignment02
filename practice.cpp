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
		vector <bucket*> time;
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
	int code, val;
	bool loop = true;
	while(loop)
	{
		cin >> code;
		switch (code)
		{
		case 2:
			cin >> val;
			extendibleHash.insert(val);
			break;
		case 3:
			// search
			break;
		case 4:
			// delete
			break;
		case 5:
			extendibleHash.print();
			break;
		case 6:
			loop = false;
			break;
		}
	}
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
		time.push_back(table[i]);
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
	time.push_back(ptr2);
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
}

void directory::print()
{
	cout << gd << '\n' << time.size() << '\n';

	for(auto j : time)
	{
		cout << j->occupancy << ' ' << j->ld << '\n';
	}
}

void directory::insertBucket(int key)
{
	int hashKey = hash(key);
	table[hashKey]->insert(key);
}