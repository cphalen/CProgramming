#include "mnist.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#define IMAGE_WIDTH 28
#define IMAGE_HEIGHT 28
#define MAX_INPUT_LENGTH 256
#define LABEL_INVALID_VALUE 10

typedef struct mnist_dataset_t mnist_dataset_t;
typedef struct mnist_image_t mnist_image_t;

struct mnist_image_t {
  unsigned char * information;
  int label;
  mnist_image_handle next;
};

struct mnist_dataset_t {
  int numRow;
  int numCol;
  int length;
  int labelMagicNumber;
  int imageMagicNumber;
  // We implement the image as a member of the mnist_dataset_t as a linked list
  mnist_image_handle head;
};

// This function will read in a single integer value
// from the FILE stream provided and ensure that the
// integer is returned in the correct endian
// returns the integer read in and will fail if
// an invaild FILE stream is passed as a parameter
static int read_in_int_from_file_internal(FILE * source) {
  assert(source);
  unsigned char inInteger;
  int returnValue = 0;
  for(int i = 0; i < sizeof(int); i++) {
    fread(&inInteger, (size_t) 1, (size_t) 1, source);
    returnValue += inInteger << ((sizeof(int) - (i + 1)) * sizeof(char) * 8);
  }

  return returnValue;
}

// This works almost as the inverse of reading in an integer
// and will instead write an integer to the FILE stream passed
// as a paremeter. This function will also ensure that the
// information being written to the file is in the correct
// endian
// returns void, yet will fail is the FILE stream is invalid
static void read_out_int_to_file_internal(FILE * source, int n) {
  assert(source);
  unsigned char writeValue;
  for(int i = 0; i < sizeof(int); i++) {
    writeValue = n >> ((sizeof(int) - (i + 1)) * sizeof(char) * 8);
    fwrite(&writeValue, 1, 1, source);
  }
}

mnist_dataset_handle mnist_create(unsigned int x, unsigned int y) {
  if(x == 0 || y == 0)
    return MNIST_DATASET_INVALID;
  mnist_dataset_handle returnValue = (mnist_dataset_handle) malloc(sizeof(mnist_dataset_t));
  returnValue->numRow = y;
  returnValue->numCol = x;
  // Initializes all other values to defaults
  returnValue->head = NULL;
  returnValue->labelMagicNumber = 0;
  returnValue->imageMagicNumber = 0;
  return returnValue;
}

// This function takes an mnist_dataset_handle and an mnist_image_handle
// it will take the mnist_image_handle value and append it to the end
// of the linked list that is a member of the mnist_dataset_handle
// returns 0 on success and -1 on failure and will ensure that
// it will not write to the next element in the linked list
// if the next value is NULL
static int mnist_dataset_append_image_internal(mnist_dataset_handle dataset, mnist_image_handle image) {
  if(!dataset || !image)
    return -1;

  if(dataset->head == NULL) {
    dataset->head = image;
    return 0;
  }

  mnist_image_handle current = dataset->head;
  while(current->next != NULL)
    current = current->next;
  current->next = image;

  return 0;
}

void mnist_image_size(const mnist_dataset_handle handle, unsigned int * x, unsigned int * y) {
  // These images need to be regularly quadrilateral
  if(handle == MNIST_DATASET_INVALID) {
    *x = 0;
    *y = 0;
    return;
  }

  *x = handle->numCol;
  *y = handle->numRow;
}

mnist_image_handle mnist_image_begin(const mnist_dataset_handle handle) {
  if(!handle)
    return MNIST_IMAGE_INVALID;
  return handle->head;
}

mnist_image_handle mnist_image_next (const mnist_image_handle h) {
  if(!h)
    return MNIST_IMAGE_INVALID;
  return h->next;
}

// This prototype takes no "label" value as a parameter, thus we have
// created another function "mnist_image_set_label" that can be used
// to augment label after the image has been appened
mnist_image_handle mnist_image_add_after (mnist_image_handle h, const unsigned char * imagedata, unsigned int x, unsigned int y) {
  if((!h || !imagedata) || (x != y))
    return MNIST_IMAGE_INVALID;
  mnist_image_handle added = (mnist_image_handle) malloc(sizeof(mnist_image_t));
  unsigned char * information = (unsigned char *) malloc(sizeof(unsigned char) * x * y);
  memcpy(information, imagedata, sizeof(unsigned char) * x * y);
  added->next = h->next;
  added->information = information;
  added->label = LABEL_INVALID_VALUE;

  h->next = added;

  return added;
}

