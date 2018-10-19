#include <bits/stdc++.h>

using namespace std;
using ll = long long;

int pow2(int x) {
    int l = 1;
    while (l < x) l <<= 1;
    return l;
}

template<typename T, typename U>
struct lazy_sgt {
    using op = T (*)(T, T);
    struct node {
        T val;
        bool ovr = false;
        U upd;
    };
    using ov = void (*)(node &, U, int);

    const int n;
    vector<node> tree;
    const T id;
    const op merge;
    const ov overwrite;

    lazy_sgt(int n, T id, op m, ov o)
        : n{pow2(n)}, tree{2 * this->n + 1}, id{id}, merge{m}, overwrite{o} {}

    // updates [a, b) with val
    void update(int a, int b, U upd) {
        update(1, a, b, upd, 0, n);
    }

    // queries [a, b)
    T query(int a, int b) {
        return query(1, a, b, 0, n);
    }

    void update(int i, int a, int b, U upd, int left, int right) {
        if (b <= left || a >= right) return; // doesn't intersect
        if (right - left == 1) overwrite(tree[i], upd, right - left);
        else if (a <= left && right <= b) { // wholly contained
            overwrite(tree[i], upd, right - left);
        } else { // intersects
            int mid = left + (right - left) / 2;
            if (tree[i].ovr == true) {
                overwrite(tree[2 * i], tree[i].upd, mid - left);
                overwrite(tree[2 * i + 1], tree[i].upd, right - mid);
                tree[i].ovr = false;
            }
            update(2 * i, a, b, upd, left, mid);
            update(2 * i + 1, a, b, upd, mid, right);
            tree[i].val = merge(tree[2 * i].val, tree[2 * i + 1].val);
        }
    }

    T query(int i, int a, int b, int left, int right) {

        if (b <= left || a >= right) {return id;} // doesn't intersect
        if (right - left == 1) { return tree[i].val;}
        else if (a <= left && right <= b) { // wholly contained
            return tree[i].val;
        } else {
            int mid = left + (right - left) / 2;
            if (tree[i].ovr == true) {
                overwrite(tree[2 * i], tree[i].upd, mid - left);
                overwrite(tree[2 * i + 1], tree[i].upd, right - mid);
                tree[i].ovr = false;
            }
            T l = query(2 * i, a, b, left, mid);
            T r = query(2 * i + 1, a, b, mid, right);
            return merge(l, r);
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    // Solution to https://www.spoj.com/problems/HORRIBLE/
    using lsgt = lazy_sgt<ll, ll>;
    lsgt::op plus =
        [] (ll a, ll b) {
            return a + b;
        };
    lsgt::ov upd =
        [] (lsgt::node &a, ll u, int width) {
            a.val += u * width;
            if (a.ovr == false) {
                a.upd = u;
            } else {
                a.upd += u;
            }
            a.ovr = true;
        };

    int T;
    cin >> T;
    for (int testcase = 0; testcase < T; ++testcase) {
        int N, C;
        cin >> N >> C;
        lsgt tree(N, 0ll, plus, upd);
        for (int i = 0; i < C; ++i) {
            int c, a, b, v;
            cin >> c >> a >> b;
            if (c == 0) {
                cin >> v;
                tree.update(a - 1, b, (ll)v);
            } else {
                cout << tree.query(a - 1, b) << "\n";
            }
        }
    }
    return 0;
}
