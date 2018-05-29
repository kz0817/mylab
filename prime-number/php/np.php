<?php

class PrimeNumber {

   function __construct() {
       $this->prime_numbers = [];
   }

   function is_prime_number($n) {
        foreach ($this->prime_numbers as $np) {
            if ($np * $np > $n)
                return true;
            if ($n % $np == 0)
                return false;
        }
        return true;
    }

    function calc($upper_limit) {

        for ($n = 2; $n <= $upper_limit; $n++) {
            if ($this->is_prime_number($n))
                $this->prime_numbers[] = $n;
        }

        echo "Count: " . count($this->prime_numbers) . "\n";
    }

    function show() {
        foreach ($this->prime_numbers as $pn)
            echo $pn . "\n";
    }

}

function parseArg() {

    $opt = [
        "upper_limit" => 0,
        "show" => false,
    ];

    global $argv;
    $next_parser = null;
    for ($i = 1; $i < count($argv); $i++) {
        $arg = $argv[$i];
        if ($next_parser) {
            $next_parser($arg, $opt);
            $next_parser = null;
            continue;
        }
        if ($arg == "-u" || $arg == "--upper-limit") {
            $next_parser = function($arg, &$opt) {
                $opt["upper_limit"] = $arg;
            };
        }
        else if ($arg == "-s" || $arg == "--show") {
            $opt["show"] = true;
        }
        else {
            echo "Warning: unprocessed: " . $arg . "\n";
        }
    }

    return $opt;
}


$args = parseArg();

$pn = new PrimeNumber();
$pn->calc($args["upper_limit"]);
if ($args["show"])
    $pn->show();

?>
