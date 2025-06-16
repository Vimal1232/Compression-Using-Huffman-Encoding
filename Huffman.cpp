#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>

using namespace std;

class Node
{
public:
    char ch;
    int freq;
    Node *left;
    Node *right;

    Node(char c, int f)
    {
        this->ch = c;
        this->freq = f;
        this->left = nullptr;
        this->right = nullptr;
    }

    Node(char c, int f, Node *l, Node *r)
    {
        this->ch = c;
        this->freq = f;
        this->left = l;
        this->right = r;
    }
};
struct Compare
{
    bool operator()(Node *a, Node *b) const
    {

        if (a->freq > b->freq)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

void paths(Node *start, string Path, unordered_map<char, string> &pathMap)
{
    if (start == nullptr)
    {
        return;
    }

    if (start->ch != '\0')
    {
        char c = start->ch;
        pathMap[c] = Path;
    }

    paths(start->left, Path + '0', pathMap);
    paths(start->right, Path + '1', pathMap);
}


pair<string, Node *> Encoding(string s)
{
    unordered_map<char, int> freq;

    for (int i = 0; i < s.size(); i++)
    {
        if (freq.find(s[i]) != freq.end())
        {
            freq[s[i]] += 1;
        }
        else
        {
            freq[s[i]] = 1;
        }
    }

    priority_queue<Node *, vector<Node *>, Compare> minHeap;

    for (auto i : freq)
    {
        Node *n = new Node(i.first, i.second);
        minHeap.push(n);
    }
    Node *root;

    while (!minHeap.empty())
    {
        if (minHeap.size() == 1)
        {
            root = minHeap.top();
            minHeap.pop();
            break;
        }

        Node *left = minHeap.top();
        minHeap.pop();
        Node *right = minHeap.top();
        minHeap.pop();
        int Combination = left->freq + right->freq;
        char dummy = '\0';
        Node *n = new Node(dummy, Combination, left, right);
        minHeap.push(n);
    }

    unordered_map<char, string> pathMap;
    string code;

    paths(root, code, pathMap);

    string encoding;

    for (int i = 0; i < s.size(); i++)
    {
        if (pathMap.find(s[i]) != pathMap.end())
        {
            encoding += pathMap[s[i]];
        }
    }
    return {encoding, root};
}

int BinaryStoring(string encoding, vector<uint8_t> &packedBytes)
{
    int i = 0;

    while (i + 8 <= encoding.size())
    {
        string chunk = encoding.substr(i, 8);
        uint8_t packedbit = stoi(chunk, nullptr, 2);
        packedBytes.push_back(packedbit);

        i += 8;
    }
    int ExtraBits = 0;

    if (i < encoding.size())
    {
        string LastChunk = encoding.substr(i);

        while (LastChunk.size() < 8)
        {
            LastChunk += '0';
            ExtraBits++;
        }

        uint8_t packedbit = stoi(LastChunk, nullptr, 2);
        packedBytes.push_back(packedbit);
    }

    return ExtraBits;
}

void Decompression(string &decompressed, int ExtraBits, vector<uint8_t> &packedBytes, Node *root)
{
    string Recoding;

    for (int i = 0; i < packedBytes.size(); i++)
    {
        bitset<8> chunk(packedBytes[i]);

        Recoding += chunk.to_string();
    }

    if (ExtraBits != 0)
    {
        Recoding.erase(Recoding.size() - ExtraBits, ExtraBits);
    }
    Node *current = root;

    for (char bit : Recoding)
    {
        if (bit == '0')
        {
            current = current->left;
        }
        if (bit == '1')
        {
            current = current->right;
        }

        if (!current)
        {
            cerr << "Traversal error: null node reached!" << endl;
            return;
        }

        if(current ->ch != '\0'){
            decompressed += current->ch;
            current = root;
        }
    }
}

int main()
{
    ifstream file("Test.txt");
    if (!file.is_open())
    {
        cerr << "Can't Open File" << endl;
        return 0;
    }
    stringstream buffer;
    buffer << file.rdbuf();
    string s = buffer.str();

    file.close();

    pair<string, Node *> result = Encoding(s);
    string encoding = result.first;
    Node *root = result.second;

    vector<uint8_t> packedBytes;

    int ExtraBits = BinaryStoring(encoding, packedBytes);

    string decompressed = "";

    Decompression(decompressed, ExtraBits, packedBytes, root);

    cout << "The String Size in bytes " << s.size() << endl;
    cout << "The Compressed Size in Bytes " << packedBytes.size() << endl;
    float saved = (1.0f - (float)packedBytes.size() / s.size()) * 100;
    cout << "The Compression Ratio " << (float)s.size() / packedBytes.size() << endl;
    cout << "Compressed By " << saved << "%" << endl;
    // cout << "Decompressed Value " << decompressed << endl;
}