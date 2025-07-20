#include <bits/stdc++.h>
using namespace std;

// Compute the closure of X under FDs
set<string> closure(const set<string>& X,
                    const vector<pair<set<string>, set<string>>>& fds) {
    set<string> C = X;
    bool added = true;
    while (added) {
        added = false;
        for (auto &fd : fds) {
            // if fd.first ⊆ C, add all of fd.second
            if (includes(C.begin(), C.end(),
                         fd.first.begin(), fd.first.end())) {
                for (auto &a : fd.second)
                    if (C.insert(a).second)
                        added = true;
            }
        }
    }
    return C;
}

// Enumerate all nonempty subsets of attributes; pick those whose closure = full set
vector<set<string>> find_superkeys(
    const vector<string>& attrs,
    const vector<pair<set<string>, set<string>>>& fds)
{
    int n = attrs.size();
    vector<set<string>> sk;
    set<string> full(attrs.begin(), attrs.end());
    for (int mask = 1; mask < (1<<n); ++mask) {
        set<string> X;
        for (int i = 0; i < n; ++i)
            if (mask & (1<<i))
                X.insert(attrs[i]);
        if (closure(X, fds) == full)
            sk.push_back(move(X));
    }
    return sk;
}

// From superkeys, filter minimal ones = candidate keys
vector<set<string>> find_candidate_keys(const vector<set<string>>& sk) {
    vector<set<string>> ck;
    int m = sk.size();
    for (int i = 0; i < m; ++i) {
        bool minimal = true;
        for (int j = 0; j < m; ++j) {
            if (j == i) continue;
            // if sk[j] is a proper subset of sk[i], i is not minimal
            if (sk[j].size() < sk[i].size() &&
                includes(sk[i].begin(), sk[i].end(),
                         sk[j].begin(), sk[j].end()))
            {
                minimal = false;
                break;
            }
        }
        if (minimal) ck.push_back(sk[i]);
    }
    return ck;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // --- Input ---
    // k attributes, then their names
    int k; 
    cout << "Enter number of attributes followed by attribute names:\n"<<flush;
    cin >> k;
    vector<string> attrs(k);
    for (int i = 0; i < k; ++i) {
        cin >> attrs[i];
    }
    // m FDs, each as: p q  A1...Ap  B1...Bq
    int m;
    cout << "Enter number of functional dependencies:\n"<<flush;
    cin >> m;
    cout << "For each FD, enter number of attributes in LHS followed by LHS attributes, then number of attributes in RHS followed by RHS attributes:\n"<<flush;
    vector<pair<set<string>, set<string>>> fds(m);
    for (int i = 0; i < m; ++i) {
        int p, q;
        cin >> p >> q;
        set<string> L, R;
        for (int j = 0; j < p; ++j) {
            string a; cin >> a;
            L.insert(a);
        }
        for (int j = 0; j < q; ++j) {
            string b; cin >> b;
            R.insert(b);
        }
        fds[i] = {L, R};
    }

    // --- Compute keys ---
    auto superkeys      = find_superkeys(attrs, fds);
    auto candidate_keys = find_candidate_keys(superkeys);

    // Compute prime attributes
    set<string> prime;
    for (auto &ck : candidate_keys)
        prime.insert(ck.begin(), ck.end());

    // --- Check normal forms ---
    bool is2NF  = true;
    bool is3NF  = true;
    bool isBCNF = true;
    set<string> full(attrs.begin(), attrs.end());

    for (auto &fd : fds) {
        const auto &L = fd.first;
        const auto &R = fd.second;

        // BCNF: L must be a superkey
        if (closure(L, fds) != full) {
            isBCNF = false;
        }

        // 2NF: no partial dependency of nonprime on part of a CK
        for (auto &ck : candidate_keys) {
            if (L.size() < ck.size() &&
                includes(ck.begin(), ck.end(), L.begin(), L.end()))
            {
                // L is a proper subset of ck
                for (auto &a : R) {
                    if (!prime.count(a)) {
                        is2NF = false;
                    }
                }
            }
        }

        // 3NF: for any FD L->A, either L is superkey, or A is prime
        if (closure(L, fds) != full) {
            for (auto &a : R) {
                if (!prime.count(a)) {
                    is3NF = false;
                }
            }
        }
    }

    // --- Output ---
    cout << "Superkeys:\n";
    for (auto &sk : superkeys) {
        for (auto &a : sk) cout << a << ' ';
        cout << "\n";
    }
    cout << "\nCandidate Keys:\n";
    for (auto &ck : candidate_keys) {
        for (auto &a : ck) cout << a << ' ';
        cout << "\n";
    }

    cout << "\nHighest Normal Form: ";
    if (isBCNF)      cout << "BCNF\n";
    else if (is3NF)  cout << "3NF\n";
    else if (is2NF)  cout << "2NF\n";
    else             cout << "1NF\n";

    return 0;
}