# gui.h

## Usage

Every source file that wants to use gui.h should include the file.
```c
#include "gui.h"
```

But, **only one** of the `.c` files (preferably the one with the `main` function) should define the implementation.
```c
#define GUI_IMPLEMENT
```

This is how it could look like in the source file with the main function.
```c
#define GUI_IMPLEMENT
#include "gui.h"
```

## Functions

```c

```

## Hash (2025-05-01)

```
08a3124e631eb089a7c5121d73c32b6148c0b475402d95cd41aad951ca5d6e27  gui.h
```

## Programs

### [gui](https://github.com/hfridholm/gui)
