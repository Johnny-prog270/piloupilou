
```bash
fruits=("pomme" "banane" "cerise")
```

```bash
${fruits[N]}   
${fruits[@]}    
${fruits[*]}    
${!fruits[@]}   
${fruits[@]:A:B} 
${#fruits[@]}   
${#fruits[N]}    
```

```bash
tableau[N] = new               -
tableau += (new)               
unset fruits[N]               
fruits = ()                    
fruits2 = ("${fruits[@]}")    
fruits2 = ("${a[@]}" "${b[@]}") 
```

```bash 
matrix[0,0]=1
matrix[0,1]=2
matrix[1,0]=3
matrix[1,1]=4

echo "${matrix[1,0]}"  # 3
```

### Dictionaries
```bash
declare -A capitales

capitales=( ["France"]="Paris" ["Italie"]="Rome" )
```

```bash
capitales["clé"]="valeur"
${capitales["clé"]}
${!capitales[@]}
${capitales[@]}
${#capitales[@]}

unset mon_dico["clé"]
unset mon_dico
```
