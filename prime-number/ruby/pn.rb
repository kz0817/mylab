#!/usr/bin/env ruby

ArgStruct = Struct.new(:upper_limit, :show)

def is_prime_number(n, prime_numbers)
  prime_numbers.each do |pn|
    if pn * pn > n
      break
    end
    if n % pn == 0
      return false
    end
  end
  true
end

def run(args)
  prime_numbers = [2]
  3.step(args.upper_limit, 2) do |n|
    if is_prime_number(n, prime_numbers)
      prime_numbers.push(n)
    end
  end

  puts "Count: #{prime_numbers.length}"
  if args.show
    prime_numbers.each do |pn|
      puts pn
    end
  end
end

def parse_args()
  args = ArgStruct.new({ upper_limit: 100, show: false })

  idx = 0
  while idx < ARGV.length do
     argv = ARGV[idx]
     if argv == '-u'
       idx += 1
       args.upper_limit = ARGV[idx].to_i
     end

     if argv == '-s'
       args.show = true
     end

     idx += 1
  end

  args
end

def start()
  args = parse_args()
  puts "Upper limit: #{args.upper_limit}"
  run(args)
end

start
