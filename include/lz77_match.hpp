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


#ifndef LZ_LZ77_MATCH_HPP
#define LZ_LZ77_MATCH_HPP

#include <stdint.h>
#include <unordered_map>
#include <lz77_match.hpp>

namespace lz {

    template <class t_value = char, uint64_t short_block_size = 2>
    class lz77_match {

    public:
        typedef t_value value_type;
        typedef uint64_t size_type;
        typedef struct short_match_hash
        {
            std::size_t operator()(const std::vector<value_type>& k) const
            {
                std::size_t hash = 0;
                for (auto i = k.begin(); i != k.end(); ++i)
                {
                    value_type val = *i;
                    hash ^= val + 0x9e3779b9 + (hash<<6) + (hash>>2);
                }
                return hash;
            }
        } short_match_hash_type;
        typedef std::unordered_map<std::vector<value_type>, std::vector<size_type>, short_match_hash_type> short_match_type;


    private:

        short_match_type m_short_match;

        void copy(const lz77_match& p){
            m_short_match = p.m_short_match;
        }

    public:

        template <class Iterator>
        lz77_match(const Iterator &beg, const Iterator &end){
            size_type i = 0;
            for(Iterator it = beg; it <= end - short_block_size; ++it){
                std::vector<value_type> collection(it, it+short_block_size);
                m_short_match[collection].push_back(i);
                ++i;
            }
        }

        template <class Iterator>
        std::vector<size_type> find(const Iterator &beg){
            std::vector<value_type> collection(beg, beg+short_block_size);
            if(m_short_match.count(collection) == 0) return std::vector<value_type>();
            return m_short_match[collection];
        }

        void print(){
            std::vector<std::pair<std::vector<value_type>, std::vector<size_type>>> data(m_short_match.begin(), m_short_match.end());
            for(const auto &v : data){
                std::cout << "key: ";
                for(const auto &k : v.first){
                    std::cout << k << ", ";
                }
                std::cout << " pos: ";
                for(const auto &k : v.second){
                    std::cout << k << ", ";
                }
                std::cout << std::endl;
            }

        }

        //! Assignment move operation
        lz77_match& operator=(lz77_match&& p) {
            if (this != &p) {
                m_short_match = std::move(p.m_short_match);
            }
            return *this;
        }

        //! Assignment operator
        lz77_match& operator=(const lz77_match& p)
        {
            if (this != &p) {
                copy(p);
            }
            return *this;
        }

        //! Copy constructor
        lz77_match(const lz77_match& p)
        {
            copy(p);
        }

        //! Move constructor
        lz77_match(lz77_match&& p)
        {
            *this = std::move(p);
        }

        //! Swap method
        /*! Swaps the content of the two data structure.
         *  You have to use set_vector to adjust the supported bit_vector.
         *  \param bp_support Object which is swapped.
         */
        void swap(lz77_match& p)
        {
            // m_bp.swap(bp_support.m_bp); use set_vector to set the supported bit_vector
            std::swap(m_short_match, p.m_short_match);
        }


    };
}

#endif //LZ78_LZ77_MATCH_HPP
