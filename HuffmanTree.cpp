/*
	Author:		Mihir K Patel
	Purpose:	This is an implementation file for the HuffmanTree.hpp. It defines the member
				function and utility functions for the huffman tree using map, deque
				and heap priority queue.
*/
#include "HuffmanTree.hpp"

/*Destructor specifically deallocate the huffman tree after the decoding */
HuffmanTree::~HuffmanTree(){
	if(!huffT_prio_queue.empty()) {
		huffT_destructor_utility(huffT_prio_queue.min());
		delete huffT_prio_queue.min();
	}
}
/*compresses strings and stores to compressed string*/
std::string HuffmanTree::compress(const std::string inputStr) {
	/*Calls a function tht generates char frequencies and stores into map*/
	inputStr_char_freq(inputStr); 

	/*Calls a function that inserts characters with frequencies as priorty key in the 
		priority queue*/
	compressed_into_pqueue(char_freq_map); 

	/*Encode a new tree with a map*/
	huffT_str_encoding(huffT_prio_queue.min(), "");

	/*Append the string with characters of map*/
	for (char x : inputStr) { compressed_str += huffT_char_map[x]; }

	/*Traverse in postorder manner and get the specialize serialized string*/
	huffT_postOrder_trav(huffT_prio_queue.min());

	/*temprory free all minimum nodes of tree, this memory will be used again for deallocating*/
	deallocate_huffT_heap();

	return compressed_str;
}
/*Returns the serialized string*/
std::string HuffmanTree::serializeTree() const { return huffT_serialized_str; }

/*decompressed the string with input code and serialized tree */
std::string HuffmanTree::decompress(const std::string inputCode, const std::string serializedTree) {
	/**/
	huffT_decompressed_deque(serializedTree);

	/*Return the decoded string based 0 or 1 branches*/
	return huffT_str_decoding(inputCode);
}
/*removes the huffman tree from heap */
void HuffmanTree::deallocate_huffT_heap() {

	while (!huffT_prio_queue.empty()) {
		huffT_prio_queue.removeMin();
	}

}
/*Utility fuction to remove each nodes- traversing in post oder manner */
void HuffmanTree::huffT_destructor_utility(HuffmanNode* root){
	if(root->left != nullptr){
		huffT_destructor_utility(root->left);
	}
	else if (root->right != nullptr){
		huffT_destructor_utility(root->right);
	}
	if(root){
		delete root;
	}
}
/*In this function we encode the characters in the string using a map with their prefix codes.*/
void HuffmanTree::huffT_str_encoding(HuffmanNode* curr_root, std::string str_para) {
	if (curr_root != nullptr) {
		if(curr_root->getFrequency() == '0'){ }
		else if (curr_root->getFrequency() != '0') {/*gets the current char frequency, and put prefix code*/
			huffT_char_map.insert(std::pair<char, std::string>(curr_root->getCharacter(), str_para));
		}
		huffT_str_encoding(curr_root->right, str_para + "1");/*Put 1 in the right branch*/
		huffT_str_encoding(curr_root->left, str_para + "0");/*put 0 in the left branch*/
	}
	else {
		return;
	}
}
/*assignement specific, serialize a string using postorder, where leaves have L appened and 
	branches have B appended to the string*/
