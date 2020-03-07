#include "Function.h"
#include "Fun_tool.h"
#include <iostream>
#include <cstring>

int path[7][128];

const int TOTAL_ROUNDS = 6;

void check(int ROUNDS) {
    int count=0;
    for(int r=0;r<ROUNDS;r++) {
        for(int i=0;i<128;i++) {
            if(path[r][i] && !deduced_set[r][0][i]) {
                // std::cout<<r<<" "<<i<<std::endl;
                count++;
            } 
        }
    }
    std::cout<<count<<" bits can't be deduced "<<std::endl;
}

int aki_r[128],aki_j[128];
int path_size,aki_size;

void read_data() {
    std::cin>>path_size;
    for(int i=0;i<path_size;i++) {
        int r,j;
        std::cin>>r>>j;
        path[r][j] = 1;
    }
    std::cin>>aki_size;
    for(int i=0;i<aki_size;i++) {
        std::cin>>aki_r[i]>>aki_j[i];
        deduced_set[aki_r[i]][0][aki_j[i]] = 1;
    }
}

void solver() {
    auto funs = gen_fun(TOTAL_ROUNDS);
    int cnt = 0;
    while(true) {
        bool update = false;
        for(auto &element:funs) {
            if(element.all_left_can_be_deduced()) {
                if(!element.all_right_can_be_deduced()) {
                    element.set_all_right_be_deduced();
                    update = true;
                    break;
                }
            }
        }
        if(!update) break;
    }
    check(TOTAL_ROUNDS);
}

int main() {
    read_data();
    solver();
    std::cout<<"try to remove one bit from AKI"<<std::endl;
    for(int ignore=0;ignore<aki_size;ignore++) {
        memset(deduced_set,0,sizeof(deduced_set));
        for(int i=0;i<aki_size;i++) {
            if(i==ignore) continue;
            deduced_set[aki_r[i]][0][aki_j[i]] = 1;
        }
        solver();
    }
    return 0;
}

