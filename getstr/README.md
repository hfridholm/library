# getstr.h

## Usage

Every source file that wants to use getstr.h should include the file.
```c
#include "getstr.h"
```

But, **only one** of the `.c` files (preferably the one with the `main` function) should define the implementation.
```c
#define GETSTR_IMPLEMENT
```

This is how it could look like in the source file with the main function.
```c
#define GETSTR_IMPLEMENT
#include "getstr.h"
```

## Functions

```c
char* getstr(const char* format, ...)
```

## Hash (2024-12-05)

```
71d21b54d1f8211f24ec7986aa03b1644da1717819613e1be916473d350ec173  getstr.h
```

## Programs

### [bunker](https://github.com/hfridholm/bunker)
