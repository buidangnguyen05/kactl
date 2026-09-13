/**
 * Author: awk
 * Description: Count number of primes up to n.
 * Time: O(\sqrt{n})
 */

#pragma once

ll piSieve(const ll n) {
    if (n <= 1) return 0LL; if (n == 2) return 1LL;
    const int lim = int(sqrt(n));
    int vsz = (lim + 1) >> 1;
    vector<int> smalls(vsz), roughs(vsz), larges(vsz);
    rep(i,0,vsz) smalls[i]=i, roughs[i]=(i<<1|1), larges[i]=(n/(i<<1|1)-1)>>1;
    vector<bool> skips(lim+1,false);
    int pCnt=0;
    for(int p=3;p<=lim;p+=2){
        if (skips[p]) continue;
        int p2 = p * p;
        if (1LL * p2 * p2 > n) break;
        skips[p] = true;
        for(int cx = p2; cx <= lim; cx += (p << 1)) skips[cx] = true;
        int ns = 0;
        rep(cz, 0, vsz) {
            int cur = roughs[cx];
            if(skips[cur]) continue;
            ll d = 1LL * cur * p;
            larges[ns]=larges[cx]-(d<=lim?larges[smalls[d>>1]-pCnt]
                :smalls[(ll((double(n)/d)-1))>>1])+pCnt;
            roughs[ns++]=cur;
        }
        vsz=ns;
        for(int cx=(lim-1)>>1,cy=((lim/p)-1)|1;cy>=p;cy-=2){
            int cur=smalls[cy>>1]-pCnt;
            for(int cz=(cy*p)>>1;cz<=cx;--cx) smalls[cx]-=cur;
        }
        ++pCnt;
    }
    larges[0]+=1LL*(vsz+((pCnt-1)<<1))*(vsz-1)>>1;
    for(int cx=1;cx<vsz;++cx) larges[0]-=larges[cx];
    for(int cx=1;cx<vsz;++cx){
        int q=roughs[cx];
        ll m=n/q;
        int e=smalls[((m/q)-1)>>1]-pCnt;
        if(e<cx+1) break;
        ll t=0;
        for(int cy=cx+1;cy<=e;++cy) t+=smalls[ll((double(m)/roughs[cy])-1)>>1];
        larges[0]+=t-1LL*(e-cx)*(pCnt+cx-1);
    }
    return larges[0]+1;
}