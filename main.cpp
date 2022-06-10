#include "rbt.h"
#include "Timer.h"
#include<iostream>
#include<vector>
#include "SimpleMemory.h"

auto get_rbt() {
    pic10c::rbt<double, std::greater<double>> vals;
    vals.insert(3.3);
    vals.insert(1.1);
    vals.insert(4.4);
    vals.insert(5.3);
    vals.emplace(1.1); // duplicate
    vals.emplace(); // adds 0
    return vals;
}

int main() {
    // basic inserting, handling duplicates,  etc.
    pic10c::rbt<std::string> colours;
    colours.insert("red");
    colours.insert("orange");
    colours.insert("yellow");
    colours.insert("green");
    colours.insert("blue");
    colours.insert("indigo");
    colours.insert("green"); // dupicate
    colours.insert("violet");
    std::cout << "colours size: " << colours.size() << '\n';

    // print the structure...
    std::cout << "colours current structure:\n";
    colours.print();

    // check find
    std::vector<std::string> cols{ "red", "cherry", "green" };

    // green will be there... and it has
    std::cout << "green has " << colours.find(cols.back())->size() << " characters.\n";

     //try to erase all these
    for (const auto& s : cols) {
        if (auto p = colours.find(s); p != colours.end()) {
            colours.erase(p);
        }
    }
    std::cout << "colours new structure:\n";
    colours.print();

    // call a function to get rbt
    const auto doubles = get_rbt();

    // print the doubles
    std::cout << "printing the doubles:\n";
    for (const auto& d : doubles) {
        std::cout << d << '\n';
    }

    pic10c::rbt<int> ints;
    pic10c::rbt<int> ints2;

    pic10c::swap(ints, ints2);

    std::cout << "now we do some time trails...\n";

    simple_timer::timer<'u'> t;

    std::cout << "time each of 1000 inserations:\n";
    for (size_t i = 0; i < 1000; ++i) {
        t.tick();
       ints.insert(i);
      std::cout << t.tock() << ' ';
    }
    std::cout << "\n";
   std::cout << "time each of 1000 removals:\n";
    for (size_t i = 0; i < 1000; ++i) {
        t.tick();
        auto p = ints.find(i);
        ints.erase(p);
        std::cout << t.tock() << ' ';
    }
    return 0;
}