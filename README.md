# fastSplitR

This package provides a simple and fast string split function. 
The delimiter should be a fixed string. If the delimiter is a null string or a one single-byte character, it calls a special internal function to make the processing faster.

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
> fastSplit("This is a pen","is")
[1] "Th"     " "      " a pen"
```

It is much faster than `strsplit()` or `str_split()`.

For long string
```{r}
> library(microbenchmark)
> library(stringr)
> s <- paste(rep("x",10000),collapse=" ")
> microbenchmark({dummy<-strsplit(s," ")[[1]]}, unit="ms")
Unit: milliseconds
                                   expr    min      lq     mean median      uq    max neval
 {     dummy <- strsplit(s, " ")[[1]] } 5.6742 5.99055 6.231967 6.2401 6.40085 7.5045   100
> microbenchmark({dummy<-str_split(s," ")[[1]]}, unit="ms")
Unit: milliseconds
                                    expr    min     lq     mean median      uq     max neval
 {     dummy <- str_split(s, " ")[[1]] } 0.5006 0.5196 0.755711 0.5418 0.56405 20.0474   100
> microbenchmark({dummy<-fastSplit(s," ",encoding="ASCII")}, unit="ms")
Unit: milliseconds
                                                   expr    min      lq     mean median      uq    max
 {     dummy <- fastSplit(s, " ", encoding = "ASCII") } 0.4156 0.45985 0.479001 0.4666 0.47775 0.7854
 neval
   100
```

However, since `fastSplit` converts the input string into UTF-8, split the string, and converts the split vector back to the original encoding, it becomes slower when no encoding is specified.

```{r}
> microbenchmark({dummy<-fastSplit(s," ")}, unit="ms")
Unit: milliseconds
                               expr    min     lq     mean  median     uq   max neval
 {     dummy <- fastSplit(s, " ") } 2.5191 2.7031 2.848691 2.77825 2.8977 4.312   100
```

For short string
```{r}
> s <- paste(rep("abcdefg",10),collapse=" ")
> microbenchmark({dummy<-strsplit(s," ")}, unit="us")
Unit: microseconds
                              expr  min   lq   mean median   uq  max neval
 {     dummy <- strsplit(s, " ") } 14.3 14.5 15.678  14.75 15.9 45.2   100
> microbenchmark({dummy<-str_split(s," ")}, unit="us")
Unit: microseconds
                               expr  min   lq   mean median   uq  max neval
 {     dummy <- str_split(s, " ") } 13.8 14.2 16.538  14.65 16.3 81.7   100
> microbenchmark({dummy<-fastSplit(s," ",encoding="ASCII")}, unit="us")
Unit: microseconds
                                                   expr min   lq   mean median   uq  max neval
 {     dummy <- fastSplit(s, " ", encoding = "ASCII") }  10 10.4 11.512   10.7 11.1 56.9   100
```
