#include <bits/stdc++.h>
using namespace std;

// Compute closure of X under given FDs
set<string> closure(const set<string>& X,
                    const vector<pair<set<string>, set<string>>>& F) {
    set<string> result = X;
    bool added;
    do {
        added = false;
        for (const auto& fd : F) {
            const auto& lhs = fd.first;
            const auto& rhs = fd.second;
            // If lhs ⊆ result, then add rhs
            if (includes(result.begin(), result.end(),
                         lhs.begin(), lhs.end())) {
                for (const auto& a : rhs) {
                    if (!result.count(a)) {
                        result.insert(a);
                        added = true;
                    }
                }
            }
        }
    } while (added);
    return result;
}

// Check if X is a superkey
bool isSuperkey(const set<string>& X,
                const vector<pair<set<string>, set<string>>>& F,
                const set<string>& allAttrs) {
    return closure(X, F).size() == allAttrs.size();
}

int main() {


    int n;
    cout << "Enter number of attributes followed by attribute names:\n";
    cin >> n;
    set<string> allAttrs;
    for (int i = 0; i < n; i++) {
        string a;
        cin >> a;
        allAttrs.insert(a);
    }

    int k;
    cout << "Enter number of attributes in primary key followed by the attributes:\n";
    cin >> k;
    set<string> PK;
    for (int i = 0; i < k; i++) {
        string a;
        cin >> a;
        PK.insert(a);
    }

    int m;
    cout << "Enter number of functional dependencies:\n";
    cin >> m;
    vector<pair<set<string>, set<string>>> F;
    cout << "For each FD, enter number of attributes in LHS followed by LHS attributes, then number of attributes in RHS followed by RHS attributes:\n";
    for (int i = 0; i < m; i++) {
        int l;
        cin >> l;
        set<string> lhs;
        while (l--) {
            string a;
            cin >> a;
            lhs.insert(a);
        }
        int r;
        cin >> r;
        set<string> rhs;
        while (r--) {
            string a;
            cin >> a;
            rhs.insert(a);
        }
        F.emplace_back(lhs, rhs);
    }

    // 1NF assumed always true (atomicity)
    bool is2NF = true, is3NF = true, isBCNF = true;
    set<string> prime = PK;

    // Check 2NF: no partial dependency of non-prime on proper subset of PK
    if (PK.size() > 1) {
        for (const auto& fd : F) {
            const auto& lhs = fd.first;
            const auto& rhs = fd.second;
            if (!lhs.empty() && lhs.size() < PK.size() &&
                includes(PK.begin(), PK.end(), lhs.begin(), lhs.end())) {
                for (const auto& a : rhs) {
                    if (!prime.count(a)) {
                        is2NF = false;
                        break;
                    }
                }
            }
            if (!is2NF) break;
        }
    }

    // Check 3NF: for each FD X->A, X is superkey or A is prime
    for (const auto& fd : F) {
        const auto& lhs = fd.first;
        const auto& rhs = fd.second;
        if (!isSuperkey(lhs, F, allAttrs)) {
            for (const auto& a : rhs) {
                if (!prime.count(a)) {
                    is3NF = false;
                    break;
                }
            }
        }
        if (!is3NF) break;
    }
    is3NF = is2NF && is3NF;

    // Check BCNF: for each FD X->A, X must be superkey
    for (const auto& fd : F) {
        const auto& lhs = fd.first;
        if (!isSuperkey(lhs, F, allAttrs)) {
            isBCNF = false;
            break;
        }
    }
    isBCNF = is3NF && isBCNF;

    // Output highest normal form
    cout << "The highest normal form is: ";
    if      (isBCNF) cout << "BCNF\n";
    else if (is3NF)  cout << "3NF\n";
    else if (is2NF)  cout << "2NF\n";
    else             cout << "1NF\n";

    return 0;
}

