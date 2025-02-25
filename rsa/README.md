# rsa.h

## Usage

Every source file that wants to use rsa.h should include the file.
```c
#include "rsa.h"
```

But, **only one** of the `.c` files (preferably the one with the `main` function) should define the implementation.
```c
#define RSA_IMPLEMENT
```

This is how it could look like in the source file with the main function.
```c
#define RSA_IMPLEMENT
#include "rsa.h"
```

## Functions

```c
void rsa_keys_gen(skey_t* skey, pkey_t* pkey)
```

```c
int rsa_encrypt(void* result, size_t* rsize, const void* message, size_t size, pkey_t* key)
```

```c
int rsa_decrypt(void* result, size_t* rsize, const void* message, size_t size, skey_t* key)
```

```c
int rsa_skey_encode(char** result, size_t* size, const skey_t* key)
```

```c
int rsa_skey_decode(skey_t* key, const void* message, size_t size)
```

```c
int rsa_pkey_encode(char** result, size_t* size, const pkey_t* key)
```

```c
int rsa_pkey_decode(pkey_t* key, const void* message, size_t size)
```

```c
void rsa_keys_free(skey_t* skey, pkey_t* pkey)
```

```c
void rsa_skey_free(skey_t* key)
```

```c
void rsa_pkey_free(pkey_t* key)
```

## Hash (2025-02-25)

```
681dbdce8d5e8cdaaf504d4de0714c2f28832454cb45e214233279a3c033ac25  rsa.h
```

## Programs

### [crypto](https://github.com/hfridholm/crypto)
