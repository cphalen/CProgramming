


int somefunc(int a)
{
   // NOTE: a below is NOT the function parameter a above -- the a below purely refers to the
   // first argument of the VAROP macro.
#define VAROP(a,b) b a b

   // The a below becomes bound to parameter b in the expansion of the macro
   // (in other words, within the macro above, b = the character 'a')
   return VAROP(+,a);
}

