#include <Rcpp.h>
#include <string.h>
using namespace Rcpp;

class StrBuffer {
public:
    char *buf;
    int size;
    const int default_length = 256;
    StrBuffer() {
        buf = new char[default_length];
        size = default_length;
    }
    ~StrBuffer() { delete[] buf; }
    void copy(const char *s, int b, int len) {
        if (len > size) {
            delete[] buf;
            size = len;
            buf = new char[size];
        }
        strncpy(buf,&s[b],len);
        buf[len] = '\0';
    }
};

// [[Rcpp::export]]
CharacterVector fastSplitC(String s, String delim) {
     int b = 0;
     const char *cs = s.get_cstring();
     int slen = strlen(cs);
     const char *cdelim = delim.get_cstring();
     char c = *cdelim;
     bool *begin = new bool[slen];
     int *substrlen = new int[slen];
     int nelem = 0;
     int i;
     for (i = 0; i < slen; i++) {
        begin[i] = false;
        substrlen[i] = 0;
     }
     for (i = 0; i < slen; i++) {
        if (cs[i] == c) {
            begin[b] = true;
            substrlen[b] = i-b;
            b = i+1;
            nelem++;
        }
     }
     if (b < i) {
        begin[b] = true;
        substrlen[b] = i-b;
        nelem++;
     }
     StrBuffer buf;
     CharacterVector v(nelem);
     int n = 0;
     for (i = 0; i < slen; i += substrlen[i]+1) {
        buf.copy(cs,i,substrlen[i]);
        v(n) = buf.buf;
        n++;
     } 
     delete[] begin;
     delete[] substrlen;
     return v;
}
