<?php

declare(strict_types=1);

class PrimeNumber {
    private array $prime_numbers = [];
    private function is_prime_number(int $n): bool {
        foreach ($this->prime_numbers as $np) {
            if ($np * $np > $n)
                return true;
            if ($n % $np == 0)
                return false;
        }
        return true;
    }

    public function calc(int $upper_limit): void {

        for ($n = 2; $n <= $upper_limit; $n++) {
            if ($this->is_prime_number($n))
                $this->prime_numbers[] = $n;
        }

        echo "Count: " . count($this->prime_numbers) . "\n";
    }

    public function show(): void {
        foreach ($this->prime_numbers as $pn)
            echo $pn . "\n";
    }

}

function parseArg(): array {

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
$pn->calc((int)$args["upper_limit"]);
if ($args["show"])
    $pn->show();
