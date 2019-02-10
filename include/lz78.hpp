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
// Created by Adrián on 09/02/2019.
//

#ifndef LZ78_LZ78_HPP
#define LZ78_LZ78_HPP

#include <lz78_trie.hpp>
#include <vector>
#include <iostream>

namespace lz78 {

    template <class t_value = char>
    class lz78 {

    public:
        typedef t_value value_type;
        typedef uint64_t size_type;

    private:

        std::vector<std::pair<size_type, value_type>> m_phrases;

        void copy(const lz78& p){
            m_phrases = p.m_phrases;
        }

        void decompress_phrase(const std::pair<size_type, value_type> &phrase,
                               const lz78_trie<value_type> &m_suffix_tree,
                               std::vector<value_type> &result){
            auto pos = phrase.first;
            if(pos > 0){
                decompress_phrase(m_phrases[pos-1], m_suffix_tree, result);
            }
            if(phrase.second > 0) result.push_back(phrase.second);

        }

    public:

        const std::vector<std::pair<size_type, value_type>> &phrases = m_phrases;

        template<class Iterator>
        lz78(const Iterator beg, const Iterator end){
            lz78_trie<value_type> m_lz78_trie;
            auto it = beg;
            while(it < end){
                auto pos_char = m_lz78_trie.search(it, end);
                m_lz78_trie.insert(pos_char.second, pos_char.first);
                m_phrases.push_back(pos_char);
            }
        }

        std::vector<value_type> decompress(){
            std::vector<value_type> result;
            lz78_trie<value_type> m_lz78_trie;
            for(const auto &phrase : m_phrases){
                decompress_phrase(phrase, m_lz78_trie, result);
                m_lz78_trie.insert(phrase.second, phrase.first);
            }
            return result;
        }

        //! Assignment move operation
        lz78& operator=(lz78&& p) {
            if (this != &p) {
                m_phrases = std::move(p.m_phrases);
            }
            return *this;
        }

        //! Assignment operator
        lz78& operator=(const lz78& tree)
        {
            if (this != &tree) {
                copy(tree);
            }
            return *this;
        }

        //! Copy constructor
        lz78(const lz78& p)
        {
            copy(p);
        }

        //! Move constructor
        lz78(lz78&& p)
        {
            *this = std::move(p);
        }

        //! Swap method
        /*! Swaps the content of the two data structure.
         *  You have to use set_vector to adjust the supported bit_vector.
         *  \param bp_support Object which is swapped.
         */
        void swap(lz78& p)
        {
            // m_bp.swap(bp_support.m_bp); use set_vector to set the supported bit_vector
            std::swap(m_phrases, p.m_phrases);
        }
    };
}

#endif //LZ78_LZ78_HPP
