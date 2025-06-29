#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <tuple>

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

struct Compare2
{
    bool operator()(pair<char, int> a, pair<char, int> b) const
    {

        if (a.second != b.second)
        {
            return a.second < b.second;
        }
        return a.first < b.first;
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

void Encoding(string &s)
{
    vector<uint8_t> packedBytes;
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

    for(auto freqe : freq){
        cout<<freqe.first << " " << freqe.second << endl;
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

    vector<pair<char, int>> Canonical;

    for (auto codes : pathMap)
    {
        char first = codes.first;
        string second = codes.second;

        pair<char, int> par = {first, second.size()};

        Canonical.push_back(par);
    }

    sort(Canonical.begin(), Canonical.end(), Compare2());

    unordered_map<char, string> realCodes;
    int NextCode = 0;
    int currentLength = 0;

    for (const auto &pair : Canonical)
    {
        char ch = pair.first;
        int length = pair.second;

        if (length > currentLength)
        {
            NextCode <<= (length - currentLength);
            currentLength = length;
        }
        string code = bitset<32>(NextCode).to_string().substr(32 - length);
        realCodes[ch] = code;
        NextCode++;
    }

    string encoding;

    for (int i = 0; i < s.size(); i++)
    {
        if (realCodes.find(s[i]) != realCodes.end())
        {
            encoding += realCodes[s[i]];
        }
    }

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
    ofstream outfile("compressed.txt", ios::binary);

    uint16_t charsize = Canonical.size();
    outfile.write(reinterpret_cast<const char *>(&charsize), sizeof(charsize));

    for (const auto &pair : Canonical)
    {
        char ch = pair.first;
        uint8_t len = pair.second;

        outfile.write(reinterpret_cast<const char *>(&ch), sizeof(ch));
        outfile.write(reinterpret_cast<const char *>(&len), sizeof(len));
    }

    outfile.write(reinterpret_cast<const char *>(packedBytes.data()), packedBytes.size());
    outfile.close();
    cout << "file Compressed Successfully" << endl;
}

void Decompression()
{
    ifstream infile("compressed.txt", ios::binary);

    if (!infile)
    {
        cout << "Failed To open";
    }

    uint16_t charsize;
    infile.read(reinterpret_cast<char *>(&charsize), sizeof(charsize));

    vector<pair<char, uint8_t>> Canonical;

    for (int i = 0; i < charsize; i++)
    {
        char ch;
        uint8_t len;
        infile.read(reinterpret_cast<char *>(&ch), sizeof(ch));
        infile.read(reinterpret_cast<char *>(&len), sizeof(len));
        Canonical.push_back({ch, len});
    }

    vector<uint8_t> packedBytes;

    infile.seekg(0, ios::end);
    streampos fileSize = infile.tellg();
    streampos dataStart = 2 + charsize * 2;
    infile.seekg(dataStart, ios::beg);

    size_t dataSize = fileSize - dataStart;
    packedBytes.resize(dataSize);

    infile.read(reinterpret_cast<char *>(packedBytes.data()), dataSize);

    infile.close();

    unordered_map<char, string> realCodes;
    int NextCode = 0;
    int currentLength = 0;

    for (const auto &pair : Canonical)
    {
        char ch = pair.first;
        int length = pair.second;

        if (length > currentLength)
        {
            NextCode <<= (length - currentLength);
            currentLength = length;
        }
        string code = bitset<32>(NextCode).to_string().substr(32 - length);
        realCodes[ch] = code;
        NextCode++;
    }

    string recoding;

    for (int i = 0; i < packedBytes.size(); i++)
    {
        bitset<8> chunk(packedBytes[i]);
        recoding += chunk.to_string();
    }
    unordered_map<string, char> reverseCodesWithChar;

    for (auto pair : realCodes)
    {
        reverseCodesWithChar[pair.second] = pair.first;
    }

    string temp;
    string Decompressed;

    for (int i = 0; i < recoding.size(); i++)
    {
        temp += recoding[i];

        if (reverseCodesWithChar.find(temp) != reverseCodesWithChar.end())
        {
            Decompressed += reverseCodesWithChar[temp];
            temp = "";
        }
    }

    ofstream outfile("decompressed.txt");

    outfile.write(Decompressed.c_str(), Decompressed.size());
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

    Encoding(s);
    Decompression();
}