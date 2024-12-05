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

## Hash (2024-12-05)

```
178eaf1db476927db99a61a6fbba442308f8e1aca83bdafb799e925ab2adab60  debug.h
```

## Programs

### bunker

### procom
