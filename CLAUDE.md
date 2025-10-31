# vibecc
This is a simple C compiler written by C aiming to be self-hosted.
We does not aim to be a full-featured compiler.

## Commands
### Build
To set up the build environment, run:
```bash
cmake -B build
```

### Test
To run the tests, run:
```bash
make -C build
make test -C build -j$(nproc 2>/dev/null || sysctl -n hw.ncpu)
```

## Plans for Development
Implement features incrementally.
For example, create a compiler that can only parse and emit assembly for simple arithmetic expressions such as `1 + 2`.
Then, add support for other arithmetic expressions, variables, functions, and control flow.

On implementing a new feature, create a specification for the feature in the specs/ directory.

## Notes for Implementation
- Do not care about freeing memory; the compiler is not intended for long-running processes.
