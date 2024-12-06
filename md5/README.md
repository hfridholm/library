# md5.h

## Usage

Every source file that wants to use md5.h should include the file.
```c
#include "md5.h"
```

But, **only one** of the `.c` files (preferably the one with the `main` function) should define the implementation.
```c
#define MD5_IMPLEMENT
```

This is how it could look like in the source file with the main function.
```c
#define MD5_IMPLEMENT
#include "md5.h"
```

## Functions

```c
char* md5(char hash[64], const void* message, size_t size)
```

## Hash (2024-12-06)

```
760ad3739f367663aed4619968ceb3cd557034aac57d41dfddd7f270929ef062  md5.h
```

## Programs

### [hashing](https://github.com/hfridholm/hashing)
