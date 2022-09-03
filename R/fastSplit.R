#' Splits the input string using the delimiter character.
#' @param s A string
#' @param delim A delimiter string, which should be no more than one character.
#' @return A character vecter
#' @export
fastSplit <- function(s,delim) {
  if (length(s) == 0) {
    error("fastSplit: input string is empty")
  }
  if (length(s) > 1) {
    warning("fastSplit: only the first element is split")
    s <- s[1]
  }
  if (nchar(delim) == 0) {
    return(fastSplitZ(s))
  }
  if (nchar(delim) > 1) {
    warning("fastSplit; only the first character of delim will be used as the delimiter")
    delim <- substr(delim,1,1)
  }
  fastSplitC(s,delim)
}
