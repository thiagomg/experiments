#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <algorithm>

namespace configuration {

	template<typename Key = std::string>
	int last_of(const Key &key, const typename Key::value_type c) {
		auto it = std::find(key.rbegin(), key.rend(), c);
		return key.size() - std::distance(key.rbegin(), it);
	}
	
	template<typename Key = std::string>
	int next_of(const Key &key, int start, const typename Key::value_type c) {
		auto it = std::find(key.begin()+start, key.end(), c);
		return std::distance(key.begin(), it)-start;
	}

	template<typename Key = std::string, typename Value = std::string>
	struct config_holder {
				
		using key_iter = typename Key::const_iterator;
		using val_iter = typename Value::const_iterator;
		using key_pair = std::pair<key_iter, key_iter>;
		using val_pair = std::pair<val_iter, val_iter>;
		using range = std::pair<key_pair, val_pair>;
		using config_map = std::unordered_map<Key, Value>;
		using map_value = typename std::unordered_map<Key,Value>::value_type;
		
		auto add_item(key_iter kb, key_iter ke, val_iter vb, val_iter ve) -> void {
			_items.insert( make_pair(Key(kb, ke), Value(vb, ve)) );
		}
		
		auto get(const Key &key) const -> const Value & {
			auto it = _items.find(key);
			if( it == _items.end() )
				return _empty;
			return it->second;
		}

		using adder_func = std::function<void(const std::pair<Key,Value> &pair)>;
		
		void iterate_and_check(const Key &prefix, adder_func f_adder) const {
			std::for_each(begin(_items), end(_items), [&prefix, &f_adder](const std::pair<Key,Value> &pair) {
				//let's check prefix
				if( std::equal( begin(prefix), end(prefix), begin(pair.first) ) )
					f_adder(pair);
			});
		}
		
		auto prefix(const Key &prefix) const -> std::vector<map_value> {
			std::vector<map_value> values;
			iterate_and_check(prefix, [&values](const std::pair<Key,Value> &pair) {
				values.push_back(pair);				
			});
			return values;
		}		

		auto sufix(const Key &prefix) const -> std::unordered_set<Key> {
			std::unordered_set<Key> values;
			iterate_and_check(prefix, [&values, &prefix](const std::pair<Key,Value> &pair) {
				values.insert( pair.first.substr( prefix.size() ) );
			});
			return values;
		}
		
		auto next_token(const Key &prefix, const typename Key::value_type separator) const -> std::unordered_set<Key> {
			std::unordered_set<Key> values;
			int pos = last_of(prefix, separator);
			iterate_and_check(prefix, [&values, &prefix, &pos, &separator](const std::pair<Key,Value> &pair) {
				int npos = next_of(pair.first, pos, separator);
				values.insert( pair.first.substr( pos, npos ) );
			});
			return values;
		}
		
		auto items() const -> const std::unordered_map<Key, Value> & {
			return _items;
		}
		
	private:
		config_map _items;
		Value _empty;
		
	};

	template<typename AdderFunc>
	void crack(const std::string &line, char delim, AdderFunc f) {
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
		
		for(auto item : config.items()) {
			std::cout << "Item: [" << item.first << "] => [" << item.second << "]" << std::endl;
		}
		
		return config;
		
	}

}

int main() {
	
	std::string fname = "example.cfg";
	auto cfg = configuration::load(fname);
	
	const std::string &log_dir = cfg.get(std::string("log.base_dir"));
	auto plugins = cfg.prefix("plugin.");
	
	std::cout << log_dir << std::endl;

	for(auto p : plugins) {
		std::cout << p.first << "=>" << p.second << std::endl;
	}
	std::cout << "--------------" << std::endl;

	auto suf = cfg.sufix("plugin.socket");
	for(auto p : suf) {
		std::cout << p << std::endl;
	}
	std::cout << "--------------" << std::endl;

	auto tok = cfg.next_token("plugin.socket", '.');
	for(auto p : tok) {
		std::cout << p << std::endl;
	}
	
	const std::string &id = cfg.get(std::string("plugin.fix.id"));
	std::cout << "-> " << id << std::endl;
	
}