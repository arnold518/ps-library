// include mint (Ultimate Mod)
struct Hash
{
    // 0-based, constructor : O(|S|), get_hash : O(1)
    // get(l, r) = hash for S[l ... r] (inclusive), 0 if l>r
    int N;
    mint B;
    vector<mint> P, IP, V;
    Hash(string S, mint _B)
    {
        B=_B;
        N=S.size();
        P=IP=V=vector<mint>(N+1);
        P[0]=IP[0]=1;
        P[1]=B; IP[1]=inv(B);
        for(int i=2; i<=N; i++) P[i]=P[i-1]*P[1], IP[i]=IP[i-1]*IP[1];
        for(int i=1; i<=N; i++) V[i]=V[i-1]+P[i]*S[i-1];
    }
    mint get_hash(int l, int r) { return (l<=r ? (V[r+1]-V[l])*IP[l] : mint(0)); } 
};