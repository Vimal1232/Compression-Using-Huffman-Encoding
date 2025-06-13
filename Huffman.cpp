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

void Decomp(Node *head, string encoding, int ptr, string &decompressed, Node *root)
{
    if (head == nullptr)
    {
        return;
    }

    if (head->ch != '\0')
    {
        char c = head->ch;
        decompressed += c;
        Decomp(root, encoding, ptr, decompressed, root);
    }

    if (encoding[ptr] == '0')
    {
        Decomp(head->left, encoding, ptr + 1, decompressed, root);
    }

    if (encoding[ptr] == '1')
    {
        Decomp(head->right, encoding, ptr + 1, decompressed, root);
    }
}

int main()
{
    // Making Frequency Table
    string s = "One day, a mighty lion was sleeping in the jungle when a little mouse accidentally ran across his paw. The lion woke up and caught the mouse, ready to eat it. The mouse pleaded, “Please, let me go, and someday I’ll repay your kindness.” The lion laughed but decided to let the mouse go. A few days later, the lion got caught in a hunter’s net. The mouse heard his roar, ran to him, and chewed through the ropes to free him. The lion was grateful and realized even small creatures can make a big difference.";

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

    // Decompression

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

    string decompressed = "";
    int stringpointer;

    Decomp(root, Recoding, stringpointer, decompressed, root);

    cout << "The String Size in bytes " << s.size() << endl;
    cout << "The Compressed Size in Bytes " << packedBytes.size() << endl;
    float saved = (1.0f - (float)packedBytes.size() / s.size()) * 100;
    cout << "The Compression Ratio " << (float)s.size() / packedBytes.size() << endl;
    cout << "Compressed By " << saved << "%" << endl;
    cout << "Decompressed Value " << decompressed << endl;
}