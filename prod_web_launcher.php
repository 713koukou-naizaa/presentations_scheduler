<?php

function generateTeachers(int $count = 15): array
{
    $teachers = [];
    for ($i = 0; $i < $count; $i++) { $teachers[] = ["id" => $i, "isTechnical" => (mt_rand(0, 1) === 1), "weeklyRemainingMinutes" => 1200]; }
    return $teachers;
}

function generateStudents(int $count = 60, int $teacherCount = 15): array
{
    $students = [];
    for ($i = 0; $i < $count; $i++) { $students[] = ["id" => $i, "hasAccommodations" => (mt_rand(1, 4) === 1), "referentTeacherId" => mt_rand(0, max(0, $teacherCount - 1))]; }
    return $students;
}

function generateRooms(int $count = 8): array
{
    $rooms = [];
    for ($i = 0; $i < $count; $i++) { $rooms[] = ["id" => $i, "tag" => sprintf("Room_%02d", $i)]; }
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
$stdinStudents = generateStudents();
$stdinRooms = generateRooms();

$stdinContent = json_encode($stdinStudents) . "\n"
              . json_encode($stdinTeachers) . "\n"
              . json_encode($stdinRooms) . "\n";

$stdinContent = '[{"id":583303,"hasAccommodations":false,"referentTeacherId":6,"tutorId":4},{"id":610000,"hasAccommodations":false,"referentTeacherId":2,"tutorId":1},{"id":610001,"hasAccommodations":false,"referentTeacherId":6,"tutorId":2},{"id":610459,"hasAccommodations":false,"referentTeacherId":6,"tutorId":5},{"id":611107,"hasAccommodations":false,"referentTeacherId":2,"tutorId":4},{"id":613453,"hasAccommodations":false,"referentTeacherId":7,"tutorId":6},{"id":641110,"hasAccommodations":false,"referentTeacherId":5,"tutorId":6},{"id":641353,"hasAccommodations":false,"referentTeacherId":2,"tutorId":2},{"id":641387,"hasAccommodations":false,"referentTeacherId":5,"tutorId":7},{"id":700005,"hasAccommodations":false,"referentTeacherId":4,"tutorId":6},{"id":700006,"hasAccommodations":false,"referentTeacherId":1,"tutorId":7},{"id":700008,"hasAccommodations":false,"referentTeacherId":6,"tutorId":2},{"id":700009,"hasAccommodations":false,"referentTeacherId":4,"tutorId":2},{"id":700010,"hasAccommodations":false,"referentTeacherId":2,"tutorId":2},{"id":700012,"hasAccommodations":false,"referentTeacherId":6,"tutorId":2},{"id":700014,"hasAccommodations":false,"referentTeacherId":5,"tutorId":6},{"id":700021,"hasAccommodations":false,"referentTeacherId":1,"tutorId":1},{"id":700022,"hasAccommodations":false,"referentTeacherId":1,"tutorId":3},{"id":700023,"hasAccommodations":false,"referentTeacherId":3,"tutorId":2},{"id":700026,"hasAccommodations":false,"referentTeacherId":7,"tutorId":7},{"id":700028,"hasAccommodations":false,"referentTeacherId":1,"tutorId":3},{"id":700031,"hasAccommodations":false,"referentTeacherId":1,"tutorId":1},{"id":700036,"hasAccommodations":false,"referentTeacherId":7,"tutorId":7},{"id":700038,"hasAccommodations":false,"referentTeacherId":4,"tutorId":5},{"id":700041,"hasAccommodations":false,"referentTeacherId":6,"tutorId":2},{"id":700047,"hasAccommodations":false,"referentTeacherId":2,"tutorId":2},{"id":700048,"hasAccommodations":false,"referentTeacherId":1,"tutorId":4},{"id":700049,"hasAccommodations":false,"referentTeacherId":7,"tutorId":4}]
                [{"id":1,"isTechnical":true},{"id":2,"isTechnical":true},{"id":3,"isTechnical":true},{"id":4,"isTechnical":true},{"id":5,"isTechnical":true},{"id":6,"isTechnical":true},{"id":7,"isTechnical":true}]
                [{"id":124},{"id":125},{"id":126},{"id":127},{"id":129},{"id":131}]';

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

if (strlen($stdout) > 0) { foreach (preg_split("/\r\n|\n|\r/", trim($stdout)) as $line) { echo $line . "\n"; } }
