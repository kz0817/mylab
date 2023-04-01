use std::env;

struct Parameter {
    upper_limit: u32
}

fn is_prime_number(n: u32, prime_numbers: &Vec<u32>) -> bool {
    for pn in prime_numbers {
        if pn * pn > n {
            break;
        }
        if n % pn == 0 {
            return false;
        }
    }
    return true;
}

fn calc(param: Parameter) {
    let mut prime_numbers: Vec<u32> = Vec::new();
    for n in 2..param.upper_limit {
        if is_prime_number(n, &prime_numbers) {
            prime_numbers.push(n);
        }
    }
    println!("Count: {}", prime_numbers.len());
}

fn parse_arg(args: Vec<String>) -> Parameter {
    let mut param = Parameter { upper_limit: 100 };

    let mut idx = 1;
    while idx < args.len() {
        if args[idx] == "-u" {
            idx += 1;
            param.upper_limit = args[idx].parse().unwrap();
        }
        idx += 1;
    }
    return param;
}

fn main() {
    let args: Vec<String> = env::args().collect();
    let param = parse_arg(args);
    calc(param)
}