void HuffmanTree::huffT_postOrder_trav(HuffmanNode* root) {
	if (root == nullptr) {
		return;
	}
	else {
		huffT_postOrder_trav(root->left);
		huffT_postOrder_trav(root->right);

		if (root->isBranch()) {
			huffT_serialized_str += 'B';
		}
		else if (root->isLeaf()) {
			huffT_serialized_str += 'L';
			huffT_serialized_str += root->getCharacter(); /*Also appened the character of current node*/
		}
		else {}
	}
}
/*Build huffman table with character frequency. The legal characters are from space (32) till z (122)*/
void HuffmanTree::inputStr_char_freq(std::string inputStr) {
	if (inputStr == "") { /*Check is the string is passed, otherwise whole huffman coompression is no point*/
		throw ("inputStr_freq() failed.\n");
	}

	for (char x : inputStr) { ++char_freq[x - ' ']; }

	unsigned int x = 0;
	while (x < (128 - 37)) { /*Pair the ascii and its frequency */
		if (char_freq[x] == 0) {}
		else if (char_freq[x] != 0) {
			char_freq_map.insert(std::pair<char, int>(ascii_value, char_freq[x]));
		}
		++ascii_value;
		++x;
	}
}
/*Here we comparess the chracters using the huffman table with their each char's frequecny*/
void HuffmanTree::compressed_into_pqueue(std::map<char, int> map_para) {
	/*Insert the char/key and value/freuency into the priority queue*/
	for (auto& kv : map_para) {
		huffT_prio_queue.insert(new HuffmanNode(kv.first, kv.second));
	}
	/*Now sort the priority queue by lowest keys, then lexicographically*/
	for (; huffT_prio_queue.size() != 1;) {

		comp_left_node = huffT_prio_queue.min();
		huffT_prio_queue.removeMin();
		comp_right_node = huffT_prio_queue.min();
		huffT_prio_queue.removeMin();

		/*Put the last pair of node/root in the tree*/
		sum_char_freq = comp_left_node->getFrequency() + comp_right_node->getFrequency();
		comp_parent_node = new HuffmanNode('\0', sum_char_freq, nullptr, comp_left_node, comp_right_node);
		huffT_prio_queue.insert(comp_parent_node);
	}
}
/*Here is decompression of the serialized string using a new tree. We look for the specific leaves and 
	branch indicators, L and B and keep storing into a double sided queue for an easier access when we 
	build tree for the decompressed string.
*/
void HuffmanTree::huffT_decompressed_deque(std::string serializedTree) {
	unsigned int x = 1;
	while (x < serializedTree.length()) {

		curr_char = serializedTree.at(x);

		if (curr_char != '\\') {
			if ((serializedTree.at(x) == 'B' && serializedTree.at(x - 1) != 'L') ||
				(serializedTree.at(x) == 'B' && serializedTree.at(x - 1) == 'L' && serializedTree.at(x - 2) == 'L')) {
				/*Build a new tree with leaves branches and parent*/
				decomp_right_node = char_deque.back();
				char_deque.pop_back();
				decomp_left_node = char_deque.back();
				char_deque.pop_back();
				decomp_parent_node = new HuffmanNode('#', 1, nullptr, decomp_left_node, decomp_right_node);
				char_deque.push_back(decomp_parent_node);
			}/*Push a new node to attach the subtree above to keep the consistency in binary tree*/
			else if (serializedTree.at(x - 1) == 'L') {
				if (!(serializedTree.at(x) == 'B' && serializedTree.at(x - 1) == 'L' && serializedTree.at(x - 2) == 'L')) {
					char_deque.push_back(new HuffmanNode(serializedTree.at(x), 1));
				}
				else {}
			}
		}
		++x;
	}
}
/*We now decode the tree from the decocompression function aboved and appened node by node to a null string*/
std::string HuffmanTree::huffT_str_decoding(std::string inputCode) {
	huffT_leaf = char_deque.back();
	aux_huffT_leaf = huffT_leaf;

	for (char x : inputCode) {

		if (x == '1') {/*If has a branch*/
			aux_huffT_leaf = aux_huffT_leaf->right;
		}
		else if (x == '0') {/*if has a leaf*/
			aux_huffT_leaf = aux_huffT_leaf->left;
		}
		if (aux_huffT_leaf->isLeaf()) {/*Is it is the leaf just get the character and appened*/
			decompressed_str += aux_huffT_leaf->getCharacter();
			aux_huffT_leaf = huffT_leaf;
		}
	}
	return decompressed_str;
}