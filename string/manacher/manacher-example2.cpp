// https://codeforces.com/contest/1951/problem/E

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

const int MAXN = 2e6;

int TC;
int N;
char S[MAXN+10];

namespace Manacher
{
    // 0-based, O(|S|)
    // S[i-P[i] ... i+P[i]] is palindrome
    // manacher(S = "abcbcba") = [ 0, 0, 1, 3, 1, 0, 0 ]
    vector<int> manacher(string S)
    {
        int N=S.size();
        vector<int> A(N);
        for(int i=0, j=0; i<N; i++)
        {
            if(j+A[j]>i) A[i]=min(A[j+j-i], j+A[j]-i);
            while(i+A[i]<N && i>=A[i] && S[i+A[i]]==S[i-A[i]]) A[i]++;
            if(i+A[i]>j+A[j]) j=i;
        }
        for(int i=0; i<N; i++) A[i]--;
        return A;
    }

    // 0-based, init : O(|S|), is_palindrome : O(1)
    // call init(S) first
    // is_palindrome(l, r) = true iff S[l ... r] is palindrome
    // 0 <= l <= r < |S| is necessary
    vector<int> V;
    void init(string S)
    {
        string S2="!";
        for(auto c : S) S2+=string(1, c)+"!";
        V=manacher(S2);
    }
    bool is_palindrome(int l, int r) { return V[l+r+1]>=(r-l+1); }
}

bool f(int l, int r) { return Manacher::is_palindrome(l-1, r-1); }

int main()
{
    scanf("%d", &TC);
    while(TC--)
    {
        scanf(" %s", S+1);
        N=strlen(S+1);

        Manacher::init(string(S+1, S+N+1));

        if(!f(1, N))
        {
            printf("YES\n1\n");
            printf("%s\n", S+1);
            continue;
        }
        bool flag=false;
        for(int i=1; i<N; i++)
        {
            if(!f(1, i) && !f(i+1, N))
            {
                printf("YES\n2\n");
                for(int j=1; j<=i; j++) printf("%c", S[j]);
                printf(" ");
                for(int j=i+1; j<=N; j++) printf("%c", S[j]);
                printf("\n");
                flag=true;
                break;
            }
        }
        if(!flag) printf("NO\n");

    }
}