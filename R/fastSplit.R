#' Splits the input string using the delimiter character.
#' If 
#' @param s A string
#' @param delim A delimiter string, which should be no more than one character.
#' @param encoding Encoding of the input string and the delimiter. 
#'        If encoding is not "UTF-8", the strings are converted into UTF-8,
#'        split into vectors, and then converted back to the string
#'        (which makes the processing slower)
#' @return A character vecter
#' @export
#' @importFrom utils localeToCharset
fastSplit <- function(s,delim,encoding="") {
  if (length(s) == 0) {
    error("fastSplit: input string is empty")
  }
  if (encoding == "")
    encoding = utils::localeToCharset()
  if (encoding != "UTF-8" && encoding != "ASCII") {
    s <- iconv(s,from=encoding,to="UTF-8")
    delim <- iconv(delim,from=encoding,to="UTF-8")
  }
  if (length(s) > 1) {
    warning("only the first element is split")
    s <- s[1]
  }
  if (nchar(delim) == 0) {
    res <- fastSplitZ(s)
  } else {
    if (nchar(delim,type="chars") > 1) {
      res <- fastSplitM(s,delim)
    } else if (nchar(delim,type="bytes") > 1) {
      res <- fastSplitM(s,delim)
    } else {
      res <- fastSplitC(s,delim)
    }
  }
  if (encoding != "UTF-8" && encoding != "ASCII") {
    res <- iconv(res,from="UTF-8",to=encoding)
  }
  res
}
