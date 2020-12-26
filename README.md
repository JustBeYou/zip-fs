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

## Coding style
Please prefix any public function with `zipfs_<submodule>_` to avoid confusion. (unfortunately there are no namespaces in C) Try to maintain a consistent coding style with the existing code, use snake case naming, braces on the same line and 4 spaces indentation. Write short and clear code. 

## Testing
Please use `valgrind` to check for memory leaks.
