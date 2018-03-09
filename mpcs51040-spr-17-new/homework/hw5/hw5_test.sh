#!/bin/bash

# find compiler
CC=`which gcc`

if test -z $CC ; then
   echo "Could not find gcc! Is this linux.cs.uchicago.edu?"
   exit 1
fi

echo "Checking filenames"

for a in arb_test.c arb_int.c arb_int.h
do
   if ! test -f "$a" ; then
      echo "File $a missing! (Please see hw assignment!)"
      exit 1
   fi
done

echo "All OK!"
echo

echo ============================================
echo Testing arb_int.c with simple program
echo ============================================
$CC -ggdb -Wall -Werror -std=c11 -pedantic -o arb_simple arb_simple.c arb_int.c -lm
if test $? != 0; then
   echo ============================================
   echo "Compilation errors or warnings!"
   echo ============================================
   exit 1
fi

echo ============================================
echo "Testing for memory leak and other errors..."
echo ============================================
valgrind --error-exitcode=1 --leak-check=full ./arb_simple
if test $? != 0; then
      echo ============================================
      echo "Errors detected! See output above"
      echo ============================================
      exit 1
fi

echo ============================================
echo "Compiling your test program..."
echo ============================================

$CC -ggdb -Wall -Werror -std=c11 -pedantic -o arb_test arb_test.c arb_int.c -lm
if test $? != 0; then
   echo ============================================
   echo "Compilation errors or warnings!"
   echo ============================================
   exit 1
fi

echo "All OK!"
echo


echo ============================================
echo "Testing for memory leak and other errors..."
echo ============================================
for a in + - \*
do
   valgrind --error-exitcode=1 --leak-check=full ./arb_test 1 "$a" 1
   if test $? != 0; then
      echo ============================================
      echo "Errors detected! See output above"
      echo ============================================
      exit 1
   fi
done

echo ============================================
echo Testing if math is correct
echo ============================================

for a in $(seq 1 10)
do
   for S1 in "-" ""
   do
      for S2 in "-" ""
      do
         num1="${S1}${RANDOM}${RANDOM}${RANDOM}"
         num2="${S1}${RANDOM}${RANDOM}${RANDOM}"
         for op in + -  \*
         do
            correct=`echo "$num1 $op $num2" | bc`
            echo -n "$num1" "$op" "$num2" "=>"
            arbint=`./arb_test "$num1" "\$op" $num2`
            if test "A$correct" == "A$arbint" ; then
               echo " OK :-) ($correct)"
            else
               echo " Error: got $arbint, expected $correct"
               exit 1
            fi
         done
      done
   done
done

echo ============================================
echo All tests passed! Well done!
echo ============================================


