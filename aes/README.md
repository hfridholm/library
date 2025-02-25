# aes.h

## Usage

Every source file that wants to use aes.h should include the file.
```c
#include "aes.h"
```

But, **only one** of the `.c` files (preferably the one with the `main` function) should define the implementation.
```c
#define AES_IMPLEMENT
```

This is how it could look like in the source file with the main function.
```c
#define AES_IMPLEMENT
#include "aes.h"
```

## Functions

```c
int aes_encrypt(uint8_t** result, size_t* rsize, const void* message, size_t msize, const void* key, ksize_t ksize)
```

```c
int aes_decrypt(uint8_t** result, size_t* rsize, const void* message, size_t msize, const void* key, ksize_t ksize)
```

## Hash (2025-02-25)

```
ec127aefcf672dad0fe44218c4cdf93fe948ff1846b04445a5e6a801d7e3262b  aes.h
```

## Programs

### [crypto](https://github.com/hfridholm/crypto)

### [passman](https://github.com/hfridholm/passman)
