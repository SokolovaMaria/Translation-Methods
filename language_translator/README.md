## Usage

A program, written in a primitive functional language, is translated to C++.<br>
[***ANTLR***](http://www.antlr.org/) is used as parser generator.<br>
Run:<br>
```
~mvn package
~java -jar target/language_translator-1.0-SNAPSHOT-jar-with-dependencies.jar 
```
Program from **resources/example.txt** will be translated.