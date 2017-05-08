#include <iostream>
#include <cstdlib> //rand
#include <ctime> //time
#include <string>
#include "KSet.h"

using namespace std;

template <typename T>
void fillSet(KSet<T>& set)
{
    srand(time(NULL));
    for (int i = 0; i < rand() % 20; i++)
        set.insert(rand() % 20);
}

template <typename T>
ostream& operator<<(ostream& os, const KSet<T>& set)
{
    for (typename KSet<T>::iterator it = set.begin(); it != set.end(); it++)
        os << *it << ' ';
    os << endl;
    return os;
}

template <typename T>
void printSet(const KSet<T>& set)
{
    for (typename KSet<T>::iterator it = set.begin(); it != set.end(); it++)
        cout << *it << ' ';
    cout << endl;
}

using namespace std;

int main()
{
    KSet<int> s, s1, s2;
    cout << boolalpha;
    //s = {100, 200, 300};
    s.insert(100).insert(200).insert(300);
    cout << "s:\n" << s;
    fillSet(s1);
    cout << "s1:\n" << s1;
    //fillSet(s2);
    //fillSet(s2);
    //cout << "s2:\n" << s2;

    s.insert(2);
    s.insert(10);
    cout << "Insert 2, 10 to s. s:\n" << s;
    //printSet(s);
    //fillSet(s1);
    //s1.insert(7);
    //printSet(s1);
   // s += s1;
    //printSet(s);
    cout << "s1 - s:\n" << (s1 - s);
    cout << "s + s1:\n" << (s + s1);
    cout << "s * s1\n" << (s * s2);
    cout << "s / s1\n" << (s / s1);
    cout << "is 1 in s? " <<  s.contains(1) << endl;
    cout << "swap sets:\n";
    s.swap(s1);
    cout << "s:\n" << s;
    cout << "s1\n" << s1;

//    s.clear();
//    s1.clear();
//    s1.insert(1);
//    s1.insert(2);
//    s.insert(1);
//    s.insert(2);
//
//    printSet(s);
//    cout << s.size() << endl;
//    printSet(s1);
//    cout << s1.size() << endl;
//    printSet(s * s1);
//    cout << s2.size() << endl;
//    cout << (s > s1);
//    //s = {10, 20, 30, 40};
//    printSet(s);
//    printSet(s.insert(228));

    system("pause");

    return 0;
}
