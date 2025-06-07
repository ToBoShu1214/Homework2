
# 41243217  
# 41243222  
JUN. 02 / 2025  

---

## 效能分析

### 題目一：Min Priority Queue 實作

- 操作時間複雜度為 $O(\log n)$，插入與刪除皆維持良好效率。
- 適用於需快速取出最小元素的情境，例如排程器、最佳優先搜尋等。

### 題目二：BST 高度分析

- BST 高度與 $\log_2(n)$ 的比值通常維持在 1.8～1.95 間，顯示隨機插入下 BST 仍然偏向平衡。
- 若使用 AVL Tree 或 Red-Black Tree 可以使高度更接近 $\log_2(n)$。

### 題目三：外部排序分析

- 當 $k$ 增加時，合併次數下降，I/O 時間減少，但當 $k$ 過大時，延遲與管理成本反而使總時間上升。
- 最佳化參數 $k$ 能顯著提升外部排序效能，需考量實際磁碟與記憶體配置。

---

## 測試與驗證

### 題目一：最小堆測試

- 插入順序：5, 3, 7, 1
- 輸出：
  - 最小值：1
  - 刪除後最小值：3
  - 依序彈出：3, 5, 7

![MinHeap 測試](https://github.com/user-attachments/assets/5220ebc1-0ddb-4818-b3c9-348debf0a8dd)

---

### 題目二：(a) BST 高度分析

- 插入 100~10000 筆隨機資料，計算高度與 $\log_2(n)$ 比值
- 測試輸出包含比值列印與趨勢圖：

![BST 高度圖](https://github.com/user-attachments/assets/43393afe-325f-4846-a420-0cab53312d18)

---

### 題目二：(b) BST 刪除功能測試

- 插入順序：5, 3, 7
- 刪除節點：5（有兩個子節點）
- 高度變化印證刪除後結構正確

![BST 刪除測試](https://github.com/user-attachments/assets/479cf367-ea85-49aa-be3b-911f90985f13)

---

### 題目三：外部排序輸入時間

- 設定：
  - $n = 200000$
  - $S = 2000$
  - $t_s = 80$ms，$t_l = 20$ms，$t_t = 1$ms/record
- 測試 $k = 2, 4, 8, 16, 32$
- 輸出輸入時間：

![外部排序圖](https://github.com/user-attachments/assets/21aba3d4-e109-41bb-8750-cc4719d79ecd)

---

## 申論及開發報告

### 題目一：

- 本題實作最小堆以滿足 `MinPQ` 抽象介面，強調資料封裝與虛擬函數的使用。
- 使用 vector 作為底層資料結構，方便動態調整容量與快速查找。

### 題目二：

- 高度實驗說明 BST 在隨機資料下表現近似平衡樹。
- 刪除功能涵蓋三種典型情況，透過遞迴與取代機制確保 BST 結構完整性。

### 題目三：

- 模擬外部排序的 I/O 行為，針對 k-way merge 次數與每次讀取成本分析。
- 實作展示不同 $k$ 值對輸入時間的影響，有助於未來系統參數調校。

---

## 程式實作

### 題目一：最小堆實作 (`MinHeap`)

```cpp
template <class T>
class MinHeap : public MinPQ<T> {
private:
    vector<T> heap;

    void SiftUp(int index);
    void SiftDown(int index);

public:
    MinHeap() {}
    bool IsEmpty() const override;
    const T& Top() const override;
    void Push(const T& value) override;
    void Pop() override;
};
```

---

### 題目二：(a) BST 高度分析主程式

```cpp
int main() {
    vector<int> n_values = { 100, 500, ..., 10000 };
    for (int n : n_values) {
        BST bst;
        for (int i = 0; i < n; ++i) {
            bst.Insert(dist(rng));
        }
        int height = bst.GetHeight();
        double log2n = log2(n);
        double ratio = height / log2n;
        ...
    }
}
```

---

### 題目二：(b) BST 刪除功能

```cpp
Node* Delete(Node* node, int key) {
    if (!node) return nullptr;
    if (key < node->value) ...
    else if (key > node->value) ...
    else {
        if (!node->left && !node->right) ...
        else if (!node->left) ...
        else if (!node->right) ...
        else {
            Node* successor = FindMin(node->right);
            node->value = successor->value;
            node->right = Delete(node->right, successor->value);
        }
    }
    return node;
}
```

---

### 題目三：外部排序時間計算

```cpp
double calculateInputTime(int n, int S, int k, double ts, double tl, double tt) {
    int m = n / S;
    double log_k_m = log(m) / log(k);
    int passes = ceil(log_k_m);
    double block_time = ts + tl + tt * (S / k);
    double total_blocks = (n / S) * k;
    return passes * total_blocks * block_time;
}
```

---
