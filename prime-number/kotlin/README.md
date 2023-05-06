# How to setup to run the program with GraalVM

- Download a GraalVM archive and extract it on a directory.
- Create a symbolic link to the above extracted GraalVM directory with the name `graalvm` in this directory.

# How to run

## Example to set the upper bound to calculate

    ./gradlew run --args="-u 100"

## Example to show calculated prime numbers

    ./gradlew run --args="-u 1000 -s"
