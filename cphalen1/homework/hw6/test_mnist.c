/*********************************************************

  Unit testing suite for loading MNIST files into
  our C programs

*********************************************************/

#include "mnist.h"

#include "CUnit/Basic.h"

int init_suite() {
  return 0;
}

int clean_suite() {
  return 0;
}

void mnist_image_set_label(mnist_image_handle h, int label);

// Ensure we can properly open a database and handle the contents
void test_mnist_open() {
  mnist_dataset_handle dataset = mnist_open("t10k");
  mnist_image_handle current = mnist_image_begin(dataset);
  CU_ASSERT_PTR_NOT_NULL_FATAL(current);
  const unsigned char * information = mnist_image_data(current);
  CU_ASSERT_PTR_NOT_NULL(information);
  int label = mnist_image_label(current);
  CU_ASSERT_NOT_EQUAL(label, 10);
  current = mnist_image_next(current);
  CU_ASSERT_PTR_NOT_NULL_FATAL(current);
  information = mnist_image_data(current);
  CU_ASSERT_PTR_NOT_NULL(information);
  label = mnist_image_label(current);
  CU_ASSERT_NOT_EQUAL(label, 10);
  mnist_free(dataset);
}

// Ensure that an empty database can be created as well to be populated with new values
void test_mnist_create() {
  mnist_dataset_handle h = mnist_create(10, 12);
  unsigned int row;
  unsigned int col;
  mnist_image_size(h, &row, &col);
  CU_ASSERT_EQUAL(row, 10);
  CU_ASSERT_EQUAL(col, 12);
  mnist_image_handle head = mnist_image_begin(h);
  CU_ASSERT_PTR_NULL(head);
  mnist_free(h);
}

// Test the majority of mutators for an mnist database or image structure
void test_mnist_linkage() {
  mnist_dataset_handle dataset = mnist_open("t10k");
  mnist_image_handle current = mnist_image_begin(dataset);
  mnist_image_handle next = mnist_image_next(current);
  unsigned int nextRow;
  unsigned int nextCol;
  mnist_image_size(dataset, &nextRow, &nextCol);
  const unsigned char * nextInformation = mnist_image_data(next);
  mnist_image_handle after = mnist_image_add_after(current, nextInformation, nextRow, nextCol);
  mnist_image_set_label(after, 8);
  const unsigned char * afterInformation = mnist_image_data(after);
  CU_ASSERT_PTR_NOT_EQUAL(nextInformation, afterInformation);
  CU_ASSERT_EQUAL(*nextInformation, *afterInformation);
  mnist_free(dataset);
}

// Really rudimentary -- just ensures the database is keeping track of its elements
void test_mnist_image_count() {
  mnist_dataset_handle dataset = mnist_open("t10k");
  int length = mnist_image_count(dataset);
  CU_ASSERT_EQUAL(length, 10000)
  mnist_free(dataset);
}

// Checks to ensure that the accessor functions for an mnist database or image are working as well
void test_mnist_accessor() {
  mnist_dataset_handle dataset = mnist_open("t10k");
  mnist_image_handle current = mnist_image_begin(dataset);
  CU_ASSERT_EQUAL(mnist_image_label(current), 7);
  const unsigned char * information = mnist_image_data(current);
  CU_ASSERT_PTR_NOT_NULL(information);
  mnist_free(dataset);
}

// Specifically tests and will example whether the sample database
// will work correctly with on the correct number of members
void test_mnist_open_sample() {
  mnist_dataset_handle dataset = mnist_open_sample("t10k", 10);
  unsigned int length = mnist_image_count(dataset);
  CU_ASSERT_EQUAL(length, 10);
  mnist_image_handle current = mnist_image_begin(dataset);
  for(int i = 0; i < 9; i++) {
    current = mnist_image_next(current);
  }
  CU_ASSERT_NOT_EQUAL(10, mnist_image_label(current));
  current = mnist_image_next(current);
  CU_ASSERT_EQUAL(10, mnist_image_label(current));
  mnist_free(dataset);
}

// Saves the data in a database to two files and ensures that no errors are genearted along the way
void test_mnist_save() {
  mnist_dataset_handle dataset = mnist_open("t10k");
  mnist_save(dataset, "destination");
  mnist_free(dataset);
}

// Returns CUE_SUCCESS on completion or a
// CUNIT error if there is failure
int main() {
  CU_pSuite testingSuite = NULL;

  // Makes a CUnit registry and ensures it was initialized without failure (and will handle such an error)
  if (CUE_SUCCESS != CU_initialize_registry())
     return CU_get_error();

   // Add our one and only suite to the registry initialized above
   testingSuite = CU_add_suite("testingSuite", init_suite, clean_suite);
   // Ensures our suite was added to the registry without failure
   if (NULL == testingSuite)
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   // Now we will add every single individual test to the testing suite -- so they are all run when we run the testingSuite
   // Puting the values into an if statement allows us to condense and utilize short-circuit logic operations
   if (NULL == CU_add_test(testingSuite, "mnist_open()", test_mnist_open) ||
       NULL == CU_add_test(testingSuite, "mnist_image_count()", test_mnist_image_count) ||
       NULL == CU_add_test(testingSuite, "mnist_create()", test_mnist_create) ||
       NULL == CU_add_test(testingSuite, "mnist_linkage()", test_mnist_linkage) ||
       NULL == CU_add_test(testingSuite, "mnist_accessor()", test_mnist_accessor) ||
       NULL == CU_add_test(testingSuite, "mnist_open_sample()", test_mnist_open_sample) ||
       NULL == CU_add_test(testingSuite, "mnist_save()", test_mnist_save)) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   // Uses the CUnit functionality to actually run the unit tests
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}
