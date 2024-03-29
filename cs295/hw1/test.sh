#!/usr/local/bin/bash
#---------------------
#
# Implements a test of the e95_scanner program.
#------------------------------------------------


cat <<EOF;
#-------------------------
#   Keyword Test
#-------------------------
EOF
for reserved_word in do for return break    \
                     short else goto signed \
		     unsigned char if void  \
		     int continue long while
do
   echo ${reserved_word} | ./scanner | egrep -i "reserved word" 1>> /dev/null
   if [ "$?" = "0" ]
   then
       printf "%15s: %10s\n" ${reserved_word} success
   else
       printf "%15s: %10s\n" ${reserved_word} failure
   fi
done





printf "\n\n\n\n\n";
cat <<EOF;
#-------------------------
#   Operator test
#-------------------------
EOF
echo -n "!"   | ./scanner
echo -n "%"   | ./scanner
echo -n "^"   | ./scanner
echo -n "&"   | ./scanner
echo -n "*"   | ./scanner
echo -n "-"   | ./scanner
echo -n "+"   | ./scanner
echo -n "="   | ./scanner
echo -n "~"   | ./scanner
echo -n "|"   | ./scanner
echo -n "<"   | ./scanner
echo -n ">"   | ./scanner
echo -n "/"   | ./scanner
echo -n "?"   | ./scanner
echo -n "+="  | ./scanner
echo -n "-="  | ./scanner
echo -n "*="  | ./scanner
echo -n "/="  | ./scanner
echo -n "%="  | ./scanner
echo -n "<<=" | ./scanner
echo -n ">>=" | ./scanner
echo -n "&="  | ./scanner
echo -n "^="  | ./scanner
echo -n "|="  | ./scanner
echo -n "++"  | ./scanner
echo -n "--"  | ./scanner
echo -n "<<"  | ./scanner
echo -n ">>"  | ./scanner
echo -n "<="  | ./scanner
echo -n ">="  | ./scanner
echo -n "=="  | ./scanner
echo -n "&&"  | ./scanner
echo -n "||"  | ./scanner




printf "\n\n\n\n\n";
cat <<EOF;
#-------------------------
#   Separator test
#-------------------------
EOF
echo -n "("   | ./scanner
echo -n ")"   | ./scanner
echo -n "["   | ./scanner
echo -n "]"   | ./scanner
echo -n "{"   | ./scanner
echo -n "}"   | ./scanner
echo -n ","   | ./scanner
echo -n ";"   | ./scanner
echo -n ":"   | ./scanner




printf "\n\n\n\n\n";
cat <<EOF;
#-------------------------
#   Constant test
#-------------------------
EOF
echo -n "2147483647" | ./scanner
echo -n "2147483648" | ./scanner
echo -n "4294967294" | ./scanner
echo -n "4294967295" | ./scanner
echo -n "4294967299" | ./scanner
echo -n "\"this is a test\"" | ./scanner
echo -n "\"this is a tab\ttest\"" | ./scanner
echo -n "'\n'" | ./scanner
echo -n "'\t'" | ./scanner
echo -n "'\b'" | ./scanner
echo -n "'\r'" | ./scanner
echo -n "'\f'" | ./scanner
echo -n "'\v'" | ./scanner
echo -n "'\a'" | ./scanner
printf "\n\n\n\n\n";





