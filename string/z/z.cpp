namespace Z
{
    // 0-based, O(|S|)
    // Z[i] = LCP(S[i...], S)
    // get_z(S = "ababacaca") = [ -, 0, 3, 0, 1, 0, 1, 0, 1 ]
    vector<int> get_z(string S)
    {
        int N=S.size();
        vector<int> A(N);
        for(int i=1, j=0; i<N; i++)
        {
            if(j+A[j]>i) A[i]=min(j+A[j]-i, A[i-j]);
            while(i+A[i]<N && S[A[i]]==S[i+A[i]]) A[i]++;
            if(i+A[i]>j+A[j]) j=i;
        }
        return A;
    }
}