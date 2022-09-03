# fastSplitR

This package provides a simple and fast string split function.  It takes only one string and one character as the delimiter.

```{r}
library(fastSplitR)
> fastSplit("This is a pen"," ")
[1] "This" "is"   "a"    "pen" 
> fastSplit("This is a pen","")
[1] "T" "h" "i" "s" " " "i" "s" " " "a" " " "p" "e" "n"
> fastSplit(c("This is a pen","I am a boy")," ")
[1] "This" "is"   "a"    "pen" 
Warning message:
In fastSplit(c("This is a pen", "I am a boy"), " ") :
  fastSplit: only the first element is split
> fastSplit("This is a pen"," /.")
[1] "This" "is"   "a"    "pen" 
Warning message:
In fastSplit("This is a pen", " /.") :
  fastSplit; only the first character of delim will be used as the delimiter
```

It is much faster than `strsplit()` or `str_split()`.

For long string
```{r}
> library(microbenchmark)
> s <- paste(rep("x",10000),collapse=" ")
> microbenchmark({dummy<-strsplit(s," ")[[1]]}, unit="ms")
Unit: milliseconds
                                   expr    min      lq     mean median    uq    max neval
 {     dummy <- strsplit(s, " ")[[1]] } 4.1164 4.23435 4.536519 4.4333 4.651 5.6672   100
> library(stringr)
> microbenchmark({dummy<-str_split(s," ")[[1]]}, unit="ms")
Unit: milliseconds
                                    expr    min      lq     mean median      uq    max neval
 {     dummy <- str_split(s, " ")[[1]] } 0.6004 0.63215 0.682874  0.636 0.65825 1.3368   100
> microbenchmark({dummy<-fastSplit(s," ")}, unit="ms")
Unit: milliseconds
                                expr    min      lq     mean  median      uq    max neval
 {     dummy <- fastSplitC(s, " ") } 0.5806 0.61345 0.673073 0.61845 0.66305 1.3246   100
```

For short string
```{r}
> s <- paste(rep("abcdefg",10),collapse=" ")
> microbenchmark({dummy<-strsplit(s," ")}, unit="us")
Unit: microseconds
                              expr min lq  mean median   uq  max neval
 {     dummy <- strsplit(s, " ") } 8.9  9 9.723    9.1 10.4 27.6   100
> microbenchmark({dummy<-str_split(s," ")}, unit="us")
Unit: microseconds
                               expr  min    lq   mean median    uq  max neval
 {     dummy <- str_split(s, " ") } 14.1 14.35 18.286   14.6 15.95 90.8   100
> microbenchmark({dummy<-fastSplit(s," ")}, unit="us")
Unit: microseconds
                               expr min  lq  mean median  uq  max neval
 {     dummy <- fastSplit(s, " ") } 6.6 6.8 8.216      7 7.5 62.9   100
```