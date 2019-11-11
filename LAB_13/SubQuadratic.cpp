#include <iostream>
#include <algorithm>

#include <set>

bool checkPallindrome(std::string& str){
    int i = 0;
    int j = str.size() - 1;
    while(i < j){
        if(str[i] != str[j]) return false;
        ++i;
        --j;
    }
    return true;
} 

int main(){
    std::set<std::string> s;
    int n;
    std::cin >> n;
    std::string str;
    bool found = false;
    for(int i = 0; i < n; i++){
        std::cin >> str;
        if(!found){
            if(!checkPallindrome(str)){
                if(s.find(std::move(str)) != s.end()){
                    found = true;
                    continue;
                }
                std::reverse(str.begin(), str.end());
                s.insert(std::move(str));
            }
        }
    }
    if(found){
        std::cout << "YES" << std::endl;
    }
    else{
        std::cout << "NO" << std::endl;
    }

    return 0;
}