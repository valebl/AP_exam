#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "bst.hpp"

int main() {

    using node_type = std::pair<const int, double>;

    std::cout << '\n' << "** RUNNING SOME TESTS ON THE CODE **" << '\n' << std::endl;

    std::cout << "------------------------------------" << std::endl;

    std::cout << "- Create and empy tree b1 and print it -" << '\n' << std::endl;

    bst<int,double> b1;

    std::cout << "b1, using the put-to operator: " << std::endl;
    std::cout << b1 << std::endl;

    std::cout << '\n' << "b1, using the Print() member function: " << std::endl;
    b1.Print();

    std::cout << "------------------------------------" << std::endl;

    std::cout << "- Insert the keys: {8, 3, 10, 1, 6, 4, 7, 14} using l-value insert -" << std::endl;

    const node_type p8 {8,8};  
    const node_type p3 {3,3};
    const node_type p10 {10,10};
    const node_type p1 {1,1};
    const node_type p6 {6,6};
    const node_type p4 {4,4};
    const node_type p7 {7,7};
    const node_type p13 {13,13};
    const node_type p14 {14,14};

    b1.insert(p8); // l-value
    b1.insert(p3);
    b1.insert(p10);
    b1.insert(p1);
    b1.insert(p6);
    b1.insert(p4);
    b1.insert(p7);
    b1.insert(p14);

    std::cout << '\n' << "b1, using the put-to operator: " << std::endl;
    std::cout << b1 << std::endl;

    std::cout << '\n' << "b1, using the Print() member function: " << std::endl;
    b1.Print();   

    std::cout << "------------------------------------" << std::endl;

    std::cout << "- Insert the key: {13} using r-value insert -" << std::endl;

    b1.insert(std::make_pair(13,13)); // r-value

    std::cout << '\n' << "b1, using the put-to operator: " << std::endl;
    std::cout << b1 << std::endl;

    std::cout << '\n' << "b1, using the Print() member function: " << std::endl;
    b1.Print();

    std::cout << "------------------------------------" << std::endl;

    std::cout << "- Insert the key: {5} using emplace -" << std::endl;

    std::cout << '\n' << "b1.emplace(5,5);" << std::endl;

    b1.emplace(5,5);

    std::cout << '\n' << "b1, using the put-to operator: " << std::endl;
    std::cout << b1 << std::endl;

    std::cout << '\n' << "b1, using the Print() member function: " << std::endl;
    b1.Print();



    std::cout << "------------------------------------" << std::endl;

    std::cout << "- Copy constructor -" << std::endl;

    bst<int,double> b2 {b1};

    std::cout << '\n' << "bst<int,double> b2 {b1};" << std::endl;

    std::cout << '\n' << "b2, using the put-to operator: " << std::endl;
    std::cout << b1 << std::endl;

    std::cout << '\n' << "b2, the Print() member function: " << std::endl;
    b1.Print();

    std::cout << "------------------------------------" << std::endl;

    std::cout << "- Copy assignment -" << std::endl;

    bst<int,double> b3;
    b3.insert(p6);
    b3.insert(p4);
    b3.insert(p1);

    std::cout << '\n' << "Create a tree b3 with keys {6, 4, 1}" << std::endl;

    std::cout << '\n' << "b3, using the put-to operator: " << std::endl;
    std::cout << b3 << std::endl;

    std::cout << '\n' << "b3, using the Print() member function: " << std::endl;
    b3.Print();

    std::cout << "b3 = b1;" << std::endl;

    b3 = b1;

    std::cout << '\n' << "b3, using the put-to operator: " << std::endl;
    std::cout << b1 << std::endl;

    std::cout << '\n' << "b3, using the Print() member function: " << std::endl;
    b1.Print();

    std::cout << "------------------------------------" << std::endl;

    std::cout << "- Clear -" << std::endl;

    b1.clear();

    std::cout << '\n' << "b1.clear;" << std::endl;

    std::cout << '\n' << "b1, using the put-to operator: " << std::endl;
    std::cout << b1 << std::endl;

    std::cout << '\n' << "b1, using the Print() member function: " << std::endl;
    b1.Print();

    std::cout << "------------------------------------" << std::endl;    

    std::cout << "- Balance -" << std::endl;

    std::cout << '\n' << "Unbalanced tree b1: {1, 3, 4, 5, 6, 7, 8, 10, 13, 14} -" << std::endl;

    const node_type p5 {5,5};

    b1.insert(p1);
    b1.insert(p3);
    b1.insert(p4);
    b1.insert(p5);
    b1.insert(p6);
    b1.insert(p7);
    b1.insert(p8);
    b1.insert(p10);
    b1.insert(p13);
    b1.insert(p14);

    std::cout << '\n' << "b1, using the put-to operator: " << std::endl;
    std::cout << b1 << std::endl;

    std::cout << '\n' << "b1, using the Print() member function: " << std::endl;
    b1.Print();

    std::cout << "b1.balance();" << std::endl;

    b1.balance();

    std::cout << '\n' << "b1, using the put-to operator: " << std::endl;
    std::cout << b1 << std::endl;

    std::cout << '\n' << "b1, using the Print() member function: " << std::endl;
    b1.Print();

    std::cout << "------------------------------------" << std::endl;    

    std::cout << "- Subscripting operator -" << std::endl;

    std::cout << '\n' << "b1[8] = " << b1[8] << std::endl;

    std::cout << '\n' << "b1[20] = " << b1[20] << std::endl;

    std::cout << '\n' << "b1, using the put-to operator: " << std::endl;
    std::cout << b1 << std::endl;

    std::cout << '\n' << "b1, using the Print() member function: " << std::endl;
    b1.Print();

    std::cout << "------------------------------------" << std::endl;  

    std::cout << "- Erase -" << std::endl;  

    std::cout << '\n' << "b1.erase(7);" << std::endl;

    b1.erase(7);

    std::cout << '\n' << "b1, using the put-to operator: " << std::endl;
    std::cout << b1 << std::endl;

    std::cout << '\n' << "b1, using the Print() member function: " << std::endl;
    b1.Print();

    std::cout << "b1.erase(8);" << std::endl;

    b1.erase(8);

    std::cout << '\n' << "b1, using the put-to operator: " << std::endl;
    std::cout << b1 << std::endl;

    std::cout << '\n' << "b1, using the Print() member function: " << std::endl;
    b1.Print();


    std::cout << "------------------------------------" << std::endl;  

    std::cout << "- Find -" << std::endl; 

    std::cout << '\n' << "b1.find(5);" << std::endl;

    std::cout << '\n' << "The returned iterator points to: ";

    std::cout << *b1.find(5) << std::endl; 

    std::cout << std::endl;

}