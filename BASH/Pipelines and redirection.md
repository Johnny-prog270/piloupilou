
| \|           | stdout pipeline        |
| ------------ | ---------------------- |
| \|&          | stdout+stderr pipeline |
| > b          | stdout -> b            |
| >> b         | stdout -> b (append)   |
| 2> b         | stderr -> b            |
| < b          | stdin -> b             |
| 2>&1         | stderr -> 1            |
| >/dev/null   | stdout -> blackhole    |
| >&3          | stdout -> 3            |
| 2> /dev/null | delete stderr          |
| N>&-         | delete N               |
| exec 3> b    | 3 -> b                 |
| &> b         | stderr + stdout ->  b  |
