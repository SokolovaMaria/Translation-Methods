# Грамматика
1. `S -> var ARR`
2. `ARR -> NAMES : ARRTYPE; ARR`
3. `ARR -> \epsilon`
4. `NAMES -> NT, NAMES`
5. `NAMES -> NT`
6. `ARRTYPE -> array [DIM] of ARRTYPE' `
7. `ARRTYPE' -> ARRTYPE `
8. `ARRTYPE' -> NT `
9. `DIM -> N .. N, DIM `
10. `DIM -> N .. N `
11. `N -> [0-9]N `
12. `NT -> [a-z,A-Z,0-9]NT `


## Устранение правового ветвления:

1. `S -> var ARR`
2. `ARR -> NAMES : ARRTYPE; ARR`
3. `ARR -> \epsilon`
4. `NAMES -> NT NAMES' `
5. `NAMES' -> , NAMES`
6. `NAMES' -> \epsilon`
7. `ARRTYPE -> array [DIM] of ARRTYPE' `
8. `ARRTYPE' -> ARRTYPE `
9. `ARRTYPE' -> NT `
10. `DIM -> N .. N DIM' `
11. `DIM' -> , DIM `
12. `DIM -> \epsilon `
13. `N -> [0-9]N `
14. `NT -> [a-z,A-Z,0-9]NT `

## FIRST и FOLLOW  для нетерминалов. 

`c` - [A-Za-z] <br>
`n` - [1 - 9]

Нетерминал | FIRST    | FOLLOW
-----------|----------|-------
S          | `var`      |`$`
ARR         | `c` `\epsilon`     |`$`
NAMES  | `c`|`,` `:`
NAMES'    | `,` `\epsilon`     |`:`
ARRTYPE   | `array`|`;`
ARRTYPE'       | `array` `c`      |`;`
DIM       | `n`      |`..``]`
DIM'       | `,` `\epsilon`      |`]`
N       | `n`      |`..``]`
NT       | `c`      |`,` `:` `;`
