#include <iostream>
#include <cstdio>
using namespace std;

// Treap implementation (only <iostream> and <cstdio> are used)
struct TreapNode {
    int key;
    unsigned int pri;
    int left, right;
} nodes[600000]; // enough for all operations (max 5e5 adds)
int node_cnt = 0;
int root = 0;

int new_node(int key) {
    ++node_cnt;
    nodes[node_cnt].key = key;
    nodes[node_cnt].pri = (unsigned int)rand();
    nodes[node_cnt].left = nodes[node_cnt].right = 0;
    return node_cnt;
}

int rotate_right(int y) {
    int x = nodes[y].left;
    nodes[y].left = nodes[x].right;
    nodes[x].right = y;
    return x;
}

int rotate_left(int x) {
    int y = nodes[x].right;
    nodes[x].right = nodes[y].left;
    nodes[y].left = x;
    return y;
}

int insert_node(int cur, int key) {
    if (!cur) return new_node(key);
    if (key < nodes[cur].key) {
        nodes[cur].left = insert_node(nodes[cur].left, key);
        if (nodes[nodes[cur].left].pri > nodes[cur].pri)
            cur = rotate_right(cur);
    } else if (key > nodes[cur].key) {
        nodes[cur].right = insert_node(nodes[cur].right, key);
        if (nodes[nodes[cur].right].pri > nodes[cur].pri)
            cur = rotate_left(cur);
    }
    return cur;
}

int erase_node(int cur, int key) {
    if (!cur) return 0;
    if (key < nodes[cur].key) {
        nodes[cur].left = erase_node(nodes[cur].left, key);
    } else if (key > nodes[cur].key) {
        nodes[cur].right = erase_node(nodes[cur].right, key);
    } else {
        if (!nodes[cur].left && !nodes[cur].right) return 0;
        if (!nodes[cur].left) return nodes[cur].right;
        if (!nodes[cur].right) return nodes[cur].left;
        if (nodes[nodes[cur].left].pri > nodes[nodes[cur].right].pri) {
            cur = rotate_right(cur);
            nodes[cur].right = erase_node(nodes[cur].right, key);
        } else {
            cur = rotate_left(cur);
            nodes[cur].left = erase_node(nodes[cur].left, key);
        }
    }
    return cur;
}

int lower_bound_node(int cur, int key) {
    int res = 0;
    while (cur) {
        if (nodes[cur].key >= key) {
            res = cur;
            cur = nodes[cur].left;
        } else {
            cur = nodes[cur].right;
        }
    }
    return res;
}

int predecessor_node(int cur, int key) {
    int res = 0;
    while (cur) {
        if (nodes[cur].key < key) {
            res = cur;
            cur = nodes[cur].right;
        } else {
            cur = nodes[cur].left;
        }
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    srand(71268726);
    int n;
    if(!(cin>>n)) return 0;
    long long cur_floor = 0;
    int dir = 1;
    string op;
    for(int i=0;i<n;++i){
        cin>>op;
        if(op=="add"){
            long long x;cin>>x;root=insert_node(root,(int)x);
        } else if(op=="cancel"){
            long long x;cin>>x;root=erase_node(root,(int)x);
        } else if(op=="locate"){
            cout<<cur_floor<<'\n';
        } else if(op=="action"){
            if(!root) continue;
            if(dir==1){
                int idx=lower_bound_node(root,(int)(cur_floor+1));
                if(idx){
                    cur_floor=nodes[idx].key;root=erase_node(root,nodes[idx].key);
                } else {dir=-1;int pid=predecessor_node(root,(int)cur_floor);if(pid){cur_floor=nodes[pid].key;root=erase_node(root,nodes[pid].key);} }
            } else {
                int pid=predecessor_node(root,(int)cur_floor);
                if(pid){cur_floor=nodes[pid].key;root=erase_node(root,nodes[pid].key);} else {dir=1;int idx=lower_bound_node(root,(int)(cur_floor+1));if(idx){cur_floor=nodes[idx].key;root=erase_node(root,nodes[idx].key);} }
            }
        }
    }
    return 0;
}

