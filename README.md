# zip-fs

## How to build?
Make sure you have installed:
- cmake >= 17.0.0
- fuse >= 3.0.0 (recommended to build from repo)
- gcc

Run the following commands:
```bash
# Configuration stage, only once
mkdir -p Debug && cd Debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
# Actually building
make
```

## Testing
Please use `valgrind` to check for memory leaks.
