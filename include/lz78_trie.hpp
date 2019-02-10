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

#ifndef LZ78_SUFFIX_TREE_HPP
#define LZ78_SUFFIX_TREE_HPP

#include <stdint.h>
#include <unordered_map>
#include <vector>

namespace lz78 {

    template <class t_value = char>
    class lz78_trie {

    public:
        typedef t_value value_type;
        typedef uint64_t size_type;
        typedef struct t_node{
            std::unordered_map<value_type, size_type> children;
        } node_type;

    private:
        std::vector<node_type> m_nodes;

        void copy(const lz78_trie& p){
            m_nodes = p.m_nodes;
        }

    public:

        const std::vector<node_type> &nodes = m_nodes;

        lz78_trie(){
            m_nodes.push_back(node_type{});
        };


        template<class Iterator>
        std::pair<size_type, value_type> search(Iterator &beg, const Iterator end){
            value_type new_value;
            size_type pos = 0;
            while(beg < end){
                new_value = *beg;
                if(m_nodes[pos].children.count(new_value) == 0){
                    ++beg;
                    return {pos, new_value};
                }
                pos = m_nodes[pos].children[new_value];
                ++beg;
            }
            return {pos,0};
        }

        void insert(const value_type value, const size_type pos){
            m_nodes[pos].children[value] = m_nodes.size();
            node_type new_node;
            m_nodes.push_back(new_node);
        }

        //! Assignment move operation
        lz78_trie& operator=(lz78_trie&& p) {
            if (this != &p) {
                m_nodes = std::move(p.m_nodes);
            }
            return *this;
        }

        //! Assignment operator
        lz78_trie& operator=(const lz78_trie& tree)
        {
            if (this != &tree) {
                copy(tree);
            }
            return *this;
        }

        //! Copy constructor
        lz78_trie(const lz78_trie& p)
        {
            copy(p);
        }

        //! Move constructor
        lz78_trie(lz78_trie&& p)
        {
            *this = std::move(p);
        }

        //! Swap method
        /*! Swaps the content of the two data structure.
         *  You have to use set_vector to adjust the supported bit_vector.
         *  \param bp_support Object which is swapped.
         */
        void swap(lz78_trie& p)
        {
            // m_bp.swap(bp_support.m_bp); use set_vector to set the supported bit_vector
            std::swap(m_nodes, p.m_nodes);
        }



    };
}

#endif //LZ78_SUFFIX_TREE_HPP
