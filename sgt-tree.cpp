#include <bits/stdc++.h>

using namespace std;

struct node {
    int i;

    node() {
        i = 0;
    }

    node(int i) {
        this->i = i;
    }

    node operator+(node other) {
        return node(i + other.i);
    }
};

struct sgt {

    int n;
    vector<node> a;

    sgt(int n) {
        this->n = n;
        a.resize(n<<1);
    }

    void assign(int i, node value) {
        a[n+i] = value;
    }

    void build() {
        for (int i = n - 1; i > 0; --i) a[i] = a[i<<1] + a[i<<1|1];
    }

    void update(int i, node value) {
        for (a[i += n] = value; i > 1; i >>=1 ) a[i>>1] = a[i] + a[i^1];
    }

    node query(int l, int r) {
        node ret;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l&1) ret = ret + a[l++];
            if (r&1) ret = ret + a[--r];
        }
        return ret;
    }
};



int main() {
    sgt s(5);
    s.assign(0, node(3));
    s.assign(1, node(4));
    s.assign(2, node(2));
    s.assign(3, node(5));
    s.assign(4, node(11));
    s.build();
    cout << s.query(0,3).i << endl;
    cout << s.query(2,4).i << endl;
    s.update(2,node(-1));
    cout << s.query(0,3).i << endl;
    return 0;
}
