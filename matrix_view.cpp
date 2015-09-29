//Thiago Massari Guedes
//2015-09-29

//Missing:
//Proper iterator implementation - operator =, etc
//Change rotate_view to std::rotate
//Templatize the structs

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;


template<typename T>
void fill_v(T &v, int count) {
    typename T::value_type i {};
    std::generate_n(back_inserter(v), 9, [&i]() { return ++i; });
}

struct matrix_view_data {
    matrix_view_data(vector<int> &data, int cols, int rows) : _data(data), _rows(rows), _cols(cols) { }
    vector<int> &_data;
    int _rows, _cols;

    int get(int x, int y) {
        return _data[ (y*_cols) + x ];
    }
};

struct matrix_view {

    vector<int> &_data;
    int _rows, _cols;

    struct point {
        int x, y;

        point(int x, int y) : x(x), y(y) { }
        point() = default;

        bool operator==(const point &src) {
            if (this->x == src.x && this->y == src.y)
                return true;
            return false;
        }

        bool operator!=(const point &src) {
            return !(this->operator==(src));
        }
    };

    matrix_view(vector<int> &data, int cols, int rows) : _data(data), _rows(rows), _cols(cols) { }

    struct iterator {
        matrix_view::point _p;
        matrix_view_data _data;
        int _count = 0;
        int _total;

        iterator(matrix_view_data &data, int x, int y) : _data(data) {
            _total = (_data._cols * 2) + (_data._rows-2) * 2;
            _p.x = x;
            _p.y = y;
        }
        iterator &operator ++(int) {
            if( _count < _total ) {
                _count++;
            }
            _p = next(_p);
            return *this;
        }

        int operator * () {
            return _data.get(_p.x, _p.y);
        }

        point &get_point() {
            return _p;
        }

        bool operator == (iterator &src) {
            if( src._count == _total && //src = end.
                _count >= _total ) //this == end;
                return true;
            if( src._p == this->_p )
                return true;
            return false;
        }
        bool operator == (const iterator &src) {
            int c1 = src._count;
            int c2 = _count;
            int t1 = src._total;
            int t2 = _total;
            if( src._count == _total ) { //src = end.
                if( _count >= _total ) //this == end;
                    return true;
                return false;
            }
            const matrix_view::point &cp = this->_p;
            if( src._p.x == cp.x && src._p.y == src._p.y )
                return true;
            return false;
        }
        bool operator != (iterator &src) {
            return !(this->operator==(src));
        }
        bool operator != (const iterator &src) {
            return !(this->operator==(src));
        }

        point next(const point &p) {
            point ret;
            int max_r = _data._rows-1;
            int max_c = _data._cols-1;

            if( p.x == 0 && p.y == 0 )
                return point(0,1);
            if( p.x == max_c && p.y == max_r )
                return point(max_c, max_r-1);
            if( p.y == 0 )
                return point(p.x-1, p.y);
            if( p.y == max_r )
                return point(p.x+1, p.y);
            if( p.x == 0 )
                return point(p.x, p.y+1);
            if( p.x == max_c )
                return point(p.x, p.y-1);

            return ret;
        }

    };

    //TODO: tirar daqui e manter internamente um matrix_view_data
    int get(int x, int y) {
        return _data[ (y*_cols) + x ];
    }

    iterator begin() {
        matrix_view_data data(_data, _cols, _rows);
        return iterator(data, 0, 0);
    }

    iterator end() {
        matrix_view_data data(_data, _cols, _rows);
        iterator it(data, 0, 0);
        it._count = it._total;
        return it;
    }

};

ostream &operator << (ostream &s, matrix_view::point &p) {
    s << p.x << "," << p.y;
    return s;
}

void show(matrix_view &view) {

    for(int r=0; r < view._rows; r++) {
        for(int c=0; c < view._cols; c++) {
            cout << view.get(c, r) << " ";
        }
        cout << endl;
    }

}

void swap_iter(matrix_view::iterator &it1, matrix_view::iterator &it2) {

    int x = it1._p.x;
    int y = it1._p.y;

    it1._p.x = it2._p.x;
    it1._p.y = it2._p.y;

    it2._p.x = x;
    it2._p.y = y;

}

void rotate_view(matrix_view &view) {

    matrix_view::iterator first = view.begin();
    matrix_view::iterator n_first = view.begin();
    n_first++;
    matrix_view::iterator next = view.begin();
    next++;
    matrix_view::iterator last = view.end();

    while (first != next) {
        swap_iter(first++, next++);
        if (next == last) {
            next = n_first;
        } else if (first == n_first) {
            n_first = next;
        }
    }
    return;

}


int main() {

    vector<int> v;
    fill_v(v, 9);
    matrix_view view(v, 3, 3);

    cout << "Start" << endl;
//    matrix_view::iterator it=view.begin();
//    int count = 0;
//    for(; it != view.end(); it++) {
//        cout << *it << " ";
//        count++;
//        if( count % 3 == 0 )
//            cout << endl;
//    }
//    cout << endl;

    show(view);
    rotate_view(view);
    show(view);

    return 0;
}
