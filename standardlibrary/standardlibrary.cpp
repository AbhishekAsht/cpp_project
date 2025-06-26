// standardlibrary.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "vectorIntface.h"
#include "listInterface.h"

int main()
{
    // Vector usage
    CustomVector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);

    std::cout << "Vector elements: ";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
    }
    std::cout << "\n";

    // List usage
    CustomList<std::string> lst;
    lst.push_back("world");
    lst.push_front("Hello");

    std::cout << "List elements: " << lst.front() << " " << lst.back() << "\n";

    return 0;
}