²## File navigation

```bash
ls
ls -l
ls -a
ls -li
ls path
```

```bash
ln file1 file2
ln -s file1 file2
```

```bash 
cd ~
cd path
cd dir
cd ..
cd -
```

```bash 
pwd
```

```bash
cat file
cat file1 file2
cat > file
```

```bash
echo text
echo -e text
```

```bash 
wc file 
wc -l file

```

```bash
less file
```

```bash
more file
```

```bash
grep pattern file
grep -r pattern dir
grep -i pattern file
grep -n pattern file
grep -E "pattern"
```

```bash
find dir -name pattern 
find . -type f

```

```bash
head -n 10 file.txt
head -c 10 file.txt
tail -n 10 file.txt
```

```bash
file -b file
```

```bash
stat -c "format" file
```


## File modification 

```bash
cp src dest
cp -r srcdir srcdst
cp -i src dest
```

```bash 
mv old new 
mv old path
mv -i old new
```

```bash
rm file
rm -r file
rm -i file 
rm -f file
```

```bash 
mkdir dir 
mkdir -p path
```

```bash
sort file.txt
sort -r file.txt
sort -n file.txt
```

## process

```date
date +"%Y-%m-%d %H:%M:%S"
date +%s%1N
```

```bash
wait pid
wait %1
```

```bash
top
htop
```

```bash
ps 
ps -e
```

```bash
setsid command
```

```bash
kill process
kill -SIG process 
kill -l
```

## variables 

```bash
which executable
```

```bash
command ls 
command -V ls
command -v ls
\ls
```

```bash
where executable
```

```bash
uniq file.txt
```

```bash
cut -c a-b file.txt
cut -c a,b file.txt
cut -sd "," -f a data.csv
```

```bash
alias new = "command"
```

```bash
unalias new
unalias -a
```

```bash
source ~/.bashrc
. ~/.bashrc

```


## Variables managment

```bash
set
set -e -u -x +e -o
```

```bash
env 
env -i command
env python3 script.py
```

```bash
printenv PATH
```

```bash
declare/typeset -r
```

```bash
export VAR
```

```bash
unset VAR
```

```bash
readonly VAR
```

## compressed files

```bash
zip archive.zip f1 f2 dir1
unzip archive.zip
unzip -l archive.zip 
```

```bash
gzip -k archive.gzip f1 f2 dir1
ungzip archive.zip
ungzip -l archive.zip 
```

```bash
xz archive.xz
unxz archive.xz
```

```bash
tar -cf archive.tar f1 f2
tar -xf archive.tar
tar -tf archive.tar 
tar -czf archive.tar.gz f1 f2
tar -xzf archive.tar.gz
tar -tzf archive.tar .gz
tar -cJf archive.tar.xz f1 f2
tar -xJf archive.tar.xz
tar -tJf archive.tar.xz 
```

## permissions and users

```bash
chmod u+x script.sh
chmod g-w fichier.txt
chmod a+r notes.txt
chmod u=rwx,g=rx,o=r
chmod 755 script.sh
```

```bash
chown alice document.txt
chown alice:developpeurs script.sh
chown :www-data index.html
chown -R ubuntu:www-data /var/www
chown --reference=modele.txt cible.txt
```

```bash 
sudo apt update
sudo -i
sudo -u nom_utilisateur commande


```

```bash
exit
```

```bash
su 
su - 
su alice
su - alice
su -c "ls /root" root
```


```bash
adduser name
addgroup name
usermod -aG group user
passwd user
userdel -r user
groupdel group
```

```bash
test command
```

```bash
read -p promtpt var
read -s
read -d ";"
```

```bash
trap 'command' SIGNAL
trap - SIGINT
```

```bash
exec command
exec 3>output.log
```

```bash
tee -a output.log
```