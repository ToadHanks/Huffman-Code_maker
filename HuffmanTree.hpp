/*
	Author:		Mihir K Patel
	Purpose:	Following is a header file for HuffmanTree.cpp, it has the data members, data function 
				utility functions that compress, decompress, and traverse the string from TestStrings.hpp.
*/
#ifndef HUFFMANTREE_HPP
#define HUFFMANTREE_HPP

#include <map>
#include <deque>
#include <climits>
#include <algorithm>
#include "HuffmanBase.hpp"
#include "HeapQueue.hpp"

class HuffmanTree : public HuffmanTreeBase {
private:
	/*map of each unique char in given strings*/
	std::map<char, std::string> huffT_char_map; 
	/*keeps frequencies in this priorities queue from map*/
	HeapQueue<HuffmanNode*, HuffmanNode::Compare> huffT_prio_queue;
	/*Stores the serialized string*/
	std::string huffT_serialized_str;

	/*91 unique chars in the string*/
	int char_freq[128-37] = { 0 };
	/*Keeps the frequences of each char in this map*/
	std::map<char, int> char_freq_map;
	char ascii_value = ' ';
	int sum_char_freq = 0; /*Sums the each frequencies*/
	
	std::string compressed_str = "";
	std::string decompressed_str = "";

	/*Deque is used to keep each node from huffman tree*/
	std::deque<HuffmanNode*> char_deque;
	HuffmanNode* huffT_leaf; /**/
	HuffmanNode* aux_huffT_leaf;

protected:
	/*Variables to track character in the priority queue*/
	char curr_char = ' ';
	/*These noeds are used for utility functions to make tree for
		compreesion and decompression of huffman codes/trees*/
	HuffmanNode* comp_left_node;
	HuffmanNode* comp_right_node;
	HuffmanNode* comp_parent_node;

	HuffmanNode* decomp_left_node;
	HuffmanNode* decomp_right_node;
	HuffmanNode* decomp_parent_node;

protected:
	void deallocate_huffT_heap(); /*remove the nodes from tree*/
	/*encodes the string, and builds the tree*/
	void huffT_str_encoding(HuffmanNode* curr_root, std::string str_para);
	/*decodes the string from deque*/
	std::string huffT_str_decoding(std::string inputCode);
	/*Traverse the huffman tree*/
	void huffT_postOrder_trav(HuffmanNode* root);
	/*Break the string down into characters*/
	void inputStr_char_freq(std::string inputStr);

	/*loads the map with char frequencies into the priority queue*/
	void compressed_into_pqueue(std::map<char,int> map_para);
	void huffT_decompressed_deque(std::string serializedTree);

public:
	HuffmanTree() {}
	/*Deallocation of a huffman tree */
	~HuffmanTree();
	void huffT_destructor_utility(HuffmanNode* root);
	/*Assingment related functions*/
	std::string compress(const std::string inputStr);
	std::string serializeTree() const;
	std::string decompress(const std::string inputCode, const std::string serializedTree);
};
#endif // !HUFFMANTREE_HPP