// Acts both as a counterbalence to the missing implementation
// in the mnist_image_add_after() function, but will additionally
// work independently as its own functionality
// returns false on failure and true on success
bool mnist_image_set_label(mnist_image_handle h, unsigned int newlabel) {
  if(!h)
    return false;
  h->label = newlabel;
  return true;
}

unsigned int mnist_image_count(const mnist_dataset_handle handle) {
  if(!handle)
    return -1;
  return handle->length;
}

const unsigned char * mnist_image_data(const mnist_image_handle h) {
  if(!h)
    return NULL;
  return h->information;
}

unsigned int mnist_image_label(const mnist_image_handle h) {
  if(!h)
    return LABEL_INVALID_VALUE;
  return h->label;
}

void mnist_free(mnist_dataset_handle handle) {
  assert(handle);
  if(handle->head != MNIST_IMAGE_INVALID) {
    mnist_image_handle current = handle->head;
    mnist_image_handle next = NULL;
    while(current->next != NULL) {
      next = current->next;
      free(current->information);
      free(current);
      current = next;
    }
    // Frees the head and the information linked to the head after
    // parsing through all elements leading up to the head
    free(current->information);
    free(current);
  } else {
    free(handle->head);
  }
  // Actually frees the database itself, the user will not be
  // able to reassign information to this dataset
  free(handle);
}

// This is an internal function that opens the FILE streams to both files
// when creating a database. It will also read in the magic numbers to
// ensure validity and initialize the actual database value with some default
// values. The main reason this function exists is to avoid repetition between
// mnist_open() and mnist_open_sample()
// returns the mnist_dataset_handle initialized on success and MNIST_DATASET_INVALID
// on failure
static mnist_dataset_handle mnist_open_initialize_file_internal(const char * name, FILE ** label, FILE ** image) {
  if(!label || !image)
    return MNIST_DATASET_INVALID;

  char destination[MAX_INPUT_LENGTH];

  memcpy(destination, name, strlen(name) + 1);
  strcat(destination, "-labels-idx1-ubyte");
  *label = fopen(destination, "r");

  memcpy(destination, name, strlen(name) + 1);
  strcat(destination, "-images-idx3-ubyte");
  *image = fopen(destination, "r");

  if(!(*label) || !(*image))
    return MNIST_DATASET_INVALID;

  int labelMagicNumber = read_in_int_from_file_internal(*label);
  if(labelMagicNumber != 2049)
    return MNIST_DATASET_INVALID;

  int imageMagicNumber = read_in_int_from_file_internal(*image);
  if(imageMagicNumber != 2051)
    return MNIST_DATASET_INVALID;

  int numLabel = read_in_int_from_file_internal(*label);
  int numImage = read_in_int_from_file_internal(*image);
  if(numLabel != numImage)
    return MNIST_DATASET_INVALID;

  int numRow = read_in_int_from_file_internal(*image);
  int numCol = read_in_int_from_file_internal(*image);

  mnist_dataset_handle returnDataset = mnist_create(numCol, numRow);
  returnDataset->length = numImage;
  returnDataset->labelMagicNumber = labelMagicNumber;
  returnDataset->imageMagicNumber = imageMagicNumber;
  return returnDataset;
}

mnist_dataset_handle mnist_open(const char * name) {

  // We need to initalize these values here and
  // then pass them so we can use the FILE streams
  // in this function as well
  FILE * label = NULL;
  FILE * image = NULL;

  mnist_dataset_handle returnDataset = mnist_open_initialize_file_internal(name, &label, &image);
  if(returnDataset == MNIST_DATASET_INVALID) {
    fclose(label);
    fclose(image);
    return MNIST_DATASET_INVALID;
  }

  for(int i = 0; i < returnDataset->length; i++) {
    mnist_image_handle current = (mnist_image_handle) malloc(sizeof(mnist_image_t));
    // Initialize values in current to ensure there are no extranous defaults
    current->next = NULL;
    current->label = LABEL_INVALID_VALUE;
    unsigned char * information = (unsigned char *) malloc(sizeof(unsigned char) * (returnDataset->numRow) * (returnDataset->numCol));
    unsigned char buffer;
    for(int r = 0; r < returnDataset->numRow; r++) {
      for(int c = 0; c < returnDataset->numCol; c++) {
        // Reads in the actual pixel values byte by byte
        fread(&buffer, 1, 1, image);
        *(information + (r * returnDataset->numCol) + c) = buffer;
      }
    }
    // Simultaniously reads in the label byte by byte
    fread(&current->label, 1, 1, label);
    current->information = information;
    mnist_dataset_append_image_internal(returnDataset, current);
  }

  fclose(label);
  fclose(image);
  return returnDataset;
}

