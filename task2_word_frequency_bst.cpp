// CS250 Lab 9 - Task 2: Word Frequency Indexing using BST
// Name: Amna Shafiq | CMS-ID: 552522 | BSDS 3A
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
using namespace std;

struct WNode {
    string word;
    int freq;
    WNode *left, *right;
    WNode(const string& w) : word(w), freq(1), left(nullptr), right(nullptr) {}
};

class WordBST {
    WNode* root = nullptr;
    int totalWords = 0, uniqueWords = 0;

    WNode* insert(WNode* r, const string& w) {
        if (!r) { uniqueWords++; return new WNode(w); }
        if (w < r->word) r->left = insert(r->left, w);
        else if (w > r->word) r->right = insert(r->right, w);
        else r->freq++; // word exists -> increment frequency
        return r;
    }
    void inorder(WNode* r) const {
        if (!r) return;
        inorder(r->left);
        cout << "  " << r->word << " -> " << r->freq << "\n";
        inorder(r->right);
    }
    void collect(WNode* r, vector<WNode*>& v) const {
        if (!r) return;
        collect(r->left, v); v.push_back(r); collect(r->right, v);
    }
public:
    void insertWord(const string& w) { root = insert(root, w); totalWords++; }

    // Parse paragraph: lowercase letters only, split on anything else
    void parse(const string& text) {
        string cur;
        for (char ch : text) {
            if (isalpha((unsigned char)ch)) cur += tolower((unsigned char)ch);
            else if (!cur.empty()) { insertWord(cur); cur.clear(); }
        }
        if (!cur.empty()) insertWord(cur);
    }
    void displayAlphabetical() const {
        cout << "\n--- Words in Alphabetical Order ---\n";
        inorder(root);
    }
    void displayByFrequency() const {
        vector<WNode*> v; collect(root, v);
        stable_sort(v.begin(), v.end(),
                    [](WNode* a, WNode* b) { return a->freq > b->freq; });
        cout << "\n--- Words by Frequency (High -> Low) ---\n";
        for (auto n : v) cout << "  " << n->word << " -> " << n->freq << "\n";
    }
    void findMostFrequentWord() const {
        vector<WNode*> v; collect(root, v);
        int mx = 0;
        for (auto n : v) mx = max(mx, n->freq);
        cout << "\n--- Most Frequent Word(s) ---\n";
        for (auto n : v) if (n->freq == mx) cout << "  " << n->word << " (" << mx << " times)\n";
    }
    void summary() const {
        cout << "\n--- Summary ---\n";
        cout << "  Total unique words : " << uniqueWords << "\n";
        cout << "  Total word count   : " << totalWords << "\n";
    }
};

int main() {
    string para = "The quick brown fox jumps over the lazy dog. The fox is quick and smart.";
    cout << "===== TASK 2: Word Frequency Indexing using BST =====\n";
    cout << "Paragraph: \"" << para << "\"\n";
    WordBST t;
    t.parse(para);
    t.displayAlphabetical();
    t.displayByFrequency();
    t.findMostFrequentWord();
    t.summary();
    return 0;
}
