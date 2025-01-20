### **Mô tả giải thuật Kruskal’s Algorithm theo các bước phân tích sơ bộ**

#### **1. Bài toán:**
Cho một đồ thị vô hướng, liên thông và có trọng số, mục tiêu là tìm **Minimum Spanning Tree (MST)** - một cây bao trùm với tổng trọng số của các cạnh nhỏ nhất, kết nối tất cả các đỉnh của đồ thị.

---

#### **2. Phân tích giải thuật:**
Kruskal’s Algorithm là một thuật toán **tham lam** (greedy), thực hiện qua các bước sau:

**Bước 1: Khởi tạo**
- Đồ thị \( G = (V, E) \), với \( V \) là tập đỉnh và \( E \) là tập cạnh.
- Khởi tạo MST rỗng (không có cạnh) và mỗi đỉnh là một tập hợp rời rạc.

**Bước 2: Sắp xếp cạnh**
- Sắp xếp tất cả các cạnh của đồ thị theo thứ tự tăng dần của trọng số.

**Bước 3: Chọn cạnh**
- Lặp lại cho đến khi MST chứa đủ \( |V| - 1 \) cạnh:
  - Chọn cạnh \( e \) có trọng số nhỏ nhất từ tập cạnh chưa xét.
  - Kiểm tra xem cạnh \( e \) có tạo thành chu trình khi thêm vào MST hay không:
    - Nếu không, thêm \( e \) vào MST.
    - Nếu có, bỏ qua \( e \).
  - Kiểm tra chu trình bằng **Union-Find**:
    - **Find:** Kiểm tra tập hợp của hai đỉnh.
    - **Union:** Hợp nhất hai tập hợp nếu không cùng thuộc một tập.

**Bước 4: Kết thúc**
- Khi MST có đủ \( |V| - 1 \) cạnh, thuật toán kết thúc và trả về tập cạnh trong MST.

---

#### **3. Pseudocode:**

```text
Function Kruskal(G):
    MST = {}  # Khởi tạo cây bao trùm rỗng
    sort_edges(G.E)  # Sắp xếp các cạnh theo trọng số tăng dần
    
    for vertex v in G.V:
        MAKE-SET(v)  # Mỗi đỉnh ban đầu là một tập hợp riêng lẻ
    
    for edge (u, v) in G.E:
        if FIND(u) != FIND(v):  # Kiểm tra nếu u và v thuộc các tập khác nhau
            MST.add(edge)  # Thêm cạnh vào MST
            UNION(u, v)  # Hợp nhất hai tập hợp của u và v
    
    return MST
```

---
