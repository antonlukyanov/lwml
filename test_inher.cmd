@echo off

lgrep "^class" #.cc;#.h >a

cat a | grep -v "public scope" | grep -v "class scope" | lred a
cat a | grep -v "public interface"  | grep -v "class interface" | lred a
cat a | grep -v "public hardcore" | grep -v "class hardcore" | lred a
cat a | grep -v "public value"  | grep -v "class value" | lred a
cat a | grep -v "public refcount"  | grep -v "class refcount" | lred a

cat a | grep -v "class minicv {"  | lred a
cat a | grep -v "public lcg32"  | lred a
cat a | grep -v "public lcg10"  | lred a
