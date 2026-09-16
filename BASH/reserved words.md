
#### For loop Syntax:

```bash
if cond1; then
#bloc1
elif cond2; then
#bloc2
else
#bloc3
fi
```

```bash 
time command
```

```bash
for var in elem1 elem2 elem3; do
#code
done
```

```bash 
for (( expr1 ; expr2 ; expr3 )); do 
#code
done
```

```bash

case $var in 
pattern1) ;;
pattern2|pattern3) ;;
*) ;;


#aussi parler de ;& et ;;&
```

```bash
function func {
#code
}

func() {
#ocde
}
```

```bash
select choice in c1 c2 c3;do
#code $REPLY , $@
break
done
```

```bash 
coproc command 
```

```bash
{ 
#bloc1
 }
 
 #advance conditionnal test
 # (support logical operators)
 [[condition]] 
 

```

```bash
while cond; do 
#code
done
```


```bash
until cond; do 
#code
done
```


