#ifndef HuffmanTree_
#define HuffmanTree_

#include <iostream>
#include <string>
#include <map>
using namespace std;

// Huffman Tree Node
class HuffmanNode {
public:
    char data;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;
    
    HuffmanNode(char d, int f) 
        : data(d), frequency(f), left(0), right(0) {}
    
    bool isLeaf() const {
        return (left == 0 && right == 0);
    }
};

// Simple Priority Queue for building Huffman Tree
class MinHeap {
private:
    HuffmanNode** heap;
    int capacity;
    int size;
    
    void heapifyDown(int index) {
        int smallest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        
        if (left < size && heap[left]->frequency < heap[smallest]->frequency)
            smallest = left;
        if (right < size && heap[right]->frequency < heap[smallest]->frequency)
            smallest = right;
        
        if (smallest != index) {
            HuffmanNode* temp = heap[index];
            heap[index] = heap[smallest];
            heap[smallest] = temp;
            heapifyDown(smallest);
        }
    }
    
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[parent]->frequency > heap[index]->frequency) {
                HuffmanNode* temp = heap[parent];
                heap[parent] = heap[index];
                heap[index] = temp;
                index = parent;
            } else {
                break;
            }
        }
    }
    
public:
    MinHeap(int cap) : capacity(cap), size(0) {
        heap = new HuffmanNode*[capacity];
    }
    
    ~MinHeap() {
        delete[] heap;
    }
    
    void insert(HuffmanNode* node) {
        if (size >= capacity) return;
        heap[size] = node;
        heapifyUp(size);
        size++;
    }
    
    HuffmanNode* extractMin() {
        if (size == 0) return 0;
        
        HuffmanNode* minNode = heap[0];
        heap[0] = heap[size - 1];
        size--;
        heapifyDown(0);
        
        return minNode;
    }
    
    int getSize() const { return size; }
};

// Huffman Compression System
class HuffmanTree {
private:
    HuffmanNode* root;
    map<char, string> codeTable;      // Character → Binary code
    map<char, int> frequencies;        // Character frequencies
    
    // Build frequency table
    void buildFrequencyTable(const string& text) {
        frequencies.clear();
        for (size_t i = 0; i < text.length(); i++) {
            frequencies[text[i]]++;
        }
    }
    
    // Build Huffman tree from frequencies
    void buildTree() {
        if (frequencies.empty()) {
            root = 0;
            return;
        }
        
        // Special case: single character
        if (frequencies.size() == 1) {
            map<char, int>::iterator it = frequencies.begin();
            root = new HuffmanNode(it->first, it->second);
            codeTable[it->first] = "0";
            return;
        }
        
        // Create min heap
        MinHeap minHeap(frequencies.size());
        
        // Insert all nodes into heap
        for (map<char, int>::iterator it = frequencies.begin(); 
             it != frequencies.end(); ++it) {
            HuffmanNode* node = new HuffmanNode(it->first, it->second);
            minHeap.insert(node);
        }
        
        // Build tree bottom-up
        while (minHeap.getSize() > 1) {
            HuffmanNode* left = minHeap.extractMin();
            HuffmanNode* right = minHeap.extractMin();
            
            HuffmanNode* parent = new HuffmanNode('\0', 
                left->frequency + right->frequency);
            parent->left = left;
            parent->right = right;
            
            minHeap.insert(parent);
        }
        
        root = minHeap.extractMin();
    }
    
    // Generate Huffman codes
    void generateCodes(HuffmanNode* node, string code) {
        if (!node) return;
        
        if (node->isLeaf()) {
            codeTable[node->data] = code.empty() ? "0" : code;
            return;
        }
        
        generateCodes(node->left, code + "0");
        generateCodes(node->right, code + "1");
    }
    
    // Clean up tree
    void destroyTree(HuffmanNode* node) {
        if (!node) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
    
    // Encode text to binary string
    string encodeToBinary(const string& text) {
        string encoded;
        for (size_t i = 0; i < text.length(); i++) {
            encoded += codeTable[text[i]];
        }
        return encoded;
    }
    
    // Decode binary string to text
    string decodeFromBinary(const string& binary) {
        string decoded;
        HuffmanNode* current = root;
        
        for (size_t i = 0; i < binary.length(); i++) {
            if (binary[i] == '0') {
                current = current->left;
            } else {
                current = current->right;
            }
            
            if (current->isLeaf()) {
                decoded += current->data;
                current = root;
            }
        }
        
        return decoded;
    }
    
    // Convert binary string to condensed format (for storage)
    string binaryToCompact(const string& binary) {
        // Store as-is for simplicity in C++98
        // In real implementation, would pack bits into bytes
        return binary;
    }
    
    string compactToBinary(const string& compact) {
        return compact;
    }
    
public:
    HuffmanTree() : root(0) {}
    
    ~HuffmanTree() {
        destroyTree(root);
    }
    
    // Main compress function
    string compress(const string& text) {
        if (text.empty()) return "";
        
        // Build frequency table
        buildFrequencyTable(text);
        
        // Build Huffman tree
        buildTree();
        
        // Generate codes
        codeTable.clear();
        generateCodes(root, "");
        
        // Encode to binary
        string binary = encodeToBinary(text);
        
        // Convert to compact format
        return binaryToCompact(binary);
    }
    
    // Main decompress function
    string decompress(const string& compressed) {
        if (compressed.empty() || !root) return "";
        
        // Convert from compact to binary
        string binary = compactToBinary(compressed);
        
        // Decode
        return decodeFromBinary(binary);
    }
    
    // Get compression statistics
    void getCompressionStats(const string& original, const string& compressed,
                            int& originalBits, int& compressedBits, 
                            double& ratio) {
        originalBits = original.length() * 8;  // ASCII = 8 bits per char
        compressedBits = compressed.length();  // Already in bits
        
        if (originalBits > 0) {
            ratio = (1.0 - (double)compressedBits / originalBits) * 100.0;
        } else {
            ratio = 0.0;
        }
    }
    
    // Display compression statistics
    void showCompressionStats(const string& original, const string& compressed) {
        int originalBits, compressedBits;
        double ratio;
        
        getCompressionStats(original, compressed, originalBits, 
                           compressedBits, ratio);
        
        cout << "\n========== HUFFMAN COMPRESSION STATS ==========" << endl;
        cout << "Original size:     " << original.length() 
             << " chars (" << originalBits << " bits)" << endl;
        cout << "Compressed size:   " << compressedBits << " bits" << endl;
        cout << "Compression ratio: " << (int)ratio << "%" << endl;
        cout << "Space saved:       " << (originalBits - compressedBits) 
             << " bits" << endl;
        cout << "===============================================\n" << endl;
    }
    
    // Display code table
    void displayCodeTable() {
        cout << "\nHuffman Code Table:" << endl;
        cout << "Char | Freq | Code" << endl;
        cout << "-----+------+----------" << endl;
        
        for (map<char, int>::iterator it = frequencies.begin(); 
             it != frequencies.end(); ++it) {
            char c = it->first;
            cout << "  ";
            if (c == ' ') cout << "SPC";
            else if (c == '\n') cout << "\\n";
            else cout << c;
            cout << "  | " << it->second << " | " << codeTable[c] << endl;
        }
    }
    
    // Get compression ratio
    double getCompressionRatio(const string& original, const string& compressed) {
        int originalBits = original.length() * 8;
        int compressedBits = compressed.length();
        
        if (originalBits == 0) return 0.0;
        return (1.0 - (double)compressedBits / originalBits) * 100.0;
    }
};

#endif
