# Presentations Scheduler

A C++ scheduling component developed for the university web project `Gestion de suivi des stages` (see [GitHub repository link](https://github.com/nzoEtd/suivi-stage)) during my third year of my computer science bachelor's degree (BUT Informatique). The scheduler implements a greedy scheduling algorithm (see [Algorithm outline markdown file](docs/algorithm_outlines.md)) and is designed to be invoked from PHP for web integration. The implementation favors compiled C++ for runtime performance and reduced energy consumption.

## Highlights
- Implemented in modern C++ for speed and low runtime overhead.
- Intended to be called from a PHP web backend.
- Includes a greedy scheduling algorithm and utilities for rooms, teachers, tutors and students.

## Generate dummy JSON data
The [generateDummyJsonData.py](generateDummyJsonData.py) file generates dummy JSON data for teachers, tutors, students and rooms.

```bash
chmod +x generateDummyJsonData.py
python generateDummyJsonData.py
```

## Use releases to deploy binary
If only the binary file is going to be run, you can download it from the releases page, here is an example (replace <verion> with the latest one (i.e. v1.0.1)).

```bash
#!/bin/bash

curl -L -o bin/presentations_scheduler \
  https://github.com/713koukou-naizaa/presentations_scheduler/releases/download/<version>/main
chmod +x bin/presentations_scheduler
```

## Integration with PHP
Use `exec`, `shell_exec` or `proc_open` to call [run.sh](run.sh) or directly `bin/main` from PHP and capture stdout/stderr. See the [prod_web_launcher.php](prod_web_launcher.php) file using exec as an example.

```bash
chmod +x prod_web_launcher.php
php prod_web_launcher.php
```

## Development example run
The [simulate_dev_run.sh](simulate_dev_run.sh) file simulates a development run of the binary file (make using [Makefile](Makefile), call to the run shell script and call to the binary file).

```bash
chmod +x simulate_dev_run.sh
./simulate_dev_run.sh
```

## Production example run
The [simulate_prod_run.sh](simulate_prod_run.sh) file simulates a production run of the binary file (call to the php web launcher, call to the run shell script and call to the binary file).

```bash
chmod +x simulate_prod_run.sh
./simulate_prod_run.sh
```

## Project structure
- `bin` - compiled binary output
- `docs` documentations (algorithm outline, related documentation, ...)
- `include/` - public headers (`config.h`, `json.hpp`, `Presentation.h`, `Room.h`, `Scheduler.h`, `Student.h`, `Teacher.h`, `Tutor.h`, `Utils.h`)
- `json` - JSON files (`teachers.json`, `tutors.json`, `students.json`, `rooms.json`), created after dummy data generation
- `src/` — implementation files (`Room.cpp`, `Scheduler.cpp`, `Teacher.cpp`, `Tutor.cpp`, `Utils.cpp`)
- `.gitignore` - git ignore file
- `download_bin.sh` - download binary file example
- `generateDummyJsonData.py` - generate dummy JSON data
- `Makefile` - build rules
- `main.cpp` - program entry point
- `prod_web_launcher.php` - simulated php production run script
- `README.md` - this file
- `run.sh` - run the binary file
- `simulate_dev_run.sh` - simulate a development run (make using Makefile, run php web launcher, run binary file)
- `simulate_prod_run.sh` - simulate a production run script (run php web launcher, run binary file)

## Notes
- The binary is intended to be light-weight and fast; prefer invoking it as an external process from the PHP web app rather than embedding heavy runtimes.
- Check `docs/algorithm_outlines.md` and `docs/greedy_scheduling_algorithm_algoforge.json` for algorithm details and rationale.