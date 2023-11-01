#include "../include/common.h"
#include "pa4.h"


// don't remove these globals
struct heap_elem *min_heap_arr = NULL;

// don't modify this function
void initialize_min_heap_arr(struct heap_elem *arr)
{
  min_heap_arr = arr;
}

// don't modify this function
static void *allocate_memory(size_t size)
{
  return __mymalloc(size);
}

// don't modify this function
static void free_memory(void *ptr)
{
  __myfree(ptr);
}

// ---------------- Add your code below this line ----------------- //


// The friends field in "struct record" stores the
// head of the linked list of friends of a given user.
// return r->friends.
static int cmp_uid(char *uid1, char *uid2)
{
  int i;

  for (i = 0; i < MAX_LEN; i++) {
    if (uid1[i] > uid2[i]) {
      return 1;
    }
    else if (uid1[i] < uid2[i]) {
      return -1;
    }
  }
  return 0;
}
static int cmp_record(struct record *r1, struct record *r2)
{
  return cmp_uid(r1->uid, r2->uid);
}
struct record *find_record(struct record *node, char *uid)
{
    if (node == NULL || cmp_uid(node->uid, uid) == 0)
        return node;

    if (cmp_uid(node->uid, uid) < 0)
        return find_record(node->right, uid);
    else
        return find_record(node->left, uid);
}

// Helper function to check if two records are friends
int are_friends(struct record *p, struct record *q)
{
    struct list_records *r = p->friends;
    while (r != NULL) {
        if (r->record == q)
            return 1;
        r = r->next;
    }
    return 0;
}

// Helper function to add a friend to a user's friend list
void add_friend(struct record *p, struct record *friend)
{
    struct list_records *frnd = (struct list_records *)allocate_memory(sizeof(struct list_records));
    frnd->record = friend;
    frnd->next = NULL;

    if (p->friends == NULL)
        p->friends = frnd;
    else {
        struct list_records *curr = p->friends;
        while (curr->next != NULL)
            curr = curr->next;
        curr->next = frnd;
    }
}
struct list_records* get_friends_list(struct record *r)
{
    return r->friends;
}


// Make r1 and r2 friends of each other if they aren't already friends.
// The friends field in "struct record" stores the
// head of the linked list of friends of a given user.
// Return 1 if r1 and r2 are already friends before this call.
// Otherwise, make r1 a friend of r2 and return 0.
// To make r1 a friend of r2, 
// insert r1 into the list r2->friends and insert r2 into the list r1->friends.
int make_friends(struct record *r1, struct record *r2)
{
    if (are_friends(r1, r2))
        return 1; // r1 and r2 are already friends

    add_friend(r1, r2);
    add_friend(r2, r1);
    return 0; // r1 and r2 became friends
}


// remove all nodes from r->friends and release the corresponding memory
void delete_friends_list(struct record *r)
{
  struct list_records *curr = r->friends;
  struct list_records *prev = NULL;
  while (curr!=NULL){
    prev=curr;
    free_memory(curr);
    curr=prev->next;
  }
  r->friends = NULL;
}

// Each friends f of r is connected via an undirected
// edge whose weight is the distance between r and f.
// compute the shortest distance and a shortest path to 
// all other vertices reachable from r.
// initially, the status fields in all records are set to zero.
// distance and pred fields may contain garbage values.
// you need to use the distance function to compute the distance
// between two records.
// you can use min_heap_arr to implement a min heap or
// a linear scan to find a vertex with the minimum distance.
// In addition to distance and pred, you can also update the
// status field in your implement.
// Don't modify other fields in struct record.
// At the end of this procedure, distance field in each record
// reachable via r, must contain the minimum distance from r;
// and the pred field must contain the predecessor on a shortest
// path.

void heapify(struct heap_elem *min_heap, int heap_size, int i)
{
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < heap_size && min_heap[left].r->distance < min_heap[smallest].r->distance)
        smallest = left;

    if (right < heap_size && min_heap[right].r->distance < min_heap[smallest].r->distance)
        smallest = right;

    if (smallest != i)
    {
        struct heap_elem temp = min_heap[i];
        min_heap[i] = min_heap[smallest];
        min_heap[smallest] = temp;

        heapify(min_heap, heap_size, smallest);
    }
}

void insert_key(struct heap_elem *min_heap, int *heap_size, struct record *r, double key)
{
    // Create a new heap element
    struct heap_elem new_elem;
    new_elem.r = r;
    new_elem.r->distance = key;

    // Insert the new element at the end of the heap
    int i = (*heap_size)++;
    min_heap[i] = new_elem;

    // Fix the min heap property by moving the new element up the heap
    while (i > 0 && min_heap[i].r->distance < min_heap[(i - 1) / 2].r->distance)
    {
        struct heap_elem temp = min_heap[i];
        min_heap[i] = min_heap[(i - 1) / 2];
        min_heap[(i - 1) / 2] = temp;

        i = (i - 1) / 2;
    }
}

struct record *extract_min(struct heap_elem *min_heap, int *heap_size){
    if (*heap_size == 0) {
        struct heap_elem empty;
        empty.r = NULL;
        return empty.r;
    }

    struct heap_elem min_elem = min_heap[0];
    min_heap[0] = min_heap[*heap_size - 1];
    (*heap_size)--;
    heapify(min_heap, *heap_size, 0);

    return min_elem.r;
}


void dfs(struct record *r)
{
    // Mark the record as visited
    r->status = 1;
    r->pred=NULL;
    // Get the linked list of friends for the current record
    struct list_records *friends_list = r->friends;

    // Traverse through the linked list of friends
    while (friends_list != NULL)
    {
        struct record *v = friends_list->record;
        v->distance=INFINITY;
        // If v is not visited, recursively call dfs on v
        if (v->status == 0)
        {
            
            dfs(v);
        }

        // Move to the next friend in the list
        friends_list = friends_list->next;
    }
}

void compute_sssp(struct record *r)
{
    dfs(r);

    r->distance = 0.0;

    struct heap_elem min_heap[100000];
    int heap_size = 0;

    insert_key(min_heap, &heap_size, r, r->distance);

    while (heap_size > 0)
    {
        struct record *u = extract_min(min_heap, &heap_size);
        u->status = 1;
        struct list_records *friends_list = u->friends;

        while (friends_list != NULL)
        {
            struct record *v = friends_list->record;
            double dist = u->distance + distance(&u->loc, &v->loc);

            if (dist < v->distance)
            {
                v->distance = dist;
                v->pred = u;

                insert_key(min_heap, &heap_size, v, v->distance);
            }

            friends_list = friends_list->next;
        }
    }
}
