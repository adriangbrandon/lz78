/***
BSD 2-Clause License

Linear Time Lempel-Ziv Factorization
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


//
// Created by Adrián on 12/02/2019.
//

#ifndef LZ_LZ77_KKP3_HPP
#define LZ_LZ77_KKP3_HPP

#include <stdint.h>
#include <vector>
#include <sdsl/csa_bitcompressed.hpp>

#define STACK_BITS 16
#define STACK_SIZE (1 << STACK_BITS)
#define STACK_HALF (1 << (STACK_BITS - 1))
#define STACK_MASK ((STACK_SIZE) - 1)

namespace lz {

    template <class t_value = char>
    class lz77_kkp2 {

    public:
        typedef t_value value_type;
        typedef int64_t size_type;

    private:

        std::vector<std::pair<size_type, size_type>> m_phrases;




        template<class Container>
        size_type parse_phrase(const Container &X, const uint64_t n, const size_type i, const size_type psv, const size_type nsv) {
            size_type pos, len = 0;
            if (nsv == -1) {
                while (X[psv + len] == X[i + len]) ++len;
                pos = psv;
            } else if (psv == -1) {
                while (i + len < n && X[nsv + len] == X[i + len]) ++len;
                pos = nsv;
            } else {
                while (X[psv + len] == X[nsv + len]) ++len;
                if (X[i + len] == X[psv + len]) {
                    ++len;
                    while (X[i + len] == X[psv + len]) ++len;
                    pos = psv;
                } else {
                    while (i + len < n && X[i + len] == X[nsv + len]) ++len;
                    pos = nsv;
                }
            }
            if (len == 0) pos = X[i];
            m_phrases.push_back({pos, len});
            return i + std::max(1LL, len);
        }

    public:

        const std::vector<std::pair<size_type, size_type>> &phrases = m_phrases;

        template<class Container>
        lz77_kkp2(const Container &X){
            std::vector<size_type> SA;
            auto n = X.size();
            if (n == 0) return;
            {
                sdsl::int_vector<> movs;
                movs.resize(n);
                std::copy(X.begin(), X.end(), movs.begin());
                sdsl::csa_bitcompressed<sdsl::int_alphabet<>> csa;
                sdsl::construct_im(csa, movs);
                SA.resize(n+2);
                for(size_type i = 0; i < csa.size(); ++i){
                    SA[i] = csa[i];
                }
            }

            std::cout << "SA Done" << std::endl;
            size_type *CS = new size_type[n + 5];
            size_type *stack = new size_type[STACK_SIZE + 5], top = 0;
            stack[top] = 0;

            // Compute PSV_text for SA and save into CS.
            CS[0] = -1;
            for (size_type i = 1; i <= n; ++i) {
                size_type sai = SA[i - 1] + 1;
                while (stack[top] > sai) --top;
                if ((top & STACK_MASK) == 0) {
                    if (stack[top] < 0) {
                        // Stack empty -- use implicit.
                        top = -stack[top];
                        while (top > sai) top = CS[top];
                        stack[0] = -CS[top];
                        stack[1] = top;
                        top = 1;
                    } else if (top == STACK_SIZE) {
                        // Stack is full -- discard half.
                        for (size_type j = STACK_HALF; j <= STACK_SIZE; ++j)
                            stack[j - STACK_HALF] = stack[j];
                        stack[0] = -stack[0];
                        top = STACK_HALF;
                    }
                }

                size_type addr = sai;
                CS[addr] = std::max(0LL, stack[top]);
                ++top;
                stack[top] = sai;
            }
            delete[] stack;

            // Compute the phrases.
            CS[0] = 0;
            size_type nfactors = 0, next = 1, nsv, psv;
            for (size_type t = 1; t <= n; ++t) {
                psv = CS[t];
                nsv = CS[psv];
                if (t == next) {
                    next = parse_phrase(X, n, t - 1, psv - 1, nsv - 1) + 1;
                    ++nfactors;
                }
                CS[t] = nsv;
                CS[psv] = t;
            }

            // Clean up.
            delete[] CS;
        }


        std::vector<value_type> decompress(){
            std::vector<value_type> result;
            for(const auto &phrase : m_phrases){
                if(phrase.second == 0){
                    result.push_back((value_type) phrase.first);
                }else{
                    for(size_type i = phrase.first; i < phrase.first + phrase.second; ++i){
                        result.push_back(result[i]);
                    }
                }

            }
            return result;
        }




    };
}


#endif //LZ78_LZ77_KKP3_HPP
