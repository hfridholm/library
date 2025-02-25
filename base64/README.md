# base64.h

## Usage

Every source file that wants to use base64.h should include the file.
```c
#include "base64.h"
```

But, **only one** of the `.c` files (preferably the one with the `main` function) should define the implementation.
```c
#define BASE64_IMPLEMENT
```

This is how it could look like in the source file with the main function.
```c
#define BASE64_IMPLEMENT
#include "base64.h"
```

## Functions

```c
int base64_encode(char** result, size_t* rsize, const void* message, size_t msize)
```

```c
int base64_decode(char** result, size_t* rsize, const void* message, size_t msize)
```

## Hash (2024-12-06)

```
04f7a9db3ed531eeddd03eab51eb4641dda91a06fe40bd923a9cd4413e941938  base64.h
```

## Programs

### [crypto](https://github.com/hfridholm/crypto)
