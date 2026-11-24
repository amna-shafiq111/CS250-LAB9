// CS250 Lab 9 - Task 1: Range Query and Successor/Predecessor using BST
// Name: Amna Shafiq | CMS-ID: 552522 | BSDS 3A
#include <iostream>
#include <vector>
using namespace std;

struct Node {
    int key;
    Node *left, *right;
    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

// Insert a key into the BST (iterative)
Node* insert(Node* root, int key) {
    if (!root) return new Node(key);
    Node* cur = root;
    while (true) {
        if (key < cur->key) {
            if (!cur->left) { cur->left = new Node(key); break; }
            cur = cur->left;
        } else {
            if (!cur->right) { cur->right = new Node(key); break; }
            cur = cur->right;
        }
    }
    return root;
}

// Inorder traversal
void inorder(Node* r) {
    if (!r) return;
    inorder(r->left);
    cout << r->key << " ";
    inorder(r->right);
}

// Find inorder predecessor and successor of 'key'
void findPredSucc(Node* root, int key, Node*& pred, Node*& succ) {
    pred = succ = nullptr;
    Node* cur = root;
    while (cur) {
        if (key < cur->key) { succ = cur; cur = cur->left; }
        else if (key > cur->key) { pred = cur; cur = cur->right; }
        else {
            // predecessor = max of left subtree
            if (cur->left) { Node* t = cur->left; while (t->right) t = t->right; pred = t; }
            // successor = min of right subtree
            if (cur->right) { Node* t = cur->right; while (t->left) t = t->left; succ = t; }
            return;
        }
    }
}

// Recursive range query (prints keys in [lo, hi] in sorted order)
void rangeRecursive(Node* r, int lo, int hi, vector<int>& res, int& comps) {
    if (!r) return;
    comps++; // compare with lo
    if (lo < r->key) rangeRecursive(r->left, lo, hi, res, comps);
    comps++; // check range membership
    if (r->key >= lo && r->key <= hi) res.push_back(r->key);
    comps++; // compare with hi
    if (hi > r->key) rangeRecursive(r->right, lo, hi, res, comps);
}

// Iterative range query using an explicit stack (inorder style)
void rangeIterative(Node* root, int lo, int hi, vector<int>& res, int& comps) {
    vector<Node*> st;
    Node* cur = root;
    while (cur || !st.empty()) {
        while (cur) {
            st.push_back(cur);
            comps++; // compare with lo
            if (lo < cur->key) cur = cur->left;
            else cur = nullptr; // everything left is smaller than lo
        }
        if (st.empty()) break;
        cur = st.back(); st.pop_back();
        comps++; // range membership check
        if (cur->key >= lo && cur->key <= hi) res.push_back(cur->key);
        comps++; // compare with hi
        if (hi > cur->key) cur = cur->right;
        else cur = nullptr; // everything right is larger than hi
    }
}

int main() {
    vector<int> data = {50, 30, 70, 20, 40, 60, 80, 35, 45, 65, 85};
    Node* root = nullptr;
    for (int x : data) root = insert(root, x);

    cout << "===== TASK 1: BST Range Query & Successor/Predecessor =====\n";
    cout << "Input sequence      : ";
    for (int x : data) cout << x << " ";
    cout << "\nInorder traversal   : "; inorder(root); cout << "\n\n";

    int key = 40;
    Node *p, *s;
    findPredSucc(root, key, p, s);
    cout << "Key = " << key << "\n";
    cout << "Inorder predecessor : " << (p ? to_string(p->key) : "None") << "\n";
    cout << "Inorder successor   : " << (s ? to_string(s->key) : "None") << "\n\n";

    int lo = 30, hi = 70;
    vector<int> r1, r2; int c1 = 0, c2 = 0;
    rangeRecursive(root, lo, hi, r1, c1);
    rangeIterative(root, lo, hi, r2, c2);

    cout << "Range query [" << lo << ", " << hi << "]\n";
    cout << "Recursive result    : "; for (int x : r1) cout << x << " ";
    cout << "\n  Comparisons       : " << c1 << "\n";
    cout << "Iterative result    : "; for (int x : r2) cout << x << " ";
    cout << "\n  Comparisons       : " << c2 << "\n";
    return 0;
}