mnist_dataset_handle mnist_open_sample(const char * name, unsigned int k) {

  // We need to initalize these values here and
  // then pass them so we can use the FILE streams
  // in this function as well
  FILE * label = NULL;
  FILE * image = NULL;

  mnist_dataset_handle returnDataset = mnist_open_initialize_file_internal(name, &label, &image);

  // Generate random subset of K numbers here
  srand(time(NULL));
  unsigned int randoms[k];
  for(int i = 0; i < k;) {
    randoms[i] = rand() % returnDataset->length;
    for(int j = 0; j < i; j++) {
      if(randoms[i] == randoms[j])
        continue;
    }
    i++;
  }

  for(int i = 0; i < returnDataset->length; i++) {
    mnist_image_handle current = (mnist_image_handle) malloc(sizeof(mnist_image_t));
    // Initialize values in current to ensure there are no extranous defaults
    current->next = NULL;
    current->label = LABEL_INVALID_VALUE;

    unsigned char * information = (unsigned char *) malloc(sizeof(unsigned char) * (returnDataset->numRow) * (returnDataset->numCol));
    char buffer;
    for(int r = 0; r < returnDataset->numRow; r++) {
      for(int c = 0; c < returnDataset->numCol; c++) {
        // Reads in the actual pixel values byte by byte
        fread(&buffer, 1, 1, image);
        *(information + r + c) = buffer;
      }
    }
    // Simultaniously reads in the label byte by byte
    fread(&current->label, 1, 1, label);
    current->information = information;

    int found = 0;
    for(int j = 0; j < k; j++) {
      if(randoms[j] == i) {
        found = 1;
        mnist_dataset_append_image_internal(returnDataset, current);
      }
    }
    if(found == 0) {
      free(information);
      free(current);
    }
  }

  returnDataset->length = k;

  fclose(label);
  fclose(image);
  return returnDataset;
}

bool mnist_save(const mnist_dataset_handle h, const char * filename) {
  if(!h || !filename)
    return false;
  char destination[MAX_INPUT_LENGTH];

  memcpy(destination, filename, strlen(filename) + 1);
  strcat(destination, "-labels-idx1-ubyte");
  FILE * saveLabel = fopen(destination, "w+");

  memcpy(destination, filename, strlen(filename) + 1);
  strcat(destination, "-images-idx3-ubyte");
  FILE * saveImage = fopen(destination, "w+");

  if(!saveLabel || !saveImage) {
    fclose(saveLabel);
    fclose(saveImage);
    return false;
  }

  read_out_int_to_file_internal(saveLabel, h->labelMagicNumber);
  read_out_int_to_file_internal(saveImage, h->imageMagicNumber);
  read_out_int_to_file_internal(saveLabel, h->length);
  read_out_int_to_file_internal(saveImage, h->length);

  read_out_int_to_file_internal(saveImage, h->numRow);
  read_out_int_to_file_internal(saveImage, h->numCol);

  mnist_image_handle current = h->head;
  for(int i = 0; i < h->length; i++) {
    // Writes out the label to the labels file byte by byte
    fwrite(&current->label, 1, 1, saveLabel);
    for(int r = 0; r < h->numRow; r++) {
      for(int c = 0; c < h->numCol; c++) {
        if(!(current->information + (r * (h->numRow)) + c)) {
          // Ensure that no information is unreleased when the program exits
          fclose(saveLabel);
          fclose(saveImage);
          return false;
        }
        // Writes out the information to the images file byte by byte
        fwrite(current->information + (r * (h->numCol)) + c, 1, 1, saveImage);
      }
    }
    current = current->next;
  }

  fclose(saveLabel);
  fclose(saveImage);
  return true;
}
