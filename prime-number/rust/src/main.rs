use std::env;
use num_traits::PrimInt;
use num_traits::NumCast;

#[derive(Debug)]
enum VarType {
    I32,
    I64,
}

struct Parameter {
    upper_limit: u32,
    var_type: VarType,
}

fn is_prime_number<T: PrimInt>(n: T, prime_numbers: &Vec<T>) -> bool {
    for _pn in prime_numbers {
        let pn: T = *_pn;
        if pn * pn > n {
            break;
        }
        if n % pn == T::zero() {
            return false;
        }
    }
    return true;
}

fn calc<T: PrimInt>(param: Parameter) {
    let mut prime_numbers: Vec<T> = Vec::new();
    for _n in 2..param.upper_limit {
        let n: T = NumCast::from(_n).unwrap();
        if is_prime_number(n, &prime_numbers) {
            prime_numbers.push(n);
        }
    }
    println!("Count: {}", prime_numbers.len());
}

fn parse_arg(args: Vec<String>) -> Parameter {
    let mut param = Parameter {
        upper_limit: 100,
        var_type: VarType::I32,
    };

    let mut idx = 1;
    while idx < args.len() {
        if args[idx] == "-u" {
            idx += 1;
            param.upper_limit = args[idx].parse().unwrap();
        } else if args[idx] == "--i64" {
            param.var_type = VarType::I64;
        }
        idx += 1;
    }
    return param;
}

fn main() {
    let args: Vec<String> = env::args().collect();
    let param = parse_arg(args);
    println!("var_type: {:?}", param.var_type);
    match param.var_type {
        VarType::I32 => calc::<i32>(param),
        VarType::I64 => calc::<i64>(param),
    }
}
