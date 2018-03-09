/*********************************************************
 *  Unit tests for varstring (also used for grading!)
 *
 *  DO NOT MODIFY
 *********************************************************/

#include "varstring.h"

#include <CUnit/Basic.h>

#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <stdbool.h>
#include <malloc.h>
#include <time.h>
#include <ctype.h>

// For certain tests, COUNT^2 (binary ops) are tested
#define COUNT 10


// Largest number generated for the test (in digits) *before*
// performing operation.
#define VERY_LARGE 80


#define MAX(a,b) ((a) > (b) ? (a) : (b))


/* The suite initialization function.
 * Opens the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
static int init_suite(void)
{
   return 0;
}

/* The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
static int clean_suite(void)
{
   return 0;
}

static void random_destroy(varstring_handle * handles, size_t count)
{
   // Free some and check the others
   unsigned int alive = count;
   while(alive)
   {
      unsigned int victim = rand() % alive;
      assert(handles[victim]);

      // just to see if we crash
      varstring_length(handles[victim]);

      varstring_free(handles[victim]);
      if (victim != (alive-1))
      {
         handles[victim]=handles[alive-1];
         handles[alive-1] = 0;
      }
      --alive;
   }
}

static void test_varstring_new()
{
   // Allocate a bunch and free in random order
   const size_t count = COUNT;
   varstring_handle * handles = malloc(count * sizeof(varstring_handle));

   for (unsigned int i=0; i<count; ++i)
   {
      handles[i] = varstring_new();
      CU_ASSERT_PTR_NOT_NULL(handles[i]);
   }

   random_destroy(handles, count);

   free(handles);
}

// generate random byte string of length up to maxlen
static size_t randomstr(char * buf, size_t maxlen, bool cstr)
{
   size_t len = rand() % (cstr ? maxlen : maxlen + 1);

   if (cstr && !len)
   {
      ++len;
      assert(len <= maxlen);
   }

   for (unsigned int i=0; i<len; ++i)
   {
      char rchar = cstr ? (rand() % 254) +1 : rand();
      assert (!cstr || rchar);
      buf[i] = rchar;
   }
   if (cstr)
      buf[len-1] = 0;

   return len;
}



static varstring_handle varstring_from_raw_helper(const char * c,
      size_t len)
{
   // WOrkaround for missing varstring_from_raw
   // @TODO Update to varstring_from_raw in next iteration of HW
   char buf[len+1];
   memset(buf, 'a', len);
   buf[len] = 0;
   varstring_handle h = varstring_from(buf);
   CU_ASSERT_EQUAL(varstring_length(h), len);

   for (unsigned int i=0; i<len; ++i)
   {
      CU_ASSERT_TRUE(varstring_set_char(h, i, c[i]));
   }
   return h;
}

static varstring_handle create_random_varstring(bool cstr)
{
   char buf[VERY_LARGE];

   size_t len = randomstr(buf, sizeof(buf), cstr);

   if (cstr)
   {
      varstring_handle h = varstring_from(buf);
      CU_ASSERT_EQUAL(varstring_length(h), len-1);
      return h;
   }
   else
   {
      varstring_handle h = varstring_from_raw_helper(buf, len);
      CU_ASSERT_EQUAL(varstring_length(h), len);
      return h;
   }
}

// Returns number of valid characters in varstring (and contents)
// contents is 0-terminated (not included in count) if cstr is true
static size_t create_random_varstring_contents(varstring_handle * h,
      char ** contents, bool cstr)
{
   char buf[VERY_LARGE];
   size_t size = randomstr(buf, sizeof(buf), cstr);
   *contents = malloc(size);
   memcpy(*contents, buf, size);
   *h = cstr ? varstring_from(buf) : varstring_from_raw_helper(buf, size);
   CU_ASSERT_PTR_NOT_NULL(*h);
   assert(!cstr || size);
   return (cstr ? size - 1 : size);
}

// -- create varstring with random contents, including 0 length
static void create_random_varstrings_contents(varstring_handle * handles,
      size_t * sizes, char ** contents, size_t count,
      bool cstr)
{
   for (unsigned int i=0; i<count; ++i)
   {
      sizes[i] = create_random_varstring_contents(&handles[i],
            &contents[i], cstr);
   }
}

// can return empty strings
static void create_random_varstrings(varstring_handle * handles, size_t count,
      bool cstr)
{
   for (unsigned int i=0; i<count; ++i)
   {
      handles[i] = create_random_varstring(cstr);
   }
}

static void test_varstring_from_null()
{
   // Check NULL
   varstring_handle h = varstring_from(NULL);
   CU_ASSERT_EQUAL(varstring_length(h), 0);
   varstring_free(h);
}

static void test_varstring_from()
{
   const size_t count = COUNT;

   varstring_handle * handles = malloc(count * sizeof(varstring_handle));

   create_random_varstrings(handles, count, true);

   random_destroy(handles, count);
   free(handles);

}


static void validate_using_getchar(varstring_handle * handles,
      const size_t * sizes, char ** contents, size_t count)
{
   for (unsigned int i=0; i<count; ++i)
   {
      for (unsigned int j=0; j<sizes[i]; ++j)
      {
         int r = varstring_get_char(handles[i], j);
         int expected = contents[i][j];
         CU_ASSERT_EQUAL(r, expected);
      }

      // Out of bounds
      CU_ASSERT_EQUAL(varstring_get_char(handles[i], sizes[i]), -1);
      CU_ASSERT_EQUAL(varstring_get_char(handles[i], sizes[i]+(rand() % 10)), -1);
   }
}

static void test_varstring_get_char_helper(bool cstr)
{
   size_t count = COUNT;
   size_t * sizes = malloc(sizeof(*sizes)*count);
   varstring_handle * handles = malloc(sizeof(*handles)*count);
   char ** contents = malloc(sizeof(*contents)*count);

   create_random_varstrings_contents(handles, sizes, contents, count, cstr);

   validate_using_getchar(handles, sizes, contents, count);

   for (unsigned int i=0; i<count; ++i)
   {
      varstring_free(handles[i]);
      handles[i] = 0;
      free(contents[i]);
   }
   free(sizes);
   free(contents);
   free(handles);
}

static void test_varstring_get_char_cstr()
{
   test_varstring_get_char_helper(true);
}

static void test_varstring_get_char_raw()
{
   test_varstring_get_char_helper(false);
}

static void test_varstring_set_char_helper(bool cstr)
{
   size_t count = COUNT;
   size_t * sizes = malloc(sizeof(*sizes)*count);
   varstring_handle * handles = malloc(sizeof(*handles)*count);
   char ** contents = malloc(sizeof(*contents)*count);

   create_random_varstrings_contents(handles, sizes, contents, count, cstr);


   // Randomly change characters (including to 0-bytes if !cstr)
   for (unsigned int i=0; i<COUNT*10; ++i)
   {
      unsigned int picked = rand() % count;
      varstring_handle h = handles[picked];

      if (!sizes[picked])
         continue;

      size_t pos = rand() % sizes[picked];

      char newchar = (cstr ? rand() % 254 + 1 : rand());
      CU_ASSERT_TRUE(varstring_set_char(h, pos, newchar));
      contents[picked][pos] = newchar;
      CU_ASSERT_FALSE(varstring_set_char(h, sizes[picked], 'd'));
      CU_ASSERT_FALSE(varstring_set_char(h, sizes[picked]+1, 'd'));
   }

   validate_using_getchar(handles, sizes, contents, count);

   for (unsigned int i=0; i<count; ++i)
   {
      varstring_free(handles[i]);
      handles[i] = 0;
      free(contents[i]);
   }
   free(sizes);
   free(contents);
   free(handles);
}

static void test_varstring_set_char_cstr()
{
   test_varstring_set_char_helper(true);
}

static void test_varstring_set_char_raw()
{
   test_varstring_set_char_helper(false);
}


static void test_varstring_access_helper(bool cstr)
{
   size_t count = COUNT;
   size_t * sizes = malloc(sizeof(*sizes)*count);
   varstring_handle * handles = malloc(sizeof(*handles)*count);
   char ** contents = malloc(sizeof(*contents)*count);

   create_random_varstrings_contents(handles, sizes, contents, count, cstr);

   for (unsigned int i=0; i<count; ++i)
   {
      // this should not be a memory leak
      const char * ptr = varstring_access(handles[i]);

      if (!cstr)
      {
         CU_ASSERT_EQUAL(memcmp(ptr, contents[i], sizes[i]), 0);
      }
      else
      {
         CU_ASSERT_EQUAL(strcmp(ptr, contents[i]), 0);
      }
   }

   for (unsigned int i=0; i<count; ++i)
   {
      varstring_free(handles[i]);
      handles[i] = 0;
      free(contents[i]);
   }
   free(sizes);
   free(contents);
   free(handles);
}

static void test_varstring_access_raw()
{
   test_varstring_access_helper(false);
}

static void test_varstring_access_cstr()
{
   test_varstring_access_helper(true);
}


static void test_varstring_convert_helper(bool cstr)
{
   size_t count = COUNT;
   size_t * sizes = malloc(sizeof(*sizes)*count);
   varstring_handle * handles = malloc(sizeof(*handles)*count);
   char ** contents = malloc(sizeof(*contents)*count);

   create_random_varstrings_contents(handles, sizes, contents, count, cstr);

   for (unsigned int i=0; i<count; ++i)
   {
      char * ptr = varstring_convert(handles[i]);

      if (!cstr)
      {
         CU_ASSERT_EQUAL(memcmp(ptr, contents[i], sizes[i]), 0);
      }
      else
      {
         CU_ASSERT_EQUAL(strcmp(ptr, contents[i]), 0);
      }

      if (sizes[i])
      {
         // change string to detect failure to make a copy
         CU_ASSERT_TRUE(varstring_set_char(handles[i], 0, ptr[0]+1));
         CU_ASSERT_NOT_EQUAL(varstring_get_char(handles[i], 0), ptr[0]);
      }


      free(ptr);
   }

   for (unsigned int i=0; i<count; ++i)
   {
      varstring_free(handles[i]);
      handles[i] = 0;
      free(contents[i]);
   }
   free(sizes);
   free(contents);
   free(handles);
}

static void test_varstring_convert_raw()
{
   test_varstring_convert_helper(false);
}

static void test_varstring_convert_cstr()
{
   test_varstring_convert_helper(true);
}

static void test_varstring_convert_empty()
{
   varstring_handle h = varstring_from("");
   CU_ASSERT_EQUAL(varstring_length(h), 0);
   char * ptr = varstring_convert(h);
   varstring_free(h);
   CU_ASSERT_PTR_NOT_NULL(ptr);
   CU_ASSERT_EQUAL(strlen(ptr), 0);
   free(ptr);
}

static void test_varstring_reverse()
{
   bool cstr = false;
   size_t count = COUNT;
   size_t * sizes = malloc(sizeof(*sizes)*count);
   varstring_handle * handles = malloc(sizeof(*handles)*count);
   char ** contents = malloc(sizeof(*contents)*count);

   create_random_varstrings_contents(handles, sizes, contents, count, cstr);

   for (unsigned int i=0; i<count; ++i)
   {
      varstring_reverse(handles[i]);
   }

   for (unsigned int i=0; i<count; ++i)
   {
      for (unsigned int j=0; j<sizes[i]; ++j)
      {
         size_t rpos = sizes[i]-j-1;
         CU_ASSERT_EQUAL(varstring_get_char(handles[i], j), contents[i][rpos]);
      }
   }

   for (unsigned int i=0; i<count; ++i)
   {
      varstring_free(handles[i]);
      handles[i] = 0;
      free(contents[i]);
   }
   free(sizes);
   free(contents);
   free(handles);

}



static void test_varstring_set_string_null()
{
   varstring_handle h = varstring_from("1234");
   varstring_set_string(h, NULL);
   CU_ASSERT_EQUAL(varstring_length(h), 0);
   varstring_free(h);
}

// Modifies varstring and does the same operation to contents (reallocating if
// needed) and size
typedef void (*modifyfunc_t) (varstring_handle h, char ** contents,
      size_t * size);

static void raw_compare(varstring_handle h, const char * c, size_t len)
{
   CU_ASSERT_EQUAL(varstring_length(h), len);
   for (unsigned int i=0; i<len; ++i)
   {
      CU_ASSERT_EQUAL(varstring_get_char(h, i), c[i]);
   }
}


static void test_varstring_generic_modify(modifyfunc_t modify, bool cstr)
{
   size_t count = COUNT;
   size_t * sizes = malloc(sizeof(*sizes)*count);
   varstring_handle * handles = malloc(sizeof(*handles)*count);
   char ** contents = malloc(sizeof(*contents)*count);

   create_random_varstrings_contents(handles, sizes, contents, count, cstr);

   // Generate new contents
   for (unsigned int i=0; i<count; ++i)
   {
      modify(handles[i], &contents[i], &sizes[i]);
   }

   // Check for matching operation on both
   for (unsigned int i=0; i<count; ++i)
   {
      if (cstr)
      {
         CU_ASSERT_EQUAL(varstring_length(handles[i]), strlen(contents[i]));
         CU_ASSERT_STRING_EQUAL(varstring_access(handles[i]), contents[i]);
      }
      else
      {
         CU_ASSERT_EQUAL(varstring_length(handles[i]), sizes[i]);
         raw_compare(handles[i], contents[i], sizes[i]);
      }
   }

   for (unsigned int i=0; i<count; ++i)
   {
      varstring_free(handles[i]);
      handles[i] = 0;
      free(contents[i]);
   }
   free(sizes);
   free(contents);
   free(handles);


}

static void modify_set_string(varstring_handle h, char ** contents, size_t * size)
{
   char buf[VERY_LARGE];
   randomstr(buf, sizeof(buf), true);
   varstring_set_string(h, buf);
   size_t len = strlen(buf);
   free(*contents);
   *contents = malloc(len +1);
   *size=len;
   strcpy(*contents, buf);
}

static void test_varstring_set_string ()
{
   test_varstring_generic_modify(modify_set_string, true);
}


static char * append_data(void * ptr, size_t cursize, const void * data, size_t count)
{
   size_t newsize = cursize + count;
   char * newptr = realloc(ptr, newsize);
   assert(newptr);
   memcpy(&newptr[cursize], data, count);
   return newptr;
}

static void modify_append_char(varstring_handle h, char ** contents, size_t * size)
{
   size_t len = varstring_length(h);
   CU_ASSERT_EQUAL(*size, len);

   const size_t appendcount = (rand() % 100 ) + 1;
   char buf[appendcount];

   size_t added = randomstr(buf, appendcount, false);

   *contents = append_data(*contents, *size, buf, added);
   *size += added;

   for (unsigned int i=0; i<added; ++i)
   {
      varstring_append_char(h, buf[i]);
      CU_ASSERT_EQUAL(varstring_length(h), len+i+1);
   }
}

static void test_varstring_append_char()
{
   test_varstring_generic_modify(modify_append_char, false);
}

static void modify_append_str(varstring_handle h, char ** contents, size_t * size)
{
   size_t len = varstring_length(h);
   CU_ASSERT_EQUAL(*size, len);

   const size_t appendcount = (rand() % 100 ) + 1;
   char buf[appendcount];

   randomstr(buf, appendcount, true);
   size_t todo = strlen(buf);
   assert(buf[todo] == 0);

   *contents = append_data(*contents, *size, buf, todo+1);
   *size += todo;

   varstring_append_str(h, buf);
   CU_ASSERT_EQUAL(varstring_length(h), strlen(*contents));
   assert(*size == strlen(*contents));
}

static void test_varstring_append_str()
{
   test_varstring_generic_modify(modify_append_str, true);
}

static void modify_append_varstring(varstring_handle h, char ** contents, size_t * size)
{
   size_t len = varstring_length(h);
   CU_ASSERT_EQUAL(*size, len);

   const size_t appendcount = (rand() % 100 ) + 1;
   char buf[appendcount];

   size_t added = randomstr(buf, appendcount, false);

   varstring_handle newh = varstring_from_raw_helper(buf, added);

   *contents = append_data(*contents, *size, buf, added);
   *size += added;

   varstring_append_varstring(h, newh);
   varstring_free(newh);
}

static void test_varstring_append_varstring()
{
   test_varstring_generic_modify(modify_append_varstring, false);
}

static char * str_insert(char * in, size_t at, char * data)
{
   size_t curlen = strlen(in);
   assert(at <= curlen);

   size_t newdata = strlen(data);

   size_t newlen = newdata + curlen;

   in = realloc(in, newlen + 1);
   assert(in);
   if (at < curlen)
      memmove(&in[at+newdata], &in[at], curlen-at);
   memcpy(&in[at], data, newdata);
   in[newlen] = 0;
   return  in;
}

static void modify_insert_str(varstring_handle h, char ** contents, size_t * size)
{
   CU_ASSERT_EQUAL(*size, varstring_length(h));

   const size_t appendcount = (rand() % 100 ) + 1;
   char buf[appendcount];

   randomstr(buf, appendcount, true);
   size_t added = strlen(buf);

   size_t at = rand() % (varstring_length(h) + 1);
   *contents = str_insert(*contents, at, buf);
   *size += added;

   varstring_insert_str(h, at, buf);
}

static void test_varstring_insert_str()
{
   test_varstring_generic_modify(modify_insert_str, true);
}

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{
   srand(time(NULL));
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite("Suite_1", init_suite, clean_suite);
   if (NULL == pSuite)
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
   if (
       (NULL == CU_add_test(pSuite, "varstring_new", test_varstring_new))
    || (NULL == CU_add_test(pSuite, "varstring_from", test_varstring_from))
    || (NULL == CU_add_test(pSuite, "varstring_from_null", test_varstring_from_null))
    || (NULL == CU_add_test(pSuite, "varstring_get_char(cstr)", test_varstring_get_char_cstr))
    || (NULL == CU_add_test(pSuite, "varstring_get_char(raw)", test_varstring_get_char_raw))
    || (NULL == CU_add_test(pSuite, "varstring_set_char(cstr)", test_varstring_set_char_cstr))
    || (NULL == CU_add_test(pSuite, "varstring_set_char(raw)", test_varstring_set_char_raw))
    || (NULL == CU_add_test(pSuite, "varstring_access(raw)", test_varstring_access_raw))
    || (NULL == CU_add_test(pSuite, "varstring_access(cstr)", test_varstring_access_cstr))
    || (NULL == CU_add_test(pSuite, "varstring_convert(raw)", test_varstring_convert_raw))
    || (NULL == CU_add_test(pSuite, "varstring_convert(cstr)", test_varstring_convert_cstr))
    || (NULL == CU_add_test(pSuite, "varstring_convert(empty)", test_varstring_convert_empty))
    || (NULL == CU_add_test(pSuite, "varstring_reverse", test_varstring_reverse))
    || (NULL == CU_add_test(pSuite, "varstring_set_string", test_varstring_set_string))
    || (NULL == CU_add_test(pSuite, "varstring_set_string_null", test_varstring_set_string_null))
    || (NULL == CU_add_test(pSuite, "varstring_append_char", test_varstring_append_char))
    || (NULL == CU_add_test(pSuite, "varstring_append_str", test_varstring_append_str))
    || (NULL == CU_add_test(pSuite, "varstring_append_varstring", test_varstring_append_varstring))
    || (NULL == CU_add_test(pSuite, "varstring_insert_str", test_varstring_insert_str))
    )
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}


