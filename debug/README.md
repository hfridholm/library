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
b3faccb0077f45764c8c33f79e30bb040489dafc2fa66901b2a8322db7abdee2  debug.h
```

## Programs

### [bunker](https://github.com/hfridholm/bunker)

### [procom](https://github.com/hfridholm/procom)

### [korsord](https://github.com/hfridholm/korsord)
