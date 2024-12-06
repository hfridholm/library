# sha256.h

## Usage

Every source file that wants to use sha256.h should include the file.
```c
#include "sha256.h"
```

But, **only one** of the `.c` files (preferably the one with the `main` function) should define the implementation.
```c
#define SHA256_IMPLEMENT
```

This is how it could look like in the source file with the main function.
```c
#define SHA256_IMPLEMENT
#include "sha256.h"
```

## Functions

```c
char* sha256(char hash[64], const void* message, size_t size)
```

## Hash (2024-12-06)

```
8c3b181193fa6acbeafa014882a63f63af86bf7106d4412967ceebba41c04359  sha256.h
```

## Programs

### [hashing](https://github.com/hfridholm/hashing)

### [crypto](https://github.com/hfridholm/crypto)

### [passman](https://github.com/hfridholm/passman)
