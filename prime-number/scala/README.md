# How to setup to run the program with GraalVM

- Download a GraalVM archive and extract it on a directory.
- Create a symbolic link to the above extracted GraalVM directory with the name `graalvm` in this directory.


# How to create a fat jar

    sbt assembly


# Examples of run-class-path.sh

```
export CLASSPATH=target/scala-3.2.2/scala-assembly-0.1.0-SNAPSHOT.jar
```
