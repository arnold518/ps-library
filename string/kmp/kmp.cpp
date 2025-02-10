namespace KMP
{
    // 0-based, kmp : O(|S|)
    // kmp(S = "ababacaca") = [ -, 0, 1, 2, 3, 0, 1, 0, 1 ]
    vector<int> kmp(string S)
    {
        int N=S.size();
        vector<int> A(N);
        for(int i=1; i<N; i++)
        {
            int j=A[i-1];
            while(j>0 && S[j]!=S[i]) j=A[j-1];
            if(S[j]==S[i]) A[i]=j+1;
        }
        return A;
    }
}
