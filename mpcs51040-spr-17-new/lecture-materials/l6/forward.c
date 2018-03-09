

// Forward declaration
struct Node;

// OK to declare pointers to struct Node, size is known (size of pointer)
void test (struct Node * n);

// BAD
Node n;

// BAD
n.member = 2;




