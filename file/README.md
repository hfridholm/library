# file.h

## Usage

Every source file that wants to use file.h should include the file.
```c
#include "file.h"
```

But, **only one** of the `.c` files (preferably the one with the `main` function) should define the implementation.
```c
#define FILE_IMPLEMENT
```

This is how it could look like in the source file with the main function.
```c
#define FILE_IMPLEMENT
#include "file.h"
```

## Functions

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

## Hash (2025-02-14)

```
f01550770c799e91f0dd73112bcc2aa65d15a6a616192c702f7961a5930171b8  file.h
```

## Programs

### [hashing](https://github.com/hfridholm/hashing)

### [crypto](https://github.com/hfridholm/crypto)

### [bunker](https://github.com/hfridholm/bunker)

### [korsord](https://github.com/hfridholm/korsord)

### [passman](https://github.com/hfridholm/passman)
