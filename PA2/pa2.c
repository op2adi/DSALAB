#include "../include/common.h"
#include "pa2.h"


// don't remove these globals
static struct record *bst_root = NULL;
static struct record *avl_root = NULL;
static size_t num_bst_nodes = 0;
static size_t num_avl_nodes = 0;

// don't modify this function
struct record *get_bst_root()
{
  return bst_root;
}

// don't modify this function
struct record *get_avl_root()
{
  return avl_root;
}

// don't modify this function
// compare two uids
// return -1 if uid1 < uid2
// return 0 if uid1 == uid2
// return 1 if uid1 > uid2
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

// don't modify this function
// compare two records
// we assume that uid is unique and
// there can't be two records with the 
// same uid
// return -1 if record1 < record2
// return 0 if record1 == record2
// return 1 if record1 > record2
static int cmp_record(struct record *r1, struct record *r2)
{
  return cmp_uid(r1->uid, r2->uid);
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

// Return the total number of records in the
// BST rooted at bst_root.
size_t get_num_bst_records()
{

    return num_bst_nodes;
}
size_t get_num_avl_records()
{
  return num_avl_nodes;
}

// Insert record r in the BST rooted at bst_root.
void insert_record_bst(struct record r) {
  struct record *new_node = (struct record *)allocate_memory(sizeof(struct record));
  *new_node = r;
  new_node->left = NULL;
  new_node->right = NULL;

  if (bst_root == NULL) {
    bst_root = new_node;
  } else {
    struct record *current_node = bst_root;
    struct record *parent_node;
    while (1) {
      parent_node = current_node;
      if (cmp_record(new_node, current_node) < 0) {
        current_node = current_node->left;
        if (current_node == NULL) {
          parent_node->left = new_node;
          break;
        }
      } else {
        current_node = current_node->right;
        if (current_node == NULL) {
          parent_node->right = new_node;
          break;
        }
      }
    }
  }
  num_bst_nodes++;
}
int ht(struct record* a){
  int h1,h2;
  h1=a && a->left?a->left->height:0;
  h2=a && a->right?a->right->height:0;
  return h1>h2?h1+1:h2+1;
}
int balancefactor(struct record* a){
  int h1,h2;
  h1=a && a->left?a->left->height:0;
  h2=a && a->right?a->right->height:0;
  return h1-h2;
}
struct record* LLrot(struct record * a){
  struct record* b=a->left;
  struct record* c=b->right;
  b->right=a;
  a->left=c;
  a->height=ht(a);
  b->height=ht(b);

  if (avl_root==a){
    avl_root=b;
    avl_root->left=c;
  }
  return b;
}
struct record* LRrot(struct record* a){
  struct record* b=a->left;
  struct record* c=b->right;

  b->right=c->left;
  a->left=c->right;

  c->left=b;
  c->right=a;

  b->height=ht(b);
  a->height=ht(a);
  c->height=ht(c);

  if (avl_root==a){
    avl_root=c;
  } 

  return c;


}

struct record* RRrot(struct record * a){
  struct record* b=a->right;
  struct record* c=b->left;
  b->left=a;
  a->right=c;
  a->height=ht(a);
  b->height=ht(b);

  if (avl_root==a){
    avl_root=b;
  }
  return b;
}

struct record* RLrot(struct record* a){
  struct record* b=a->right;
  struct record* c=b->left;

  b->left=c->right;
  a->right=c->left;

  c->right=b;
  c->left=a;

  b->height=ht(b);
  a->height=ht(a);
  c->height=ht(c);

  if (avl_root==a){
    avl_root=c;
  }
  return c;
}

int max(int a,int b){
  if (a>=b){
    return a;
  }
  else {
    return b;
  }
}
struct record* abcd(struct record* p,struct record r){
  if (p==NULL){
    struct record *new_node = (struct record *)allocate_memory(sizeof(struct record));
    *new_node = r;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->height = 1;
    new_node->parent = NULL;
    return new_node;
  }
  if (cmp_uid(p->uid,r.uid)>0){
    p->left=abcd(p->left,r);
  }
  else {
    p->right=abcd(p->right,r);
  }
  
  p->height=ht(p);
  if(balancefactor(p)==2 && balancefactor(p->left)==1)
    return LLrot(p);
 else if(balancefactor(p)==2 && balancefactor(p->left)==-1)
    return LRrot(p);
 else if(balancefactor(p)==-2 && balancefactor(p->right)==-1)
    return RRrot(p);
 else if(balancefactor(p)==-2 && balancefactor(p->right)==1)
    return RLrot(p);
  return p;
}
void insert_record_avl(struct record r) {
  avl_root=abcd(avl_root,r);
  avl_root->height=ht(avl_root);
  num_avl_nodes++;
}


// Search the record corresponding to uid in the BST
// rooted at bst_root.
// If the record is not present, return a dummy record
// with -1 in the status field.
struct record search_record_bst(char uid[MAX_LEN]) {
    struct record *current_node = bst_root;
    while (current_node != NULL) {
        int cmp_result = cmp_uid(uid, current_node->uid);
        if (cmp_result == 0) {
            // found the record
            struct record result = *current_node;
            return result;
        } else if (cmp_result < 0) {
            current_node = current_node->left;
        } else {
            current_node = current_node->right;
        }
    }

    // record not found, return a dummy record with -1 status
    struct record result;
    result.status = -1;
    return result;
}


// The friends field in "struct record" stores the
// head of the linked list of friends of a given user.
// Return the head of the linked list of friends
// of the user with uid uid in the BST rooted at bst_root.
// If the corresponding record doesn't exist, return NULL.
struct list_records *get_friends_list_bst(char uid[MAX_LEN])
{
  struct record a = search_record_bst(uid);
  if (a.status==-1){
    return NULL;
  }
  struct list_records* b=a.friends;
  return b;
}


// Search the record corresponding to uid in the AVL tree
// rooted at avl_root.
// If the record is not present, return a dummy record 
// with -1 in the status field.
struct record search_record_avl(char uid[MAX_LEN])
{
    struct record *current_node = avl_root;
    while (current_node != NULL) {
        int cmp_result = cmp_uid(uid, current_node->uid);
        if (cmp_result == 0) {
            // found the record
            struct record result = *current_node;
            return result;
        } else if (cmp_result < 0) {
            current_node = current_node->left;
        } else {
            current_node = current_node->right;
        }
    }

    // record not found, return a dummy record with -1 status
    struct record result;
    result.status = -1;
    return result;
}

// The friends field in "struct record" stores the
// head of the linked list of friends of a given user.
// Return the head of the linked list of friends
// of the user with uid uid in the AVL tree rooted at avl_root.
// If the corresponding record doesn't exist, return NULL.
struct list_records *get_friends_list_avl(char uid[MAX_LEN])
{
  struct record a = search_record_avl(uid);
  if (a.status==-1){
    return NULL;
  }
  struct list_records* b=a.friends;
  return b;
}

void delete_from_friends_list(struct record* r1, char uid[MAX_LEN]) {
    
    struct list_records* temp = r1->friends;
    struct list_records* prev = NULL;
    while (temp != NULL) {
        if (cmp_uid(temp->record->uid, uid) == 0) {
          if (prev == NULL) {
            r1->friends = temp->next;
            free_memory(temp);
            return;
          }
            prev->next = temp->next;
            free_memory(temp);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
}
// Delete the node with record r from the AVL tree rooted at avl_root.

int ht2(struct record* a){
  if (a==NULL){
    return 0;
  }
  int h1,h2;
  h1=a && a->left?a->left->height:0;
  h2=a && a->right?a->right->height:0;
  return h1>h2?h1+1:h2+1;
}
// Delete the node with record r from the AVL tree rooted at avl_root.
void inorder(struct record* avl){
      if (avl==NULL){
        return;
      }
      inorder(avl->left);
      printf("%s\n",(*avl).uid);
      inorder(avl->right);
    }
void update_height(struct record *n) {
    n->height = max(ht2(n->left), ht2(n->right)) + 1;
}


void set_height(struct record *n) {
    int l_height = (n->left) ? n->left->height : 0;
    int r_height = (n->right) ? n->right->height : 0;
    n->height = (l_height > r_height) ? l_height + 1: r_height + 1;
}
int get_balance(struct record *n) {
    int l_height = (n->left) ? n->left->height : 0;
    int r_height = (n->right) ? n->right->height : 0;
return l_height - r_height;
}
struct record* LL(struct record *n) {
    struct record *new_root = n->left;
    n->left = new_root->right;
    new_root->right = n;
    set_height(n);
    set_height(new_root);
    return new_root;
}
struct record* RR(struct record *n) {
    struct record *new_root = n->right;
    n->right = new_root->left;
    new_root->left = n;
    set_height(n);
    set_height(new_root);
    return new_root;
}

static struct record* LR(struct record *n) {
    struct record *mid = n->left;
    struct record *new_root = mid->right;
    mid->right = new_root->left;
    n->left = new_root->right;
    new_root->left = mid;
    new_root->right = n;
    set_height(n);
    set_height(mid);
    set_height(new_root);
    return new_root;
}

static struct record* RL(struct record *n) {
    struct record *mid = n->right;
    struct record *new_root = mid->left;
    mid->left = new_root->right;
    n->right = new_root->left;
    new_root->left = n;
    new_root->right = mid;
    set_height(n);
    set_height(mid);
    set_height(new_root);
    return new_root;
}
struct record* try_rotate(struct record* n) {
    int bal = get_balance(n);
    if (bal < -1) {
        bal = get_balance(n->right);
        if (bal > 0) {
            n = RL(n);
        }
        else {
            n = RR(n);
        }
    }
    else if (bal > 1) {
        bal = get_balance(n->left);
        if (bal < 0) {
            n = LR(n);
        }
        else {
            n = LL(n);
        }
    }
    return n;
}
struct record* find_min(struct record* root){
  if(root == NULL){
    return NULL;
  }
  if(root->left ==  NULL){
    return root;
  }
  else{
    return find_min(root->left);
}
}
struct record* tree_search_avl(struct record* root,char uid[MAX_LEN]){
  if(root == NULL){
    return NULL;
  }
  if(cmp_uid(uid,root->uid)==0){
    return root;
  }
  else if(cmp_uid(uid,root->uid)==1){
    return tree_search_avl(root->right,uid);
  }
  else{
    return tree_search_avl(root->left,uid);
  }
}
void rebalance_avl_tree(struct record* root) {
    struct record *p = root;
    while (p != NULL) {
        p->height = ht(p);
        int balance = balancefactor(p);
        if (balance > 1 && balancefactor(p->left) >= 0) {
            p = LLrot(p);
            update_height(p->right);
            update_height(p);
        } else if (balance > 1 && balancefactor(p->left) < 0) {
            p = LRrot(p);
            update_height(p->left);
            update_height(p->right);
            update_height(p);
        } else if (balance < -1 && balancefactor(p->right) <= 0) {
            p = RRrot(p);
            update_height(p->left);
            update_height(p);
        } else if (balance < -1 && balancefactor(p->right) > 0) {
            p = RLrot(p);
            update_height(p->left);
            update_height(p->right);
            update_height(p);
        } else {
            p = p->parent;
        }
    }
}

struct record* adi_sp_2_del_avl(struct record* root,char uid[MAX_LEN]){
 if(root == NULL){
  return NULL;
 }
 if(cmp_uid(uid,root->uid)==0){
  if(root->left == NULL){
    struct record* ret = root->right;
    root->right = NULL;
    if(ret != NULL){
      set_height(ret);
      ret = try_rotate(ret);
    }
    return ret;
  }
  else if(root->right == NULL){
    struct record* ret = root->left;
    root->left = NULL;
    if(ret != NULL){
      set_height(ret);
      ret = try_rotate(ret);
    }    
    return ret;
  }  
  else if(root->left != NULL && root->right != NULL){
    struct record* min_node = find_min(root->right);
    // memcpy(root->uid,min_node->uid,16);
    root->right=adi_sp_2_del_avl(root->right,min_node->uid);
    min_node->left = root->left;
    min_node->right = root->right;
    set_height(min_node);
    struct record* n = try_rotate(min_node);
    return n;
  }
 }
 else if(cmp_uid(uid,root->uid)==1){
  root->right = adi_sp_2_del_avl(root->right,uid);
 }
 else{
  root->left = adi_sp_2_del_avl(root->left,uid);
 }
 set_height(root);
 struct record* n = try_rotate(root);
 return n;
} 

struct list_records* delete_friend_record(struct list_records* l, struct record* r) {
    // Check if the first element of the list should be deleted
    if (l->record == r) {
        // Create a temporary pointer to the first element
        struct list_records* temp = l;
        // Update the list pointer to the second element
        l = l->next;
        // Free the memory allocated for the first element
        free_memory(temp);
        // Return the updated list pointer
        return l;
    }
    
    // Iterate over the rest of the list to find the element to delete
    struct list_records* temp = l->next;
    struct list_records* prev = l;
    while (temp != NULL) {
        if (cmp_record(temp->record, r) == 0) {
            // Update the next pointer of the previous element to skip the element to delete
            prev->next = temp->next;
            // Clear the next pointer of the element to delete
            temp->next = NULL;
            // Free the memory allocated for the element to delete
            free_memory(temp);
            // Return the original list pointer
            return l;
        }
        temp = temp->next;
        prev = prev->next;
    }
    
    // If the element to delete was not found, return the original list pointer
    return l;
}

void dl_fr_avl(struct record* r1){
  while(r1->friends!=NULL){
    r1->friends->record->friends = delete_friend_record(r1->friends->record->friends,r1);
    struct list_records* tmp1 = r1->friends;
    r1->friends=r1->friends->next;
    free_memory(tmp1);
  }
}

struct record delete_record_avl(char uid[MAX_LEN])
{
 struct record* Adi = tree_search_avl(avl_root,uid);
  if(Adi == NULL){
    struct record dummy;
    dummy.status=-1;
    return dummy;
  }
  dl_fr_avl(Adi);  
  avl_root = adi_sp_2_del_avl(avl_root,Adi->uid);
  set_height(avl_root);
  num_avl_nodes--;
  struct record* Adi2 = Adi;
  free_memory(Adi);
  return *Adi2;
}


// Delete the record (say n) corresponding to uid from the BST.
// Remove n from the lists of friends of other records
// and release the memory for the linked list nodes.
// Release memory for all the nodes in the list of friends of n.
// Return a copy of the value of the deleted node.
// If the node is not present, return a dummy record
// with -1 in the status field.
void free_friends_list(struct list_records* node) {
    if (node == NULL) {
        return;
    }
    free_friends_list(node->next);
    free_memory(node->record);
    free_memory(node);
}


struct record delete_record_bst(char uid[MAX_LEN]) {
    struct record dummy;
    dummy.status = -1;
    struct record *curr = bst_root;
    struct record *parent = NULL;
    
    // Search for node to delete
    while (curr != NULL  && cmp_uid(curr->uid, uid) != 0) {
        parent = curr;
        if (cmp_uid(uid, curr->uid) < 0) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }
    if (curr == NULL) {
        // Node not found, return dummy record
        return dummy;
    }
    
    // Remove node from BST
    if (curr->left == NULL && curr->right == NULL) {
        // Case 1: Node has no children
        if (parent == NULL) {
            bst_root = NULL;
        } else if (parent->left == curr) {
            parent->left = NULL;
        } else {
            parent->right = NULL;
        }
    } else if (curr->left == NULL) {
        // Case 2a: Node has one child (right child)
        if (parent == NULL) {
            bst_root = curr->right;
        } else if (parent->left == curr) {
            parent->left = curr->right;
        } else {
            parent->right = curr->right;
        }
    } else if (curr->right == NULL) {
        // Case 2b: Node has one child (left child)
        if (parent == NULL) {
            bst_root = curr->left;
        } else if (parent->left == curr) {
            parent->left = curr->left;
        } else {
            parent->right = curr->left;
        }
    } else {
        // Case 3: Node has two children
        // Find inorder successor
        struct record *succ_parent = curr;
        struct record *succ = curr->right;
        while (succ->left != NULL) {
            succ_parent = succ;
            succ = succ->left;
        }
        
        // Replace node with its inorder successor
        if (succ_parent != curr) {
            succ_parent->left = succ->right;
            succ->right = curr->right;
        }
        succ->left = curr->left;
        if (parent == NULL) {
            bst_root = succ;
        } else if (parent->left == curr) {
            parent->left = succ;
        } else {
            parent->right = succ;
        }
    }
    
    // Delete friends list and return deleted node
    struct list_records *temp = curr->friends;
    while (temp != NULL) {
        struct list_records *next = temp->next;
        delete_from_friends_list(temp->record, curr->uid);
        free_memory(temp);
        temp = next;
    }
    struct record ret = *curr;
    free_memory(curr);
    num_bst_nodes--;
    return ret;
}


// Release memory for all BST nodes and their lists of friends.
// Make bst_root points to an empty tree.
void del_bst(struct record* a) {
  if (a == NULL) {
    return;
  }
  del_bst(a->left);
  del_bst(a->right);
  struct list_records* b = a->friends;
  while (b != NULL) {
    struct list_records* tmp = b->next;
    free_memory(b);
    b = tmp;
  }
  free_memory(a);
}

void destroy_bst() {
    del_bst(bst_root);
    bst_root = NULL;
    num_bst_nodes=0;
}
// Release memory for all AVL nodes and their lists of friends.
// Make avl_root points to an empty tree.
// made by me to delete 
void del_avl(struct record* a) {
  if (a == NULL) {
    return;
  }
  del_avl(a->left);
  del_avl(a->right);
  struct list_records* b = a->friends;
  while (b != NULL) {
    struct list_records* tmp = b->next;
    free_memory(b);
    b = tmp;
  }
  free_memory(a);
}

void destroy_avl() {
  del_avl(avl_root);
  avl_root = NULL;
  num_avl_nodes = 0;
}

// Make users with uids uid1 and uid2 in the BST rooted at bst_root
// friends of each other if they aren't already friends.
// The friends field in "struct record" stores the
// head of the linked list of friends of a given user.
// To make the user with record A a friend of the user with record B, 
// add A to B's list of friends and add B to A's list of friends.
// Return 1 if uid1 and uid2 are already friends before this call.
// Return 0 if they become friends during this call.
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
int make_friends_bst(char uid1[MAX_LEN], char uid2[MAX_LEN])
{
    struct record *p = find_record(bst_root, uid1);
    struct record *q = find_record(bst_root, uid2);
    if (are_friends(p, q))
        return 1;
    add_friend(p, q);
    add_friend(q, p);

    return 0;
}



// Make users with uids uid1 and uid2 in the AVL tree rooted at avl_root
// friends of each other if they aren't already friends.
// The friends field in "struct record" stores the
// head of the linked list of friends of a given user.
// To make the user with record A a friend of the user with record B, 
// add A to B's list of friends and add B to A's list of friends.
// Return 1 if uid1 and uid2 are already friends before this call.
// Return 0 if they become friends during this call.
int make_friends_avl(char uid1[MAX_LEN], char uid2[MAX_LEN])
{
    struct record *p = find_record(avl_root, uid1);
    struct record *q = find_record(avl_root, uid2);
    if (are_friends(p, q))
        return 1;
    add_friend(p, q);
    add_friend(q, p);

    return 0;
}
