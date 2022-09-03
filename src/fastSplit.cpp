#include <Rcpp.h>
#include <R.h>
#include <string.h>
using namespace Rcpp;

extern void Rf_error(const char *, ...);
#define error Rf_error

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

static int utf_bytes(char c) {
  if ((c & 0x80) == 0) return 1;
  if ((c & 0xe0) == 0xc0) return 2;
  if ((c & 0xf0) == 0xe0) return 3;
  if ((c & 0xf0) == 0xf0) return 4;
  return -1;
}

// delim should be 1-byte character
// [[Rcpp::export]]
CharacterVector fastSplitC(String s, String delim) {
     int b = 0;
     const char *cs = s.get_cstring();
     int slen = strlen(cs);
     const char *cdelim = delim.get_cstring();
     char c = *cdelim;
     if (utf_bytes(c) != 1) {
       error("Delimiter should be 1-byte character");
     }
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

// character-by-character split
// [[Rcpp::export]]
CharacterVector fastSplitZ(String s) {
  const char *cs = s.get_cstring();
  int slen = strlen(cs);
  char buf[5];
  int *charlen = new int[slen];
  for (int i = 0; i < slen; i++) charlen[i] = 0;
  int nc = 0;
  for (int i = 0; i < slen;) {
    int clen = utf_bytes(cs[i]);
    if (clen < 0) {
      error("Invalid UTF-8 sequence");
    }
    charlen[i] = clen;
    i += clen;
    nc++;
  }
  CharacterVector v(nc);
  int i = 0;
  int n = 0;
  while (i < slen) {
    int clen = charlen[i];
    for (int j = 0; j < clen; j++)
      buf[j] = cs[i+j];
    buf[i+clen] = '\0';
    v(n) = buf;
    i += clen;
    n++;
  } 
  return v;
}
