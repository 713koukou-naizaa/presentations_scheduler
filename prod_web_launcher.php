<?php

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

$jsonDirPath = "./json";

$binaryFile = './bin/main';

$args = [
    $startMorningTime,
    $endMorningTime,
    $startAfternoonTime,
    $endAfternoonTime,
    $normalPresentationLength,
    $accommodatedPresentationLength,
    $inBetweenBreakLength,
    $maxTeachersWeeklyWorkedTime,
    $jsonDirPath
];

$cmd = $binaryFile . " " . implode(" ", $args);

echo "Executing:" . $cmd;

exec($cmd, $output, $status);

if ($status != 0)
{
    foreach ($output as $line) { echo $line . "\n"; }
    echo "Error: " . $status . "\n";
    exit(1);
}

foreach ($output as $line) { echo $line . "\n"; }