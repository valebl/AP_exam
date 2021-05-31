#include <iostream>
#include <chrono>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm> // std::random_shuffle

#include "bst.hpp"

using Clock = std::conditional<std::chrono::high_resolution_clock::is_steady,
                               std::chrono::high_resolution_clock,
                               std::chrono::steady_clock>::type;

using TimeUnits = std::chrono::nanoseconds;
constexpr char unit[] = "nanoseconds";


int main() {

    
    std::cout << '\n' << "** RUNNING THE BENCHMARK **" << '\n' << std::endl;

    int MIN = 1000;
    int MAX = 50000;
    int INCR = 100;
    int runs = 50;

    std::ofstream out_bst;
    std::ofstream out_map;
    std::ofstream out_bst_balanced;

    out_bst.open("bench_bst.txt");
    out_bst_balanced.open("bench_bst_balanced.txt");
    out_map.open("bench_map.txt");

    out_bst << "Unit: " << unit << "\t" << "MIN: " << MIN << "\t" << "INCR: " << INCR << "\t" << "MAX: " << MAX << std::endl;
    out_bst_balanced << "Unit: " << unit << "\t" << "MIN: " << MIN << "\t" << "INCR: " << INCR << "\t" << "MAX: " << MAX << std::endl;
    out_map << "Unit: " << unit << "\t" << "MIN: " << MIN << "\t" << "INCR: " << INCR << "\t" << "MAX: " << MAX << std::endl;

    for (auto nodes = MIN; nodes <= MAX; nodes+=INCR){

        // out_bst << nodes << '\t';
        // out_bst_balanced << nodes << '\t';
        // out_map << nodes << '\t';

        std::vector<int> v(nodes);
        for (auto i = 0; i < nodes; ++i)
            v[i] = i;

        std::random_shuffle(v.begin(),v.end());

        bst<int, int> t;        
        for ( const auto& i : v ) { t.emplace(i,i); }  

        bst<int, int> tb;
        for ( const auto& i : v ) { tb.emplace(i,i); }    
        tb.balance();
        
        std::map<int, int> m;
        for ( const auto& i : v ) { m.emplace(i,i); }    

        std::random_shuffle(v.begin(),v.end());
        
        for (auto r = 1; r < nodes/runs; ++r){

            auto start =  Clock::now();
            for ( int i = (r-1)*runs; i < r*runs; ++i )
                m.find(v[i]);
            auto end = Clock::now();
            out_map << std::chrono::duration_cast<TimeUnits>((end-start)).count() << " ";

            start = Clock::now();
            for ( int i = (r-1)*runs; i < r*runs; ++i )
                t.find(v[i]);
            end = Clock::now();
            out_bst << std::chrono::duration_cast<TimeUnits>((end-start)).count() << " ";

            start = Clock::now();
            for ( int i = (r-1)*runs; i < r*runs; ++i )
                tb.find(v[i]);
            end = Clock::now();
            out_bst_balanced << std::chrono::duration_cast<TimeUnits>((end-start)).count() << " ";

        }

        out_map << std::endl;
        out_bst << std::endl;
        out_bst_balanced << std::endl;

    }

    out_bst.close();
    out_map.close();
    out_bst_balanced.close();

}