# Oppgave 4

- First error is `(MYHTTP *)malloc(sizeof(PMYHTTP));` it allocates only 8 bytes for the struct by using the PMYHTTP typedef and not MYHTTP witch is not a pointer variant.

- Seccond error is that `strcpy(pHttp->szContentType, pszPtr);` is not limited by how long the array is in the struct, and can overflow.

- Third error is that `pHttp->iContentLength = '0' + atoi(pszPtr);` is adding (char) '0' to a int, and makes the content Length wrong with adding 48 to it witch is the ascii code for '0'.

Also had some segmentation fault with this code ``strchr(pszPtr, '\n')[0] = 0; ``, don't know why but changed it to use `strcspn()` with `strncpy()` instead.