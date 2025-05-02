# stock.h

## Usage

Every source file that wants to use stock.h should include the file.
```c
#include "stock.h"
```

But, **only one** of the `.c` files (preferably the one with the `main` function) should define the implementation.
```c
#define STOCK_IMPLEMENT
```

This is how it could look like in the source file with the main function.
```c
#define STOCK_IMPLEMENT
#include "stock.h"
```

## Functions

```c

```

## Hash (2025-05-01)

```
9a186c55ca8d146a51215de5874a67a22a3e1f706d1a2dbd18ab1f68d7b9fb39  stock.h
```

## Programs

### [stocks](https://github.com/hfridholm/stocks)
