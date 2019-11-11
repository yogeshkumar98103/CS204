#include <iostream>
#include "Library/HashTable.h"

class customHashModule: public Hash_Module<std::string>{
    std::hash<std::string> hasher;

  public:
    customHashModule():Hash_Module<std::string>(){}

    unsigned long int hash(const std::string& key) override {
        return hasher(key);
    }
};

bool checkPallindrome(std::string& str){
    int i = 0;
    int j = str.size() - 1;
    char c;
    while(i < j){
        if(str[i] != str[j]) return false;
        ++i;
        --j;
    }
    return true;
} 


int main(){
    // auto module = LinearProbingModule<std::string, 17>();
    auto module = customHashModule();
    HashTable<std::string, bool> hash_table(module);
    hash_table.set_default_value(false);

    int n;
    std::cin >> n;
    std::string str;
    bool found = false;
    for(int i = 0; i < n; i++){
        std::cin >> str;
        if(!found){
            if(!checkPallindrome(str)){
                if(hash_table[str]){
                    found = true;
                    continue;
                }
                std::reverse(str.begin(), str.end());
                hash_table[str] = true;
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
