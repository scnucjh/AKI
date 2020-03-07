#ifndef _FUN_TOOL_H
#define _FUN_TOOL_H
#include "Function.h"
#include <iostream>

int id(int x,int y) {
    x = (x+4) % 4;
    y = (y+32) % 32;
    return x * 32 + y;
}

std::vector<Fun> gen_fun(int ROUNDS) {
    std::vector<Fun> res;
    for(int r=0;r<ROUNDS;r++) {
        for(int i=0;i<=3;i++) {
            for(int j=8;j<32;j++) {
                std::vector<Variable> left,right;

                left.emplace_back(r,0,id(i,j));

                right.emplace_back(r,1,id(i,j));

                res.emplace_back(left,right);
                res.emplace_back(right,left);
            }
        }
        for(int j=0;j<8;j++) {
            std::vector<Variable> left,right;

            left.emplace_back(r,0,id(0,j));
            left.emplace_back(r,0,id(1,j));
            left.emplace_back(r,0,id(2,j));
            left.emplace_back(r,0,id(3,j));

            right.emplace_back(r,1,id(0,j));
            right.emplace_back(r,1,id(1,j));
            right.emplace_back(r,1,id(2,j));
            right.emplace_back(r,1,id(3,j));

            res.emplace_back(left,right);
            res.emplace_back(right,left);
        }
        for(int j=0;j<32;j++) {
            std::vector<Variable> left,right;
            left = { Variable(r,1,id(0,j-8)), Variable(r,1,id(1,j)) };
            right = {Variable(r,2,id(0,j))};
            
            res.emplace_back(left,right);

            left = {Variable(r,1,(0,j-8)),Variable(r,2,id(0,j)) };
            right = { Variable(r,1,id(1,j)) };
            res.emplace_back(left,right);

            left = { Variable(r,1,id(1,j)), Variable(r,2,id(0,j)) };
            right = { Variable(r,1,id(0,j-8)) };
            res.emplace_back(left,right);

            // ------------------------------------ //
            left = { Variable(r,1,id(2,j)) };
            right = { Variable(r,2,id(1,j)) };
            res.emplace_back(left,right);
            res.emplace_back(right,left);

            // ------------------------------------ //
            left = {Variable(r,1,id(2,j-16)),Variable(r,1,id(3,j))};
            right = {Variable(r,2,id(2,j))};
            res.emplace_back(left,right);

            left = {Variable(r,1,id(2,j-16)),Variable(r,2,id(2,j))};
            right = {Variable(r,1,id(3,j))};
            res.emplace_back(left,right);

            left = {Variable(r,2,id(2,j)),Variable(r,1,id(3,j))};
            right = {Variable(r,1,id(2,j-16))};
            res.emplace_back(left,right);

            // ------------------------------------ //
            left = {Variable(r,1,id(0,j))};
            right = {Variable(r,2,id(3,j))};
            res.emplace_back(left,right);
            res.emplace_back(right,left);

        }
        for(int i=0;i<4;i++) {
            for(int j=0;j<32;j++) {
                std::vector<Variable> left = {Variable(r,2,id(i,j))};
                std::vector<Variable> right ={Variable(r+1,0,id(i,j))};
                res.emplace_back(left,right);
                res.emplace_back(right,left);
            }
        }
    }
    return res;
}



#endif