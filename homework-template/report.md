
# 41243217 
# 41243222  
# 資料結構作業二  
# 日期：2025年6月7日

---

## 解題說明

本次作業共包含三個題目，涵蓋堆積（Heap）、二元搜尋樹（Binary Search Tree）與外部排序的實作與分析。以下依題號分別說明設計原則與實作方式。

### 題目一：最小堆積（Min Heap）實作說明

- 定義一個抽象類別 `MinPQ`，作為 Priority Queue 接口。
- 使用 `vector<T>` 來模擬一顆最小堆積。
- 設計 `SiftUp` 與 `SiftDown` 輔助函數以維持堆積性質。
- 所有操作：`Push()`、`Pop()`、`Top()` 時間複雜度為 $O(\log n)$。
- 實作過程中考慮例外處理，例如當 `Top()` 與 `Pop()` 時堆為空的情況。

### 題目二：BST 高度與刪除操作

- 插入亂數節點至 BST 中，觀察其高度與 $\log_2(n)$ 的比值。
- 設計遞迴函數計算高度，並重複試驗不同 n 值取得趨勢。
- 刪除節點時考慮三種情況：葉節點、單子節點、雙子節點（使用右子樹最小值替代）。
- 所有操作保持 BST 的結構與性質。

### 題目三：外部排序解題說明

- 題目要求計算多階段 k 路歸併所需的總輸入時間，因此需考慮：
  - 資料筆數 `n`、
  - 每個區塊大小 `S`、
  - 磁碟搜尋時間 `ts`、
  - 磁頭延遲時間 `tl`、
  - 資料傳輸時間 `tt`。
- 模擬公式為：  
  ```
  total_time = ceil(log_k(m)) × m × (ts + tl + tt × (S / k))
  ```
  其中 m 為初始區塊數 `n/S`，log_k(m) 為總合併階段數。
- 程式設計上，使用 `log()` 換底公式轉換 `log_k(m)` 為 `log(m) / log(k)`。
- 為了測試不同 k 值的效能，寫迴圈進行多組輸出觀察趨勢。
- 透過測試發現：
  - 當 k 適中時（如 8 或 16），能有效減少 passes 數。
  - 當 k 過大時，(S / k) 過小造成單位區塊傳輸成本升高，反而變慢。

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

## 程式實作

請見各段程式碼區塊與註解：

### 題目一：Min Heap

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

### 題目二：BST 插入與刪除

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

### 題目三：輸入時間模擬

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
