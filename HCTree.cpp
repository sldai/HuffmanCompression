#include "HCTree.hpp"
#include <istream>
#include <sstream>
#include <math.h>
#include <bitset>
using namespace std;

void HCTree::build_from_cnt(map<char, unsigned int> char_cnt)
{
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;
    for (auto iter = char_cnt.begin(); iter != char_cnt.end(); iter++)
    {
        HCNode* tmp_node = new HCNode(iter->second, iter->first);
        tmp_node->is_leaf = true;
        this->leaves.push_back(tmp_node);
        pq.push(tmp_node);
    }

    while (pq.size() > 2)
    {
        HCNode* node0 = pq.top();
        pq.pop();
        HCNode* node1 = pq.top();
        pq.pop();
        HCNode* new_node = new HCNode(node0->count+node1->count, node0->symbol);
        new_node->c0 = node0;
        new_node->c1 = node1;
        node0->p = new_node;
        node1->p = new_node;
        pq.push(new_node);
    }
    if (pq.size() == 2)
    {
        HCNode* node0 = pq.top();
        pq.pop();
        HCNode* node1 = pq.top();
        pq.pop();
        HCNode* new_node = new HCNode(node0->count+node1->count, node0->symbol);
        new_node->c0 = node0;
        new_node->c1 = node1;
        node0->p = new_node;
        node1->p = new_node;
        this->root = new_node;
    }
    else if (pq.size() == 1)
    {
        HCNode* node0 = pq.top();
        pq.pop();
        HCNode* new_node = new HCNode(node0->count, node0->symbol);
        new_node->c0 = node0;
        node0->p = new_node;
        this->root = new_node;
    }
    else if (pq.size() == 0)
    {
        this->root = new HCNode(0, '@');
    }

    for (size_t i = 0; i < this->leaves.size(); i++)
    {
        char k = this->leaves[i]->symbol;
        int cnt = this->leaves[i]->count;
        string msg = "";
        auto cur = this->leaves[i];
        while (cur->p != nullptr)
        {
            if (cur == cur->p->c0)
            {
                msg = "0" + msg;
            }
            else if (cur == cur->p->c1)
            {
                msg = "1" + msg;
            }
            cur = cur->p;
        }
        this->encode_msg[k] = msg;
    }
    
}

string HCTree::print_encode_msg()
{
    string info = "";
    for (auto iter = this->encode_msg.begin(); iter != this->encode_msg.end(); iter++)
    {
        string k = string(1, iter->first);
        string code = iter->second;
        string tmp_msg = k + ": " + code + "\n";
        info += tmp_msg;
    }
    return info;
}

void HCTree::test_encode(my_byte symbol, ofstream& os)
{
    auto code = this->encode_msg[symbol];
    os << code;
}

void HCTree::encode(my_byte symbol, BitOutputStream& os)
{
    auto code = this->encode_msg[symbol];
    for (size_t i = 0; i < code.length(); i++)
    {
        if (code[i] == '0')
        {
            os.put_bit(0);
        }
        else if (code[i] == '1')
        {
            os.put_bit(1);
        }
    }
}

my_byte HCTree::test_decode(ifstream& is)
{
    auto cur = this->root;
    my_byte symbol = 0;
    char c;
    while (is.get(c))
    {
        if (c == '0')
        {
            cur = cur->c0;
        }
        else if (c == '1')
        {
            cur = cur->c1;
        }
        if (cur->is_leaf)
        {
            symbol = cur->symbol;
            break;
        }
    }
    return symbol;
}

my_byte HCTree::decode(BitInputStream& in)
{
    auto cur = this->root;
    my_byte symbol = 0;
    unsigned int c;
    while (in.get_bit(c))
    {
        if (c == 0)
        {
            cur = cur->c0;
        }
        else if (c == 1)
        {
            cur = cur->c1;
        }
        if (cur->is_leaf)
        {
            symbol = cur->symbol;
            break;
        }
    }
    return symbol;
}

void HCTree::write_head(BitOutputStream& os)
{
    int N = this->leaves.size();
    if (N == 0) return; // if the file is empty, do nothing
    os.put_byte((char)(N-1)); // convert 1~256 to 0~255
    for (size_t i = 0; i < N; i++)
    {
        my_byte symbol = this->leaves[i]->symbol;
        int cnt = this->leaves[i]->count;
        os.put_byte(symbol);
        unsigned char bytes[sizeof(int)];
        // cout << "Write " << symbol;
        for (size_t _i = 0; _i < sizeof(int); _i++)
        {
            bytes[_i] = (cnt >> (8 * (sizeof(int) - 1 - _i))) & 0xFF;
            os.put_byte(bytes[_i]);
            bitset<8> x(bytes[_i]);
            // cout << x;
        }
        
        // cout << endl;
    }
    int padding_num = this->calc_padding();
    os.put_byte((char)padding_num);
    cout << "Write " << padding_num << " paddings" << endl;
    this->write_padding(os, padding_num);
}

// write n padding bits to make bits fullfill bytes
void HCTree::write_padding(BitOutputStream& os, int num)
{
    for (size_t i = 0; i < num; i++)
    {
        os.put_bit(0);
    }
}

void HCTree::skip_padding(BitInputStream& is, int num)
{
    for (size_t i = 0; i < num; i++)
    {
        unsigned int c;
        if (!is.get_bit(c)) cout << "Padding reach out of file" << endl;
    }
}

int HCTree::calc_padding()
{
    int n_bits=0;
    for (int i = 0; i < this->leaves.size(); i++)
    {
        auto code = this->encode_msg[this->leaves[i]->symbol];
        n_bits += code.length() * this->leaves[i]->count;
    }
    return ceil(n_bits/8.0) * 8 - n_bits;
}

map<char, unsigned int> HCTree::read_head(BitInputStream& is)
{
    string err_msg = "Head corrupted";
    char c;
    bool valid;

    int N;
    const int cnt_size = sizeof(int);
    map<char, unsigned int> char_cnt;

    // Read N
    valid = is.get_byte(c);
    if (!valid) // the file is empty
    {
        cout << "Empty file" << endl;
        return char_cnt;
    }
    N = (my_byte) c + 1;
    cout << N << endl;
    // Read char and corresponding count
    for (size_t i = 0; i < N; i++)
    {
        my_byte symbol;
        int cnt=0;
        valid = is.get_byte(c);
        if (!valid)
        {
            cout << err_msg << endl;
            return char_cnt;
        }
        symbol = c;
        cout << "Read " << symbol;
        unsigned char bytes[cnt_size];
        for (size_t j = 0; j < cnt_size; j++)
        {
            is.get_byte(c);
            bytes[j] = c;
            cnt = cnt | (bytes[j] << ((cnt_size-1 - j) * 8));
            bitset<8> x(c);
            cout << x;
        }
        cout << endl;
        char_cnt[symbol] = cnt;
    }

    // Skip paddings
    is.get_byte(c);
    int paddings = (my_byte)c;
    this->skip_padding(is, paddings);
    return char_cnt;
}