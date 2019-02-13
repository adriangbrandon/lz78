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


//
// Created by Adrián on 12/02/2019.
//

#ifndef LZ_LZ77_HPP
#define LZ_LZ77_HPP

#include <lz77_match.hpp>
#include <vector>

namespace lz {

    template <class t_value = char, uint64_t short_block_size = 2, const uint64_t size_window = 0>
    class lz77 {

    public:
        typedef t_value value_type;
        typedef uint64_t size_type;

    private:

        std::vector<std::pair<size_type, size_type>> m_phrases;

        template<class Iterator>
        std::pair<size_type, size_type> get_factor_search(
                const Iterator &beg, const Iterator &start_data,
                const Iterator &end_data, const Iterator &beg_window){

            auto it_window = beg_window;
            std::pair<size_type, size_type> res((size_type) *beg, 0);
            while(it_window < beg){ //Go through the window
                auto it_data = beg, aux_it_window = it_window;
                //1. Check if the values in the window and the buffer are equals.
                // The buffer is limited by the short_block_size and the last position
                while(it_data != end_data && std::distance(beg, it_data) < short_block_size
                      && *it_data == *aux_it_window){
                    ++it_data;
                    ++aux_it_window;
                }
                auto length = std::distance(beg, it_data);
                //2. If the result is better we update it
                if(res.second < length){
                    res = {std::distance(start_data, it_window), length};
                }
                //3. We cannot improve the result when we reach the last position or the length is equal to short_block_size-1.
                // So we stop the search
                if(it_data == end_data || length = short_block_size-1) return res;
                ++it_window;
            }
            return res;
        }

        template<class Iterator>
        std::pair<size_type, size_type> get_factor_from_iterator(
                const Iterator &beg, const Iterator &start_data,
                const Iterator &end_data, const Iterator &it_window){

            auto it_data = beg, aux_it_window = it_window;
            //1. Check if the values in the window and the buffer are equals.
            // The buffer is limited by the short_block_size and the last position
            while(it_data != end_data && std::distance(beg, it_data) < short_block_size
                  && *it_data == *aux_it_window){
                ++it_data;
                ++aux_it_window;
            }
            auto length = std::distance(beg, it_data);
            //2. If the result is better we update it
            return  {std::distance(start_data, it_window), length};
        }

    public:

        template<class Iterator>
        lz77(const Iterator beg, const Iterator end){
            lz77_match<value_type, short_block_size> m_lz77_match(beg, end);
            auto it = beg, beg_window = beg;
            size_type pos = 0;
            while(it < end){
                if(pos == 0) {
                    m_phrases.push_back({(size_type) (*it), 0});
                    ++pos; ++it;
                }else{
                    if(std::distance(beg_window, it) < short_block_size){
                        auto pos_size = get_factor_search(it, beg, end, beg_window);
                        pos += pos_size.second;
                        it += pos_size.second;
                        m_phrases.push_back(pos_size);
                    }else{
                        auto matches = m_lz77_match.find(beg);
                        size_type i = 0, j = matches.size()-1;
                        //todo: adjust window size
                        if(i > j){
                            auto pos_size = get_factor_search(it, beg, end, beg_window);
                            pos += pos_size.second;
                            it += pos_size.second;
                            m_phrases.push_back(pos_size);
                        }else{
                            std::pair<size_type, size_type> res(matches[i], short_block_size);
                            for(size_type k = i; k <= j; ++k){
                                auto pos_size = get_factor_from_iterator(it, beg, end, beg + matches[k]);
                                if(pos_size){

                                }
                                //todo: search from position matches[k]
                            }
                        }
                    }
                }

            }
        }


    };
}
#endif //LZ_LZ77_HPP
