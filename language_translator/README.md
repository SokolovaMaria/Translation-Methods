## Usage

A program, written in a primitive functional language, is translated to C++.<br>
[***ANTLR***](http://www.antlr.org/) is used as parser generator.<br>
Run:<br>
```
~mvn package
~java -jar target/language_translator-1.0-SNAPSHOT-jar-with-dependencies.jar 
```
Program from **resources/example** will be translated.

## Example
**Input:**

```ocaml
let func1 a, b =
    let func2 c =
        let func3 d, e, f =
            let func4 g, g1 =
                a / g1 in
            func4 (d + e, func4(e, f)) in
        func3(c, func3(k, t, func3(12, 41, c)), func4(225, 6)) in
    func2 (a + b);;    
    
val func : int -> string -> bool -> int -> int
let func n1, str, b, n2 =
    (n1 * n2) + n1;;   
```

**Output:**
```c
auto func1 = [] (auto arg0, auto arg1) {
    auto func2 = [] (auto arg2) {
        auto func3 = [] (auto arg3, auto arg4, auto arg5) {
            auto func4 = [] (auto arg6, auto arg7) {
                return arg0/arg7;
            };
            return func4(arg3+arg4, func4(arg4, arg5));
        };
        return func3(arg2, func3(k, t, func3(12, 41, arg2)), func4(225, 6));
    };
    return func2(arg0+arg1);
};

int func (int arg0, string arg1, bool arg2, int arg3) {
    return (arg0*arg3)+arg0;
}
```