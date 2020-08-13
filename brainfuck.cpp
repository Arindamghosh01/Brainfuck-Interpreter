/**
 *		author :  arindam
 *  	created : 12-08-2020 00:03:48
**/

#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <stdexcept>
#include <limits>
#include <sstream>
#include <iterator>

#define MEMSIZE 65536

std::vector <char> input_src (const std::string& filepath) {
    std::ifstream in(filepath);
    
    //File does not exist
    if(in.fail()){
        throw std::invalid_argument("File Path specified doesn't exists. Enter a valid path.");
    }

    std::vector <char> src;

    int Brace_cnt = 0;
    char c;
    while(in.get(c)) {
        if (c == ']') --Brace_cnt;
        if (c == '[') ++Brace_cnt;
        if (Brace_cnt < 0) 
            throw std::runtime_error("Check for matching braces.");
        src.push_back(c);
    }  

    if (Brace_cnt != 0)
        throw std::runtime_error("Check for Braces");    

    std::vector<char>::iterator it;
    it = std::remove_if(src.begin(), src.end(), 
            [](char c) { return !std::strchr("+-<>.,[]", c); });

    src.erase(it, src.end());

    return src;
}

void interpret(const std::vector <char>& s) {
    std::vector <unsigned char> mem(MEMSIZE);

    int p = 0;  // pointer to memory cell
    int cnt = 0;  // loop counter
   
    std::size_t len = s.size();

    for (std::size_t i = 0; i < len; i++) {
        // > moves the pointer to the right
        if (s[i] == '>') {
            if (p == MEMSIZE-1) 
                throw std::out_of_range("Out of memory block... Maybe your loop ran for too long, at index:" + std::to_string(p+1));
            ++p;
        }
        
        // < moves the pointer to the left
        else if (s[i] == '<') {
            if (p == 0)
                throw std::out_of_range("Invalid memory Access, at index:"
                    + std::to_string(p-1));
            --p;
        }

        // + increment the value at current cell
        else if (s[i] == '+') ++mem[p];
 
        // - decrement the value at current cell
        else if (s[i] == '-') --mem[p];

        // print the current cell char 
        else if (s[i] == '.') 
            std::cout << mem[p] << std::flush;

        // input char in current cell
        else if (s[i] == ',') 
            std::cin >> std::noskipws >> mem[p];

        // [ jumps past the matching ] if the cell  
        // under the pointer is 0 
        else if (s[i] == '[') { 
            if (mem[p] == 0) { 
                ++cnt;
                while (cnt > 0 || s[i] != ']') { 
                    ++i;
                    if (s[i] == '[') 
                        ++cnt; 
                    else if (s[i] == ']') 
                        --cnt; 
                } 
            } 
        } 

        // ] jumps back to the matching [ if the 
        // cell under the pointer is !0 
        else if (s[i] == ']') {
            if (mem[p] != 0) { 
                ++cnt; 
                while (cnt > 0 || s[i] != '[') { 
                    --i;
                    if (s[i] == ']') 
                        ++cnt; 
                    else if (s[i] == '[') 
                        --cnt; 
                }
            } 
        }
    }
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Expected path to Brainfuck source file as a command line argument" << std::endl;
        return 1;
    } 
    
    std::vector <char> src = input_src(argv[1]);
    interpret(src);
}