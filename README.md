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

```c
int debug_file_open(const char* filepath)
```

```c
void debug_file_close(void)
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

## [file.h](file/)

```c
char* path_clean(char* path)
```

```c
size_t file_size_get(const char* filepath)
```

```c
size_t file_read(void* pointer, size_t size, const char* filepath)
```

```c
size_t file_write(const void* pointer, size_t size, const char* filepath)
```

```c
int file_remove(const char* filepath)
```

```c
int file_rename(const char* old_filepath, const char* new_filepath)
```

```c
int files_get(char*** files, size_t* count, const char* path, int depth)
```

```c
size_t files_size_get(char** files, size_t count)
```

```c
size_t files_read(void* pointer, size_t size, char** files, size_t count)
```

```c
void files_free(char** files, size_t count)
```

```c
size_t dir_file_size_get(const char* dirpath, const char* name)
```

```c
size_t dir_file_write(const void* pointer, size_t size, const char* dirpath, const char* name)
```

```c
size_t dir_file_read(void* pointer, size_t size, const char* dirpath, const char* name)
```

```c
int dir_file_remove(const char* dirpath, const char* name)
```

```c
int dir_file_rename(const char* dirpath, const char* old_name, const char* new_name)
```

## socket.h

## thread.h

## base64.h
