<?php

function generateTeachers(int $count = 15): array
{
    $teachers = [];
    for ($i = 0; $i < $count; $i++) {
        $name = sprintf("Teacher_%02d", $i);
        $teachers[] = [
            "id" => $i,
            "name" => $name,
            "isTechnical" => (mt_rand(0, 1) === 1),
            "weeklyRemainingMinutes" => 1200
        ];
    }
    return $teachers;
}

function generateStudents(int $count = 60, int $teacherCount = 15): array
{
    $students = [];
    for ($i = 0; $i < $count; $i++) {
        $name = sprintf("Student_%02d", $i);
        $students[] = [
            "id" => $i,
            "name" => $name,
            "hasAccommodations" => (mt_rand(1, 4) === 1),
            "referentTeacherId" => mt_rand(0, max(0, $teacherCount - 1)),
            "tutorId" => $i
        ];
    }
    return $students;
}

function generateRooms(int $count = 8): array
{
    $rooms = [];
    for ($i = 0; $i < $count; $i++) {
        $rooms[] = [
            "id" => $i,
            "tag" => sprintf("Room_%02d", $i)
        ];
    }
    return $rooms;
}

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

$binaryFile = './bin/main';

$args = [
    $startMorningTime,
    $endMorningTime,
    $startAfternoonTime,
    $endAfternoonTime,
    $normalPresentationLength,
    $accommodatedPresentationLength,
    $inBetweenBreakLength,
    $maxTeachersWeeklyWorkedTime
];

$cmd = $binaryFile . ' ' . implode(' ', $args);

echo "Executing:" . $cmd . "\n";

// Arrays to send to stdin as JSON
$stdinTeachers = generateTeachers();
$stdinStudents = generateStudents(60, count($stdinTeachers));
$stdinRooms = generateRooms();

$stdinContent = json_encode($stdinStudents) . "\n"
              . json_encode($stdinTeachers) . "\n"
              . json_encode($stdinRooms) . "\n";

$descriptorSpecs = [
    0 => ['pipe', 'r'], // stdin
    1 => ['pipe', 'w'], // stdout
    2 => ['pipe', 'w']  // stderr
];

$process = proc_open($cmd, $descriptorSpecs, $pipes);

if (!is_resource($process))
{
    echo "Error: failed to start process\n";
    exit(1);
}

// Write JSON input in stdin and close it
fwrite($pipes[0], $stdinContent);
fclose($pipes[0]);

// Read stdout and stderr
$stdout = stream_get_contents($pipes[1]);
fclose($pipes[1]);

$stderr = stream_get_contents($pipes[2]);
fclose($pipes[2]);

$returnStatus = proc_close($process);

if ($returnStatus != 0)
{
    if (strlen($stdout) > 0) { foreach (preg_split("/\r\n|\n|\r/", trim($stdout)) as $line) { echo $line . "\n"; } }
    if (strlen($stderr) > 0) { foreach (preg_split("/\r\n|\n|\r/", trim($stderr)) as $line) { echo $line . "\n"; } }
    echo "Error: " . $returnStatus . "\n";
    exit(1);
}

if (strlen($stdout) > 0) { foreach (preg_split("/\r\n|\n|\r/", trim($stdout)) as $line) { echo $line . "\n"; }
}
