#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <random>
#include <ctime>
#include <stdexcept>

using namespace std;

// ----------- BST Class -----------
class BST {
private:
    struct Node {
        int value;
        Node* left;
        Node* right;
        Node(int val) : value(val), left(nullptr), right(nullptr) {}
    };
    Node* root;

    Node* Insert(Node* node, int value) {
        if (!node) return new Node(value);
        if (value < node->value)
            node->left = Insert(node->left, value);
        else
            node->right = Insert(node->right, value);
        return node;
    }

    int Height(Node* node) const {
        if (!node) return -1;
        return 1 + max(Height(node->left), Height(node->right));
    }

    void Clear(Node* node) {
        if (!node) return;
        Clear(node->left);
        Clear(node->right);
        delete node;
    }

    Node* FindMin(Node* node) {
        while (node->left) node = node->left;
        return node;
    }

    Node* Delete(Node* node, int key) {
        if (!node) return nullptr;
        if (key < node->value) {
            node->left = Delete(node->left, key);
        }
        else if (key > node->value) {
            node->right = Delete(node->right, key);
        }
        else {
            if (!node->left && !node->right) {
                delete node;
                return nullptr;
            }
            if (!node->left) {
                Node* temp = node->right;
                delete node;
                return temp;
            }
            if (!node->right) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            Node* successor = FindMin(node->right);
            node->value = successor->value;
            node->right = Delete(node->right, successor->value);
        }
        return node;
    }

public:
    BST() : root(nullptr) {}
    ~BST() { Clear(root); }
    void Insert(int value) { root = Insert(root, value); }
    int GetHeight() const { return Height(root); }
    void Delete(int key) { root = Delete(root, key); }
};

// ----------- Input Time Calculation -----------
double calculateInputTime(int n, int S, int k, double ts, double tl, double tt) {
    int m = n / S;
    double log_k_m = log(static_cast<double>(m)) / log(static_cast<double>(k));
    int passes = static_cast<int>(ceil(log_k_m));
    double block_time = ts + tl + tt * (S / k);
    double total_blocks = (static_cast<double>(n) / S) * k;
    return passes * total_blocks * block_time;
}

// ----------- MinHeap Class -----------
template <class T>
class MinPQ {
public:
    virtual ~MinPQ() {}
    virtual bool IsEmpty() const = 0;
    virtual const T& Top() const = 0;
    virtual void Push(const T&) = 0;
    virtual void Pop() = 0;
};

template <class T>
class MinHeap : public MinPQ<T> {
private:
    vector<T> heap;
    void SiftUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[parent] <= heap[index]) break;
            swap(heap[parent], heap[index]);
            index = parent;
        }
    }
    void SiftDown(int index) {
        int minIndex = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        if (left < heap.size() && heap[left] < heap[minIndex]) minIndex = left;
        if (right < heap.size() && heap[right] < heap[minIndex]) minIndex = right;
        if (minIndex != index) {
            swap(heap[index], heap[minIndex]);
            SiftDown(minIndex);
        }
    }

public:
    MinHeap() {}
    bool IsEmpty() const override { return heap.empty(); }
    const T& Top() const override {
        if (IsEmpty()) throw runtime_error("堆為空");
        return heap[0];
    }
    void Push(const T& value) override {
        heap.push_back(value);
        SiftUp(heap.size() - 1);
    }
    void Pop() override {
        if (IsEmpty()) throw runtime_error("堆為空");
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) SiftDown(0);
    }
};

// ----------- Main Function -----------
int main() {
    // 第一段：BST 高度與刪除測試
    cout << "[BST 高度與比率]" << endl;
    vector<int> n_values = { 100, 500, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000 };
    mt19937 rng(static_cast<unsigned>(time(0)));
    uniform_int_distribution<int> dist(1, 1000000);

    for (int n : n_values) {
        BST bst;
        for (int i = 0; i < n; ++i) bst.Insert(dist(rng));
        int height = bst.GetHeight();
        double log2n = log2(n);
        double ratio = height / log2n;
        cout << "n = " << n << ", 高度 = " << height << ", log2(n) = " << log2n
            << ", 比率 = " << fixed << setprecision(2) << ratio << endl;
    }

    BST bst;
    bst.Insert(5);
    bst.Insert(3);
    bst.Insert(7);
    cout << "初始高度: " << bst.GetHeight() << endl;
    bst.Delete(5);
    cout << "刪除5後高度: " << bst.GetHeight() << endl;
    cout << endl;

    // 第二段：輸入時間模擬
    cout << "[計算 t_input]" << endl;
    int n = 200000, S = 2000;
    double ts = 80.0, tl = 20.0, tt = 1.0;
    vector<int> k_values = { 2, 4, 8, 16, 32 };

    time_t now = time(0);
    tm* ltm = localtime(&now);
    cout << "當前時間: " << 1900 + ltm->tm_year << "年"
         << 1 + ltm->tm_mon << "月" << ltm->tm_mday << "日星期"
         << (ltm->tm_wday == 0 ? "日" : ltm->tm_wday == 1 ? "一" :
             ltm->tm_wday == 2 ? "二" : ltm->tm_wday == 3 ? "三" :
             ltm->tm_wday == 4 ? "四" : ltm->tm_wday == 5 ? "五" : "六")
         << (ltm->tm_hour >= 12 ? "下午" : "上午")
         << (ltm->tm_hour >= 12 ? ltm->tm_hour - 12 : ltm->tm_hour) << ":"
         << (ltm->tm_min < 10 ? "0" : "") << ltm->tm_min << " (CST)" << endl;

    for (int k : k_values) {
        double t_input = calculateInputTime(n, S, k, ts, tl, tt) / 1000.0;
        cout << "k = " << k << ", t_input = " << fixed << setprecision(1)
             << t_input << " 秒" << endl;
    }
    cout << endl;

    // 第三段：最小堆測試
    cout << "[最小堆測試]" << endl;
    MinHeap<int> minHeap;
    minHeap.Push(5);
    minHeap.Push(3);
    minHeap.Push(7);
    minHeap.Push(1);

    cout << "最小元素: " << minHeap.Top() << endl;
    minHeap.Pop();
    cout << "刪除後最小元素: " << minHeap.Top() << endl;

    while (!minHeap.IsEmpty()) {
        cout << "彈出: " << minHeap.Top() << endl;
        minHeap.Pop();
    }

    return 0;
}
