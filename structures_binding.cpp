#include <tuple>
#include <string>
#include <iostream>

#include <typeinfo>

auto f1() -> std::tuple<std::string, double> {
    return std::make_tuple("Massari", 2.0);
}

auto f2() {
    return std::make_tuple("Guedes", 3.0);
}

int main() {

    auto tuple = std::make_tuple("Thiago", 1.0);
    
    //unpacking the tuple
    auto [res, ok] = tuple;
    
    //unpacking directly from the function
    auto [res1, ok1] = f1();
    auto [res2, ok2] = f2();
 
    std::cout << res << " => " << ok << std::endl;
    std::cout << res1 << " => " << ok1 << std::endl;
    std::cout << res2 << " => " << ok2 << std::endl;

    //Another example
    //Let's search using a regex and return two iterator with the boundaries of the result
    //auto [begin, end] = search_regex("\w+=(\w+)", buffer);
    
}
