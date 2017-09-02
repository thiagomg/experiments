#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>

class matrix {
public:
  std::vector<int> data;
  int cols, rows;

  matrix(int cols, int rows) : data(cols*rows), cols(cols), rows(rows) { }

  matrix(const matrix& o) = delete; //: data(o.data), cols(o.cols), rows(o.rows) { std::cout << "move failed!\n"; }
  matrix(matrix&& o) : data(std::move(o.data)), cols(o.cols), rows(o.rows) { }

  matrix& operator=(const matrix& other) = delete;
  /*  {
    data = other.data;
    cols = other.cols;
    rows = other.rows;
    std::cout << "copy assigned\n";
    return *this;
    }*/
  matrix& operator=(matrix&& other)
  {
    data = std::move(other.data);
    cols = other.cols;
    rows = other.rows;
    std::cout << "move assigned\n";
    return *this;
  }
  
  int _get_pos(int x, int y) const {
    return x + (y*cols);
  }

  int& operator [] (int pos) {
    return data[pos];
  }

  int& operator () (int x, int y) {
    return data[_get_pos(x,y)];
  }

  int operator() (int x, int y) const {
    return data[_get_pos(x,y)];
  }

  bool valid(int x, int y) const {
    return _get_pos(x,y) < data.size();
  }

  std::vector<int>::iterator begin() {
    return data.begin();
  }
  
  std::vector<int>::iterator end() {
    return data.end();
  }

};

struct matrix_view {
  matrix &_data;
  const int _x, _y;
  const int cols, rows;

  matrix_view(matrix &data, int x, int y, int cols, int rows) :
    _data(data), _x(x), _y(y), cols(cols), rows(rows) { }

  int _get_x(int x) const {
    int rx = _x + x;
    if( rx >= _data.cols ) { rx -= _data.cols; }
    if( rx < 0 ) { rx += _data.cols; }
    return rx;
  }

  int _get_y(int y) const {
    int ry = _y + y;
    if( ry >= _data.rows ) { ry -= _data.rows; }
    if( ry < 0 ) { ry += _data.rows; }
    return ry;
  }
  
  int& operator () (int x, int y) {
    return _data(_get_x(x), _get_y(y));
  }
  
};

matrix_view get_surrounding(matrix &data, int x, int y) {
  matrix_view mv(data, x-1, y-1, 3, 3);
  return std::move(mv);
}

void show_view(matrix_view &mv) {
  std::cout << std::endl;
  for(int y=0; y < mv.rows; ++y) {
    for(int x=0; x < mv.cols; ++x) {
      std::cout << mv(x,y) << " ";
    }
    std::cout << std::endl;
  }
}

void show_matrix(matrix &v) {
  std::cout << "\x1B[2J\x1B[H";
  //std::cout << std::endl;
  for(int y=0; y < v.rows; ++y) {
    for(int x=0; x < v.cols; ++x) {
      std::cout << v(x,y) << " ";
    }
    std::cout << std::endl;
  }
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(1s);
}

int count_living(matrix_view &mv) {
  int total = 0;  
  for(int y=0; y < mv.rows; ++y) {
    for(int x=0; x < mv.cols; ++x) {
      if(x == 1 && y == 1) {
	continue;
      }
      total += mv(x, y);
    }
  }
  return total;
}

struct life {
  
  matrix _nm;

  life() : _nm(9, 9) {
  }

  matrix step(matrix &m) {
    
    for(int y=0; y < m.rows; ++y) {
      for(int x=0; x < m.cols; ++x) {
	auto mv = get_surrounding(m, x, y);
	int count = count_living(mv);
	
	//Let's apply the rules
	if( count == 3 || (count == 2 && (mv(1, 1) == 1)) ) {
	  _nm(x, y) = 1;
	} else {
	  _nm(x, y) = 0;
	}	
	
      }
    }

    matrix tmp = std::move(_nm);
    _nm = std::move(m);
    m = std::move(tmp);

    return std::move(m);
  }
  
};
  
int main(int argc, char **argv) {
  
  int cols = 9;
  int rows = 9;
  
  matrix v(cols, rows);

  //matrix_view mv(v, -1, -1, 3, 3);
  matrix_view mv = get_surrounding(v, 2, 2);
  mv(1, 0) = 1;
  mv(1, 1) = 1;
  mv(1, 2) = 1;

  //show_view(mv);

  life lf;
  while(true) {
    show_matrix(v);
    v = lf.step(v);
  }  
  
}
