#include <iostream>
#include <string>
#include <vector>
#include <queue>

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

int main()
{
    // Making Frequency Table

    string s = "Once upon a time, in a small village nestled beside a sparkling river, lived a young woman named Anya. Anya was known for her gentle spirit and kind heart, always ready to lend a helping hand to those in need. One day, a traveling merchant arrived in the village, his cart laden with exotic goods.";
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

    // Building Tree using Priority_queue(Minheap)

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

    // Pathmapping and Encoding

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

    // Binary Conversion

    vector<uint8_t> packedBytes;
    int i = 0;

    while (i + 8 <= encoding.size())
    {
        string chunk = encoding.substr(i, 8);
        uint8_t packedbit = stoi(chunk, nullptr, 2);
        packedBytes.push_back(packedbit);

        i += 8;
    }

    if (i < encoding.size())
    {
        string LastChunk = encoding.substr(i);

        while (LastChunk.size() < 8)
        {
            LastChunk += '0';
        }

        uint8_t packedbit = stoi(LastChunk, nullptr, 2);
        packedBytes.push_back(packedbit);
    }

    cout << "The String Size in bytes " << s.size() << endl;
    cout << "The Compressed Size in Bytes " << packedBytes.size() << endl;
    float saved = (1.0f - (float)packedBytes.size() / s.size()) * 100;
    cout << "The Compression Ratio " << (float)s.size() / packedBytes.size() << endl;
    cout << "Compressed By " << saved << "%" << endl;
}