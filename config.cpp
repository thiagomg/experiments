#include <fstream>

#include <iostream>
#include <unordered_map>
#include <functional>
#include <algorithm>

//using namespace std;

namespace configuration {

	template<typename Key = std::string, typename Value = std::string>
	struct config_holder {
		
		//using key = std::string;
		//using value = std::string;
		
		using key_iter = typename Key::const_iterator;
		using val_iter = typename Value::const_iterator;
		using key_pair = std::pair<key_iter, key_iter>;
		using val_pair = std::pair<val_iter, val_iter>;
		using range = std::pair<key_pair, val_pair>;
		
		void add_item(key_iter kb, key_iter ke, val_iter vb, val_iter ve) {
			_items.insert( make_pair(Key(kb, ke), Value(vb, ve)) );
		}
		
		const Value &get(const Key &key) const {
			auto it = _items.find(key);
			if( it == _items.end() )
				return _empty;
			return *it;
		}
		
		std::vector<const Value &> prefix(const Key &prefix) const {
			std::vector<const Value &> values;
			std::for_each(begin(_items), end(_items), [&values](auto pair){
				values.push_back(pair.second);
			});
			return values;
		}
		
		
	//private:
		std::unordered_map<Key, Value> _items;
		static Value _empty;
		
	};

	
	void crack(const std::string &line, char delim, 
		std::function<void(std::string::const_iterator, std::string::const_iterator, std::string::const_iterator)> f) {
		auto b = std::begin(line);
		auto e = std::end(line);
		auto pos = line.find(delim);
		if( pos != std::string::npos ) {
			f(b, b+pos, e);
		}
	}
	
	template<typename Key = std::string, typename Value = std::string>
	config_holder<Key, Value> load(const std::string &file_name) {
		
		using cfg = config_holder<Key, Value>;
		cfg config;
		
		std::ifstream istr;
		istr.open(file_name);
		
		using iter_type = std::string::const_iterator;
		std::function<void(iter_type, iter_type, iter_type)> f = [&config](iter_type b, iter_type m, iter_type e) {
			std::string k(b, m);
			std::string v(m+1, e);
			//b-m => key / m+1-e => value
			config.add_item( b, m, m+1, e );
		};
		
		while( istr.good() )  {
			std::string line;
			std::getline(istr, line);
			crack(line, '=', f);
		}
		
		for(auto item : config._items) {
			std::cout << "Item: [" << item.first << "] => [" << item.second << "]" << std::endl;
		}
		
		return config;
		
	};

}

int main() {
	
	std::string fname = "example.cfg";
	configuration::load(fname);
	
}