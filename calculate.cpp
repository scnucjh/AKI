#include "Function.h"
#include "Fun_tool.h"
#include <iostream>
#include <cstring>
#include <set>
#include <utility>

int IN_PATH[7][128];

const int TOTAL_ROUNDS = 6;

int aki_r[128],aki_j[128];
int path_size;

void read_data() {
    std::cin>>path_size;
    for(int i=0;i<path_size;i++) {
        int r,j;
        std::cin>>r>>j;
        IN_PATH[r][j] = 1;
    }
}


auto funs = gen_fun(TOTAL_ROUNDS);
std::set< std::pair<int,int> > aki_set;

void DEDUCE() {
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
}



int main() {
    read_data();
    for(int r = 0; r<TOTAL_ROUNDS;) {
        bool update = 0;
        DEDUCE();
        for(int j=0;j<128;j++) {
            if(IN_PATH[r][j] && !deduced_set[r][0][j]) {
                aki_set.insert( std::make_pair(r,j) );
                deduced_set[r][0][j] = 1;
                update = 1;
                break;
            }
        }
        if(!update) r++;
    }
    std::cout<< " The AKI size is : " << aki_set.size() << std::endl;
    return 0;
}

