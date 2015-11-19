#include <iostream>
#include <vector>
#include <functional>

/***
 * Objective - create a filter chain 
 * The shackle must return true to continue processing filters or 
 * false to stop filtering.
 * If some changed is needed, the shackle must do it.
 */

template<typename Return, typename Param>
struct shackle {

	using return_type = Return;
	using param_type = Param;
	using function_type = std::function<Return(Param)>;
	using push_type = std::function<void(Param)>;

	shackle(function_type f) : _f(f) { }

	bool operator()(param_type &val) {
		return _f(val);
	}

private:
	function_type _f;
};

template<typename ChainType>
struct chain {

	using chain_tpl = shackle<typename ChainType::return_type, typename ChainType::param_type>;
	using ChainT = ChainType;

	chain<ChainType> operator | (chain_tpl c) {
		_chains.push_back(c);
		return *this;
	}

	std::vector<chain_tpl> &chains() { return _chains; }
	void setFunction(typename ChainType::push_type f) {
		_f = f;
	}
	bool hasPushFunction() {
		return _f?true:false;
	}
	void execute(typename ChainType::param_type value) {
		_f(value);
	}

private:
	std::vector<chain_tpl> _chains;
	typename ChainType::push_type _f;
};

// template<typename ChainType>
// chain<ChainType> make_chain(typename ChainType::function_type f) {
// chain<ChainType> holder();
// }

template<typename Return, typename Param>
auto make_chain(std::function<Return(Param)> f) -> chain<shackle<Return, Param>> {
	using ChainT = shackle<Return, Param>;
	chain<ChainT> chains;
	chains | ChainT(f);
	return chains;
}

template<typename ChainType>
struct processor {
	using value_type = chain<ChainType>;

	processor<ChainType> &operator << (value_type &list) {
		_chain_lists.push_back(list);
		return *this;
	}

	void push(typename ChainType::param_type &value /*, std::function<void(typename ChainType::param_type)> f*/) {
		for (auto list : _chain_lists) {
			bool exec = true;
			for (auto shackle : list.chains()) {
				//std::cout << "shackle(" << value << ") = " << shackle(value) << std::endl;
				if (!shackle(value)) {
					exec = false;
					break;
				}
			}
			if (exec && list.hasPushFunction())
				list.execute(value);
		}
	}

private:
	std::vector<value_type> _chain_lists;
};

template<typename ChainListType>
auto make_processor(ChainListType &reference) -> processor<typename ChainListType::ChainT> {
	return processor<typename ChainListType::ChainT>();
}

int main(int argc, char* argv[])
{
	std::function<bool(int &)> gt10 = [](int &val) -> bool { return (val > 10); };
	std::function<bool(int &)> lt10 = [](int &val) -> bool { return (val < 10); };
	std::function<bool(int &)> odd  = [](int &val) -> bool { return (val % 2 != 0); };
	std::function<bool(int &)> even = [](int &val) -> bool { return (val % 2 == 0); };

	std::cout << "make chains" << std::endl;
	auto chain_1 = make_chain(gt10) | odd;
	auto chain_2 = make_chain(lt10) | even;

	chain_1.setFunction([](int &val) -> void {
		std::cout << "Grandes " << val << std::endl;
	});

	chain_2.setFunction([](int &val) -> void {
		std::cout << "Pequenos " << val << std::endl;
	});

	std::cout << "make processor" << std::endl;
	//auto proc = make_processor(chain_1, chain_2); //Devera ficar assim
	auto proc = make_processor(chain_1); //chain_1 => template de construcao
	proc << chain_1 << chain_2; 

	std::cout << "pushing" << std::endl;
	for (int i = 0; i < 20; i++) {
		proc.push(i);
	}

	char cc;
	std::cin >> cc;

	return 0;
}
