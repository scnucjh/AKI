#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include <vector>
#include <algorithm>

class Variable {
public:
    int r,s,m;
    Variable(){}
    Variable(int r,int s,int m) : r(r), s(s), m(m) {}
};

bool deduced_set[7][3][128];

class Fun {
public:
    std::vector<Variable> left,right;
    Fun() {}
    Fun(std::vector<Variable> left,std::vector<Variable> right) :
        left(left), right(right) {}

    bool all_left_can_be_deduced() {
        for(const auto & x:left) {
            if(!deduced_set[x.r][x.s][x.m]) return false;
        }
        return true;
    }
    bool all_right_can_be_deduced() {
        for(const auto & x:right) {
            if(!deduced_set[x.r][x.s][x.m]) return false;
        }
        return true;
    }
    void set_all_right_be_deduced() {
        for(const auto &x : right) {
            deduced_set[x.r][x.s][x.m] = true;
        }
    }
};

#endif