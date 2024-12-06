# Library

This repository is a collection of the libraries I have created.

## [debug.h](debug/)
```c
int debug_print(FILE* stream, const char* title, const char* format, ...)
```

```c
int error_print(const char* format, ...)
```

```c
int info_print(const char* format, ...)
```

## [getstr.h](getstr/)
```c
char* getstr(const char* format, ...)
```

## [sha256.h](sha256/)

```c
char* sha256(char hash[64], const void* message, size_t size)
```

## [md5.h](md5/)

```c
char* md5(char hash[64], const void* message, size_t size)
```

## socket.h

## file.h

## thread.h

## base64.h
