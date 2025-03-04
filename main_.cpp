#include <iostream>
#include "ring.h"

int main(){
	MBRing<int, int> R1, R2, R3;
	cout << "Ring 1:" << endl;
	R1.insert(1, 1);
	R1.insert(11, 11);
	R1.insert(111, 111);
	R1.insert(1111, 1111);
	cout << "Number of elements in R1: " << R1.size() << endl;
	R1.print();
	cout << endl;

	cout << "Ring 2:" << endl;
	R2.insert(2, 2);
	R2.insert(22, 22);
	R2.insert(222, 222);
	R2.insert(2222, 2222);
	cout << "Number of elements in R2: " << R2.size() << endl;
	R2.print();
	cout << endl;

	cout << "Result1 Ring:" << endl;
	MBRing<int, int> Result1;
	cout << "Number of elements in Result Ring: " << Result1.size() << endl;
	Result1.print();
	cout << endl;

	cout << "Result2 Ring:" << endl;
	MBRing<int, int> Result2(R1);
	Result2 += R2;
	cout << "Number of elements in Result2 Ring: " << Result2.size() << endl << endl << endl;
	Result2.print();

	cout <<"\nSplit by position Ring:" << endl;
	MBRing<int, int> s1, s2;
	split_by_pos(Result2, 0, 1, 2, 3, 10, s1, s2);
	cout << "\nSplit1 Ring:" << endl;
	s1.print();
	cout << "\nSplit2 Ring:" << endl;
	s2.print();

	cout <<"\nSplit by key Ring:" << endl;
	MBRing<int, int> s3, s4;
	split_by_key(Result2, 1, 111, 2, 3, 3, s3, s4);
	cout <<"\nSplit3 Ring:" << endl;
	s3.print();
	cout <<"\nSplit4 Ring:" << endl;
	s4.print();
}