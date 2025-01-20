#include <stdio.h>
#include <stdlib.h>

// Cấu trúc đại diện cho một cạnh
typedef struct Edge {
    int src, dest, weight;  // src: đỉnh nguồn, dest: đỉnh đích, weight: trọng số của cạnh
} Edge;

// Cấu trúc đại diện cho một đồ thị
typedef struct Graph {
    int V, E;       // V: số lượng đỉnh, E: số lượng cạnh
    Edge* edges;    // Mảng các cạnh của đồ thị
} Graph;

// Tạo đồ thị với V đỉnh và E cạnh
Graph* createGraph(int V, int E) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));  // Cấp phát bộ nhớ cho đồ thị
    graph->V = V;  // Số đỉnh của đồ thị
    graph->E = E;  // Số cạnh của đồ thị
    graph->edges = (Edge*)malloc(E * sizeof(Edge));  // Cấp phát bộ nhớ cho mảng cạnh
    return graph;  // Trả về con trỏ đồ thị
}

// Cấu trúc đại diện cho tập hợp Disjoint Set (Tập hợp rời rạc)
typedef struct Subset {
    int parent;  // Lưu cha của phần tử (đỉnh)
    int rank;    // Lưu độ sâu của tập hợp (dùng cho kỹ thuật Union by Rank)
} Subset;

// Tìm đại diện của tập hợp chứa u (dùng path compression để tối ưu)
int find(Subset subsets[], int u) {
    // Nếu u không phải là gốc (root) thì tiếp tục gọi đệ quy để tìm gốc
    if (subsets[u].parent != u) {
        subsets[u].parent = find(subsets, subsets[u].parent);  // Path compression
    }
    return subsets[u].parent;  // Trả về gốc của tập hợp chứa u
}

// Hợp nhất hai tập hợp chứa x và y (dựa trên rank)
void unionSets(Subset subsets[], int x, int y) {
    int rootX = find(subsets, x);  // Tìm gốc của tập hợp chứa x
    int rootY = find(subsets, y);  // Tìm gốc của tập hợp chứa y

    // Nếu rank của rootX nhỏ hơn rootY, gắn rootX làm con của rootY
    if (subsets[rootX].rank < subsets[rootY].rank) {
        subsets[rootX].parent = rootY;
    }
    // Nếu rank của rootX lớn hơn rootY, gắn rootY làm con của rootX
    else if (subsets[rootX].rank > subsets[rootY].rank) {
        subsets[rootY].parent = rootX;
    }
    // Nếu rank bằng nhau, chọn một root làm gốc và tăng rank của nó
    else {
        subsets[rootY].parent = rootX;
        subsets[rootX].rank++;  // Tăng rank của rootX sau khi hợp nhất
    }
}

// Hàm so sánh để sắp xếp các cạnh theo trọng số tăng dần
int compareEdges(const void* a, const void* b) {
    Edge* edgeA = (Edge*)a;
    Edge* edgeB = (Edge*)b;
    return edgeA->weight - edgeB->weight;  // Sắp xếp theo trọng số
}

// Hàm chính của thuật toán Kruskal
void kruskal(Graph* graph) {
    int V = graph->V;  // Số đỉnh của đồ thị
    Edge result[V];     // Mảng lưu trữ các cạnh trong cây bao trùm nhỏ nhất (MST)
    int edgeCount = 0;  // Biến đếm số lượng cạnh trong MST
    int i = 0;          // Biến duyệt qua mảng các cạnh đã sắp xếp

    // Bước 1: Sắp xếp các cạnh theo trọng số tăng dần
    qsort(graph->edges, graph->E, sizeof(Edge), compareEdges);

    // Tạo tập hợp rời rạc (mỗi đỉnh là một tập riêng biệt)
    Subset* subsets = (Subset*)malloc(V * sizeof(Subset));  // Cấp phát bộ nhớ cho mảng subsets
    for (int v = 0; v < V; v++) {
        subsets[v].parent = v;  // Mỗi đỉnh là cha của chính nó
        subsets[v].rank = 0;     // Rank ban đầu là 0
    }

    // Bước 2: Lặp qua các cạnh đã sắp xếp để xây dựng MST
    while (edgeCount < V - 1 && i < graph->E) {  // Dừng khi đã có đủ V-1 cạnh
        Edge nextEdge = graph->edges[i++];  // Lấy cạnh tiếp theo từ mảng đã sắp xếp

        // Tìm đại diện của hai đỉnh đầu của cạnh (src, dest)
        int x = find(subsets, nextEdge.src);
        int y = find(subsets, nextEdge.dest);

        // Nếu hai đỉnh thuộc các tập hợp khác nhau, tức là không tạo chu trình
        if (x != y) {
            result[edgeCount++] = nextEdge;  // Thêm cạnh vào MST
            unionSets(subsets, x, y);  // Hợp nhất hai tập hợp của x và y
        }
    }

    // Kiểm tra nếu không đủ số cạnh để tạo MST
    if (edgeCount != V - 1) {
        printf("Không đủ số cạnh để tạo cây bao trùm nhỏ nhất.\n");
        return;  // Dừng hàm nếu không thể tạo MST
    }

    // In các cạnh trong MST
    printf("Các cạnh trong MST:\n");
    for (i = 0; i < edgeCount; i++) {
        printf("(%d -- %d) trọng số: %d\n", result[i].src, result[i].dest, result[i].weight);
    }

    // Giải phóng bộ nhớ đã cấp phát cho subsets
    free(subsets);
}

int main() {
    // Ví dụ dữ liệu đầu vào
    int V = 7;  // Số đỉnh
    int E = 9;  // Số cạnh
    Graph* graph = createGraph(V, E);  // Tạo đồ thị

    // Định nghĩa các cạnh (src, dest, weight) trong đồ thị
    graph->edges[0] = (Edge){0, 1, 7};
    graph->edges[1] = (Edge){1, 2, 8};
    graph->edges[2] = (Edge){0, 3, 5};
    graph->edges[3] = (Edge){1, 4, 7};
    graph->edges[4] = (Edge){3, 4, 15};
    graph->edges[5] = (Edge){3, 5, 6};
    graph->edges[6] = (Edge){4, 5, 8};
    graph->edges[7] = (Edge){4, 6, 9};
    graph->edges[8] = (Edge){5, 6, 11};

    // Chạy thuật toán Kruskal để xây dựng MST
    kruskal(graph);

    // Giải phóng bộ nhớ cho các cạnh và đồ thị
    free(graph->edges);
    free(graph);

    return 0;
}
