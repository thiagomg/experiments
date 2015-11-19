#include <iostream>
#include <vector>
#include <functional>

// T, U=T
// C(arr(T)) -> C(arr(T)) -> C(W)

template<typename T, typename U>
struct shackle {
	
	shackle(std::function<U(T)> f) : _f(f) { }
	
	U operator | (T &t) {
		return _f(t);
	}
	
	U operator | (shackle<T,U> &s) {
		return _f(t);
	}
	
	std::function<U(T)> _f;
	
};

int main(int argc, char* argv[]) {

	using Vec = std::vector<int>;
	
	std::function<Vec(Vec)> mult = [](Vec v) -> Vec { for(auto &i : v) { i *= 10; } return v; };
	std::function<Vec(Vec)> inc  = [](Vec v) -> Vec { for(auto &i : v) { i += 1;  } return v; };
	
	Vec v {1, 2, 3};
	
	shackle<Vec, Vec> sm(mult);
	shackle<Vec, Vec> si(inc);
	
	auto vr = sm | v;
	
	for(auto i : vr) { std::cout << i << " "; }
	std::cout << std::endl;
	
	
	stream s;
	s | filter( gettype{"X"} ) | filter( isBook ) | exec( updateBook );
	s | filter( gettype{"X"} ) | filter( isStat   | exec( updateStats );
	s | filter( gettype{"y"} ) | exec( updateInstr );
	
}