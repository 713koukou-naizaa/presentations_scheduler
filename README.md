# Presentations Scheduler

A C++ scheduling component developed for the university web project `Gestion de suivi des stages` (see [GitHub repository link](https://github.com/nzoEtd/suivi-stage)) during my third year of my computer science bachelor's degree (BUT Informatique). The scheduler implements a greedy scheduling algorithm (see [Algorithm outline markdown file](docs/algorithm_outlines.md)) and is designed to be invoked from PHP for web integration. The implementation favors compiled C++ for runtime performance and reduced energy consumption.

## Highlights
- Implemented in modern C++ for speed and low runtime overhead.
- Intended to be called from a PHP web backend.
- Includes a greedy scheduling algorithm and utilities for rooms, teachers, tutors and students.

## Use releases to deploy binary
If only the binary file is going to be run, you can download it from the releases page, here is an example:
```bash
curl -L -o bin/main https://github.com/713koukou-naizaa/presentations_scheduler/releases/download/v1.0.0/main
chmod +x bin/main
```
Le fichier est ensuite prêt à être exécuté.

## Build
A helper wrapper `exec.sh` is available for consistent invocation:
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
Use `exec` or `shell_exec` to call `exec.sh` or `bin/main` from PHP and capture stdout/stderr. Here is an example with exec:
```php
<?php
// File web_launcher.php

$startMorningHour = 7;
$endMorningHour = 12;
$startAfternoonHour = 14;
$endAfternoonHour = 17;

$maxTeachersWeeklyWorkedHours = 20;

$startMorningTime = $startMorningHour * 60;
$endMorningTime = $endMorningHour * 60 + 30;
$startAfternoonTime = $startAfternoonHour * 60;
$endAfternoonTime = $endAfternoonHour * 60;
$normalPresentationLength = 1 * 60;
$accommodatedPresentationLength = $normalPresentationLength + 20;
$inBetweenBreakLength = 5;
$maxTeachersWeeklyWorkedTime = $maxTeachersWeeklyWorkedHours * 60;
$nbStudents = 60;
$nbTeachers = 15;
$nbRooms = 8;
$nbTutors = $nbStudents;

$cmd = "./exec.sh $startMorningTime $endMorningTime $startAfternoonTime $endAfternoonTime $normalPresentationLength $accommodatedPresentationLength $inBetweenBreakLength $maxTeachersWeeklyWorkedTime $nbStudents $nbTeachers $nbRooms $nbTutors";

exec($cmd, $output, $status);

if ($status != 0)
{
    echo "Error: " . $status . "\n";
    exit(1);
}

foreach ($output as $line) { echo $line . "\n"; }
```

```bash
chmod +x web_launcher.php
php web_launcher.php
```

## Project structure
- `main.cpp` — program entry point
- `Makefile` — build rules
- `exec.sh` — helper wrapper to invoke the scheduler
- `bin/` — compiled binary output (`bin/main`)
- `src/` — implementation files (`Room.cpp`, `Scheduler.cpp`, `Teacher.cpp`, `Tutor.cpp`, `Utils.cpp`)
- `include/` — public headers (`Presentation.h`, `Room.h`, `Scheduler.h`, `Student.h`, `Teacher.h`, `Tutor.h`, `Utils.h`, `globalConsts.h`)
- `docs/` — algorithm outline and related documentation

## Notes
- The binary is intended to be light-weight and fast; prefer invoking it as an external process from the PHP web app rather than embedding heavy runtimes.
- Check `docs/algorithm_outlines.md` and `docs/greedy_scheduling_algorithm_algoforge.json` for algorithm details and rationale.