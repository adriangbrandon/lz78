/***
BSD 2-Clause License

Copyright (c) 2018, Adrián
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/

#include <iostream>
#include <file_util.hpp>
#include <lz78.hpp>
//
// Created by Adrián on 10/02/2019.
//

int main(int argc, const char* argv[]) {

    if(argc == 2){

        std::string input = argv[1];
        auto size = util::file::file_size(input) / sizeof(uint64_t);
        std::vector<uint64_t > movements(size);
        std::ifstream in(input);
        in.read((char*) movements.data(), movements.size()*sizeof(uint64_t));
        in.close();

        lz78::lz78<uint64_t> m_lz78(movements.begin(), movements.end());
        std::cout << "Decompressing: " << std::flush;
        auto result = m_lz78.decompress();
        if(result.size() != movements.size()){
            std::cout << "Error size" << std::endl;
            exit(1);
        }
        for(uint64_t i = 0; i < result.size(); ++i){
            if(result[i] != movements[i]){
                std::cout << "Error movement: " << i << std::endl;
                exit(1);
            }
        }
        std::cout << "Phrases: " << m_lz78.phrases.size() << std::endl;

    }


}