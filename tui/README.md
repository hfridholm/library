# tui.h

## Usage

Every source file that wants to use tui.h should include the file.
```c
#include "tui.h"
```

But, **only one** of the `.c` files (preferably the one with the `main` function) should define the implementation.
```c
#define TUI_IMPLEMENT
```

This is how it could look like in the source file with the main function.
```c
#define TUI_IMPLEMENT
#include "tui.h"
```

## Functions

```c

```

## Hash (2025-05-01)

```
1cfd1dafaf7154e86b450e00d60fcca76fea254d301daee07d03a2579811cbe9  tui.h
```

## Programs

### [stocks](https://github.com/hfridholm/stocks)
