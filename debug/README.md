# debug.h

## Usage

Every source file that wants to use debug.h should include the file.
```c
#include "debug.h"
```

But, **only one** of the `.c` files (preferably the one with the `main` function) should define the implementation.
```c
#define DEBUG_IMPLEMENT
```

This is how it could look like in the source file with the main function.
```c
#define DEBUG_IMPLEMENT
#include "debug.h"
```

## Functions
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

## Hash (2024-12-05)

```
073f1572f3ac70f7fcf45a7fecb19525c872938a9eadc61cd25ad441cf800a63  debug.h
```

## Programs

### [bunker](https://github.com/hfridholm/bunker)

### [procom](https://github.com/hfridholm/procom)

### [korsord](https://github.com/hfridholm/korsord)
