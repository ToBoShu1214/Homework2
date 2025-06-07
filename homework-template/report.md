# 41243217 41243222
JUN. 02 / 2025  

---

##  解題說明

本次作業分為三個部分：

---

##  題目一：Min Priority Queue 實作

###  說明
實作一個抽象類別 `MinPQ` 並定義以下函數：

- `IsEmpty()`：判斷是否為空
- `Top()`：回傳最小值
- `Push()`：加入元素
- `Pop()`：刪除最小元素

接著定義一個 `MinHeap` 類別繼承 `MinPQ`，以 **vector** 實作最小堆。

###  程式碼
```cpp
#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;

template <class T>
class MinPQ {
public:
    virtual ~MinPQ() {} // 虛擬解構函數
    virtual bool IsEmpty() const = 0; // 回傳true如果優先隊列為空
    virtual const T& Top() const = 0; // 回傳最小元素的引用
    virtual void Push(const T&) = 0; // 向優先隊列添加元素
    virtual void Pop() = 0; // 刪除具有最小優先級的元素
};

template <class T>
class MinHeap : public MinPQ<T> {
private:
    vector<T> heap; // 儲存堆元素

    // 輔助函數，用於堆操作
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

        if (left < heap.size() && heap[left] < heap[minIndex])
            minIndex = left;
        if (right < heap.size() && heap[right] < heap[minIndex])
            minIndex = right;

        if (minIndex != index) {
            swap(heap[index], heap[minIndex]);
            SiftDown(minIndex);
        }
    }

public:
    MinHeap() {} // 建構函數

    bool IsEmpty() const override {
        return heap.empty();
    }

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

int main() {
    MinHeap<int> minHeap;

    // 測試最小堆
    minHeap.Push(5);
    minHeap.Push(3);
    minHeap.Push(7);
    minHeap.Push(1);

    cout << "最小元素: " << minHeap.Top() << endl; // 應輸出1
    minHeap.Pop();
    cout << "刪除最小元素後最小元素: " << minHeap.Top() << endl; // 應輸出3

    while (!minHeap.IsEmpty()) {
        cout << "彈出: " << minHeap.Top() << endl;
        minHeap.Pop();
    }

    return 0;
}
```
### 測試結果
![image](https://github.com/user-attachments/assets/5220ebc1-0ddb-4818-b3c9-348debf0a8dd)

##  題目二：Binary Search Tree 高度分析與刪除功能

### (a) BST 高度分析

建立一個初始為空的 BST，插入 n 筆隨機資料，計算其高度並與 $\log_2(n)$ 做比值，觀察其比值變化。

###  圖片

![image](https://github.com/user-attachments/assets/43393afe-325f-4846-a420-0cab53312d18)


###  程式碼

```cpp
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <ctime>

using namespace std;

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

public:
    BST() : root(nullptr) {}
    ~BST() { Clear(root); }

    void Insert(int value) {
        root = Insert(root, value);
    }

    int GetHeight() const {
        return Height(root);
    }
};

int main() {
    vector<int> n_values = { 100, 500, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000 };
    vector<double> ratios;

    mt19937 rng(static_cast<unsigned>(time(0)));
    uniform_int_distribution<int> dist(1, 1000000);

    for (int n : n_values) {
        BST bst;
        for (int i = 0; i < n; ++i) {
            bst.Insert(dist(rng));
        }
        int height = bst.GetHeight();
        double log2n = log2(n);
        double ratio = height / log2n;
        ratios.push_back(ratio);
        cout << "n = " << n << ", 高度 = " << height << ", log2(n) = " << log2n
             << ", 比率 = " << ratio << endl;
    }

    return 0;
}
```
---

### (b) BST 刪除功能實作

實作 BST 的刪除功能，處理以下三種情況：

1. 被刪除節點為葉子
2. 被刪除節點只有一個子節點
3. 被刪除節點有兩個子節點（用右子樹的最小值替代）

###  程式碼

```cpp
#include <iostream>
using namespace std;

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

    void Clear(Node* node) {
        if (!node) return;
        Clear(node->left);
        Clear(node->right);
        delete node;
    }

    int Height(Node* node) const {
        if (!node) return -1;
        return 1 + max(Height(node->left), Height(node->right));
    }

public:
    BST() : root(nullptr) {}
    ~BST() { Clear(root); }

    void Insert(int value) {
        root = Insert(root, value);
    }

    void Delete(int key) {
        root = Delete(root, key);
    }

    int GetHeight() const {
        return Height(root);
    }
};

int main() {
    BST bst;
    bst.Insert(5);
    bst.Insert(3);
    bst.Insert(7);
    cout << "初始高度: " << bst.GetHeight() << endl;

    bst.Delete(5);
    cout << "刪除5後高度: " << bst.GetHeight() << endl;

    return 0;
}
```
### 測試結果
![image](https://github.com/user-attachments/assets/479cf367-ea85-49aa-be3b-911f90985f13)

---


##  題目三：外部排序輸入時間分析

###  說明

已知輸入資料總數 $n = 200000$，記憶體容量 $S = 2000$，以 $k$ 路歸併進行排序，計算輸入時間：

- 磁碟搜尋時間 $t_s = 80$ms
- 延遲時間 $t_l = 20$ms
- 傳輸時間 $t_t = 1$ms/record

計算並繪製 $k = 2, 4, 8, 16, 32$ 時的總輸入時間變化。

###  圖片

  ![image](https://github.com/user-attachments/assets/21aba3d4-e109-41bb-8750-cc4719d79ecd)


###  程式碼

```cpp
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <ctime>

using namespace std;

double calculateInputTime(int n, int S, int k, double ts, double tl, double tt) {
    int m = n / S; // 假設n/S整除
    double log_k_m = log(static_cast<double>(m)) / log(static_cast<double>(k));
    int passes = static_cast<int>(ceil(log_k_m));
    double block_time = ts + tl + tt * (S / k);
    double total_blocks = (static_cast<double>(n) / S) * k;
    return passes * total_blocks * block_time;
}

int main() {
    int n = 200000;
    int S = 2000;
    int m = 100; // 使用n/S=100，忽略問題中的m=64以保持一致性
    double ts = 80.0; // ms
    double tl = 20.0; // ms
    double tt = 1.0;  // ms/record
    vector<int> k_values = { 2, 4, 8, 16, 32 };
    vector<double> input_times;

    // 獲取當前時間
    time_t now = time(0);
    tm* ltm = localtime(&now);
    cout << "當前日期和時間: " << 1900 + ltm->tm_year << "年"
        << 1 + ltm->tm_mon << "月" << ltm->tm_mday << "日星期"
        << (ltm->tm_wday == 1 ? "一" : ltm->tm_wday == 2 ? "二" :
            ltm->tm_wday == 3 ? "三" : ltm->tm_wday == 4 ? "四" :
            ltm->tm_wday == 5 ? "五" : ltm->tm_wday == 6 ? "六" : "日")
        << (ltm->tm_hour >= 12 ? "下午" : "上午")
        << (ltm->tm_hour >= 12 ? ltm->tm_hour - 12 : ltm->tm_hour) << ":"
        << (ltm->tm_min < 10 ? "0" : "") << ltm->tm_min << " (CST)" << endl;
    cout << "計算 t_input (單位: 秒):" << endl;

    for (int k : k_values) {
        double t_input = calculateInputTime(n, S, k, ts, tl, tt) / 1000.0; // 轉換為秒
        input_times.push_back(t_input);
        cout << "k = " << k << ", t_input = " << fixed << setprecision(1)
            << t_input << " 秒" << endl;
    }

    return 0;
}
```
### 測試結果
![image](https://github.com/user-attachments/assets/b2acfcbe-b560-46ec-9643-ff83de3b5658)

---

##  註解與觀察

- 題目一中最小堆操作時間複雜度為 $O(\log n)$。
- 題目二中實驗數據顯示 BST 高度與 $\log_2(n)$ 的比值大約維持在 1.8～1.95 之間。
- 題目三中輸入時間會先隨 $k$ 遞增而下降，但超過某個 $k$ 後會反而增加。

