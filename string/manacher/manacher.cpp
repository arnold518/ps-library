namespace Manacher
{
    // 0-based, manacher : O(|S|)
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
