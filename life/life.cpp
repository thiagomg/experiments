#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>

#include <random>

//Please use random access iterator
template<typename Type, typename Storage=std::vector<Type>>
class matrix {
public:
    Storage data;
    int cols, rows;
    
    matrix(int cols, int rows) : data(cols*rows), cols(cols), rows(rows) { }
    
    matrix(const matrix& o) = delete; //disable copy
    matrix(matrix&& o) : data(std::move(o.data)), cols(o.cols), rows(o.rows) { }
    
    matrix& operator=(const matrix& other) = delete; //disable copy
    matrix& operator=(matrix&& other)
    {
        data = std::move(other.data);
        cols = other.cols;
        rows = other.rows;
        return *this;
    }
    
    Type _get_pos(int x, int y) const {
        return x + (y*cols);
    }
    
    Type& operator () (int x, int y) {
        return data[_get_pos(x,y)];
    }
    
    int operator() (int x, int y) const {
        return data[_get_pos(x,y)];
    }
    
    bool valid(int x, int y) const {
        return _get_pos(x,y) < data.size();
    }
    
};

template<typename Type>
struct matrix_view {
    matrix<Type> &_data;
    const int _x, _y;
    const int cols, rows;
    
    matrix_view(matrix<int> &data, int x, int y, int cols, int rows) :
    _data(data), _x(x), _y(y), cols(cols), rows(rows) { }
    
    Type _get_x(int x) const {
        int rx = _x + x;
        if( rx >= _data.cols ) { rx -= _data.cols; }
        if( rx < 0 ) { rx += _data.cols; }
        return rx;
    }
    
    Type _get_y(int y) const {
        int ry = _y + y;
        if( ry >= _data.rows ) { ry -= _data.rows; }
        if( ry < 0 ) { ry += _data.rows; }
        return ry;
    }
    
    Type& operator () (int x, int y) {
        return _data(_get_x(x), _get_y(y));
    }
    
};

struct life_counter {
    int operator() (matrix_view<int> &mv) {
        int total = 0;  
        for(int y=0; y < mv.rows; ++y) {
            for(int x=0; x < mv.cols; ++x) {
                //Let's not count itself
                if(x == 1 && y == 1) {
                    continue;
                }
                
                if( mv(x, y) != 0 )
                    total += 1;
            }
        }
        return total;
    }
};

struct life_rule {

    int increase_until(int val, int max) {
	val += 1;
	if( val > max ) return max;
	return val;
    }
    
    int operator() (matrix_view<int> &view, int count) {
        //Rules of the game - https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
        int cur_cell = view(1, 1);
        
        //Any live cell with two or three live neighbours lives on to the next generation.
        if( cur_cell != 0 && (count == 2 || count == 3) ) {
            //To make it fun, let's increase generation
            return increase_until(cur_cell, 10);
        }
        
        //Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
        if( cur_cell == 0 && count == 3 ) {
            //Baby cell !
            return 1;
        }
        
        //Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
        //Any live cell with more than three live neighbours dies, as if by overpopulation.
        return 0;
    }
    
};

template<typename T>
matrix_view<T> get_surrounding(matrix<T> &data, int x, int y) {
    matrix_view<T> mv(data, x-1, y-1, 3, 3);
    return std::move(mv);
}

template <typename Rule, typename Counter>
struct life_processor {
    
    matrix<int> _nm;
    Rule rule;
    Counter counter;
    
    life_processor(const matrix<int> &v) : _nm(v.cols, v.rows) {
    }
  

    matrix<int> step(matrix<int> &m) {
        
        for(int y=0; y < m.rows; ++y) {
            for(int x=0; x < m.cols; ++x) {
                auto mv = get_surrounding(m, x, y);
                int count = counter(mv);
                
                //Let's apply the rules
                _nm(x, y) = rule(mv, count);                
            }
        }
        
	std::swap(m, _nm);
        
        return std::move(m);
    }
    
};

void fill_random(matrix<int> &m) {
    
    std::random_device rd;
    std::mt19937_64 generator(rd());
    std::uniform_int_distribution<int> distribution(0,1);
    auto dice_2_side = std::bind ( distribution, generator );   //2 side dice????
    
    for(auto it = m.data.begin(); it != m.data.end(); ++it) {
        *it = dice_2_side();
    }
    
}

void fill_line(matrix<int> &v) {
    matrix_view<int> mv = get_surrounding(v, 2, 2);
    mv(1, 0) = 1;
    mv(1, 1) = 1;
    mv(1, 2) = 1;
}

template<typename T>
void show_matrix(T &v) {
    std::cout << "\x1B[2J\x1B[H";
    for(int y=0; y < v.rows; ++y) {
        for(int x=0; x < v.cols; ++x) {
            
            char c = ' ';
            if( v(x,y) > 9 ) c = '*';
            else if( v(x,y) > 0 ) c = '0' + v(x,y); 
            
            std::cout << c;
        }
        std::cout << std::endl;
    }
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(150ms);
}

int main(int argc, char **argv) {
    
    int cols = 60;
    int rows = 40;
    
    matrix<int> v(cols, rows);
        
    fill_random(v);
    
    life_processor<life_rule, life_counter> life(v);
    while(true) {
        show_matrix(v);
        v = life.step(v);
    }  
    
}
