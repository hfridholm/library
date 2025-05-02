# socket.h

## Usage

Every source file that wants to use socket.h should include the file.
```c
#include "socket.h"
```

But, **only one** of the `.c` files (preferably the one with the `main` function) should define the implementation.
```c
#define SOCKET_IMPLEMENT
```

This is how it could look like in the source file with the main function.
```c
#define SOCKET_IMPLEMENT
#include "socket.h"
```

## Functions

```c

```

## Hash (2025-05-01)

```
80648d264805780476bd0039a9fac0604c662ac1a16581c4a362a4b3a05b088b  socket.h
```

## Programs

### [procom](https://github.com/hfridholm/procom)
