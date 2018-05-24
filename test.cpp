#include <iostream>
#include <vector>

int main() {
    std::vector<int> v{ 1, 2, 3, 4, 5 };
    v.insert(v.begin(), 6);
    std::cout << v[0] <<", "<< v[1] <<", "<< v[2] <<", "<< v[3] <<", "<< v[4] <<", "<< v[5] << std::endl;
    //6, 1, 2, 3, 4, 5
}
