# Presentations Scheduler

A C++ scheduling component developed for the university web project `Gestion de suivi des stages` (see [GitHub repository link](https://github.com/nzoEtd/suivi-stage)) during my third year of my computer science bachelor's degree (BUT Informatique). The scheduler implements a greedy scheduling algorithm (see [Algorithm outline Markdown file](docs/algorithm_outlines.md)) and is designed to be invoked from PHP for web integration. The implementation favors compiled C++ for runtime performance and reduced energy consumption.

## Highlights

- Implemented in modern C++ for speed and low runtime overhead.
- Intended to be called from a PHP web backend.
- Includes a greedy scheduling algorithm.
- Reads data from stdin, and returns the output into stdout.

## Tests

Unit tests have been made in the `tests` directory, the latest coverage report is available in `coverage-report`.

## Generate dummy JSON data

Methods in the [prod_web_launcher.php](prod_web_launcher.php) file generates dummy JSON data for teachers, students and rooms.

## Use releases to deploy binary

If only the binary file is going to be run, you can download it from the releases page, here is an example (replace <version> with the latest one (i.e. v1.0.1)).

```bash
#!/bin/bash

curl -L -o bin/presentations_scheduler \
  https://github.com/713koukou-naizaa/presentations_scheduler/releases/download/<version>/main
chmod +x bin/presentations_scheduler
```

## Integration with PHP

Use `exec`, `shell_exec` or `proc_open` to call directly `bin/main` from PHP and capture stdout/stderr. See the [prod_web_launcher.php](prod_web_launcher.php) file using `proc_pen` as an example.

```bash
chmod +x prod_web_launcher.php
php prod_web_launcher.php
```

## Project structure

```
.
├── bin/
│   └── (compiled binaries)
├── coverage-report/
│   └── (latest test coverage output)
├── docs/
│   └── (algorithm outline, related documentation, ...)
├── include/
│   ├── config.h
│   ├── json.hpp
│   ├── Presentation.h
│   ├── Room.h
│   ├── Scheduler.h
│   ├── Student.h
│   ├── Teacher.h
│   └── Utils.h
├── src/
│   ├── config.cpp
│   ├── Room.cpp
│   ├── Scheduler.cpp
│   ├── Student.cpp
│   ├── Teacher.cpp
│   └── Utils.cpp
├── tests/
│   ├── SchedulerTest.cpp
│   ├── TeacherTest.cpp
│   └── UtilsTest.cpp
├── .gitignore
├── Makefile
├── main.cpp
└── README.md
```

## Notes
- The binary is intended to be light-weight and fast; prefer invoking it as an external process from the PHP web app rather than embedding heavy runtimes.
- Check `docs/algorithm_outlines.md` and `docs/greedy_scheduling_algorithm_algoforge.json` for algorithm details and rationale.
- The code is compiled the code statically to ensure the binary file has all the libraries it needs to run once on the server.
- The binary file reads JSON formatted input data from stdin and sends JSON formatted output data to stdout
- The JSON data have to be sent into stdin in the order used in [prod_web_launcher.php](prod_web_launcher.php).