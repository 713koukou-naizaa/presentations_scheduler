# Presentations Scheduler

A C++ scheduling component developed for the university web project `Gestion de suivi des stages` (see [GitHub repository link](https://github.com/nzoEtd/suivi-stage)) during my third year of my computer science bachelor's degree (BUT Informatique). The scheduler implements a greedy scheduling algorithm (see [Algorithm outline markdown file](docs/algorithm_outlines.md)) and is designed to be invoked from PHP for web integration. The implementation favors compiled C++ for runtime performance and reduced energy consumption.

## Highlights
- Implemented in modern C++ for speed and low runtime overhead.
- Intended to be called from a PHP web backend.
- Includes a greedy scheduling algorithm and utilities for rooms, teachers, tutors and students.

## Build
A helper wrapper `exec.sh` is available for consistent build and invocation:
```bash
chmod +x exec.sh
./exec.sh
```

If you do not wish to use the provided shell script, from the project root:
```bash
make
```
This produces the executable (by default placed in) `bin/main`.

To clean:
```bash
make clean
```

## Run
After building, run the scheduler binary:
```bash
./bin/main
```

## Integration with PHP
Use `exec` or `shell_exec` to call `exec.sh` or `bin/main` from PHP and capture stdout/stderr.

## Project structure
- `main.cpp` — program entry point
- `Makefile` — build rules
- `exec.sh` — helper wrapper to build and invoke the scheduler
- `bin/` — compiled binary output (`bin/main`)
- `src/` — implementation files (`Room.cpp`, `Scheduler.cpp`, `Teacher.cpp`, `Tutor.cpp`, `Utils.cpp`)
- `include/` — public headers (`Presentation.h`, `Room.h`, `Scheduler.h`, `Student.h`, `Teacher.h`, `Tutor.h`, `Utils.h`, `globalConsts.h`)
- `docs/` — algorithm outline and related documentation

## Notes
- The binary is intended to be light-weight and fast; prefer invoking it as an external process from the PHP web app rather than embedding heavy runtimes.
- Check `docs/algorithm_outlines.md` and `docs/greedy_scheduling_algorithm_algoforge.json` for algorithm details and rationale.