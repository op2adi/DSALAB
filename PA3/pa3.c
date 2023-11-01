#include "../include/common.h"
#include "pa3.h"

// Global variables
static struct trie_node* trie_root = NULL;
size_t num_msgs = 0;

// Utility function to allocate memory
static void* allocate_memory(size_t size) {
    return __mymalloc(size);
}

// Utility function to free memory
static void free_memory(void* ptr) {
    __myfree(ptr);
}

// ---------------- Add your code below this line ----------------- //

#define POST_EVENT 1
#define DELETE_EVENT 2

struct trie_node* find_square_node(char msg[MAX_MSG_LEN]) {
    struct trie_node* current = trie_root;

    int i = 0;
    while (msg[i] != '\0') {
        struct trie_node* child = current->child;
        while (child != NULL) {
            if (child->val == msg[i]) {
                current = child;
                break;
            }
            child = child->next;
        }

        if (child == NULL) {
            return NULL; // Square node not found
        }

        i++;
    }

    return current;
}

struct list_events* get_history(char msg[MAX_MSG_LEN]) {
    struct trie_node* node = find_square_node(msg);

    if (node == NULL) {
        return NULL; // Message not found in the trie
    }

    return node->history;
}

struct list_events* get_clean_history(char msg[MAX_MSG_LEN]) {
    struct trie_node* node = find_square_node(msg);

    if (node == NULL) {
        return NULL; // Message not found in the trie
    }

    struct list_events* current = node->history;
    struct list_events* prev = NULL;

    while (current != NULL) {
        if (current->action == DELETE_EVENT) {
            if (prev == NULL) {
                node->history = current->next;
            } else {
                prev->next = current->next;
            }
            struct list_events* temp = current;
            current = current->next;
            free_memory(temp);
        } else {
            prev = current;
            current = current->next;
        }
    }

    return node->history;
}

struct trie_node* insert_message(struct trie_node* root, char msg[MAX_MSG_LEN]) {
    if (root == NULL) {
        root = (struct trie_node*)allocate_memory(sizeof(struct trie_node));
        root->val = msg[0];
        root->child = NULL;
        root->next = NULL;
        root->parent = NULL;
        root->history = NULL;
    }

    struct trie_node* current = root;
    int i = 0;
    while (msg[i] != '\0') {
        struct trie_node* child = current->child;
        struct trie_node* prev = NULL;
        while (child != NULL && child->val != msg[i]) {
            prev = child;
            child = child->next;
        }

        if (child == NULL) {
            child = (struct trie_node*)allocate_memory(sizeof(struct trie_node));
            child->val = msg[i];
            child->child = NULL;
            child->next = NULL;
            child->parent = current;
            child->history = NULL;

            if (prev == NULL) {
                current->child = child;
            } else {
                prev->next = child;
            }
        }

        current = child;
        i++;
    }

    return current;
}

void post_a_msg(char msg[MAX_MSG_LEN], struct record* r) {
    // Insert the message into the trie
    struct trie_node* square_node = insert_message(trie_root, msg);

    // Update the history in the square node
    struct list_events* new_event = (struct list_events*)allocate_memory(sizeof(struct list_events));
    new_event->action = POST_EVENT;
    new_event->record = r;
    new_event->next = square_node->history;
    square_node->history = new_event;

    // Add the square node to the list of posts in struct record
    struct list_posts* new_post = (struct list_posts*)allocate_memory(sizeof(struct list_posts));
    new_post->node = square_node;
    new_post->next = r->posts;
    r->posts = new_post;

    // Increment the number of messages
    num_msgs++;
}

int delete_latest_post(struct record *r) {
    // Check if the user has any posts
    if (r->posts == NULL) {
        return 0;
    }

    // Get the latest post
    struct list_posts *latest_post = r->posts;
    struct trie_node *latest_node = latest_post->node;

    // Remove the latest post from the list
    r->posts = latest_post->next;

    // Update the parent's child pointer in the trie
    struct trie_node *parent = latest_node->parent;
    if (parent != NULL) {
        if (parent->child == latest_node) {
            parent->child = latest_node->next;
        } else {
            struct trie_node *prev_sibling = parent->child;
            while (prev_sibling->next != latest_node) {
                prev_sibling = prev_sibling->next;
            }
            prev_sibling->next = latest_node->next;
        }
    }

    // Free the memory for the latest post and trie node
    free_memory(latest_post);
    free_memory(latest_node);

    // Update the history of the latest post
    struct list_events *delete_event = (struct list_events *)allocate_memory(sizeof(struct list_events));
    delete_event->action = DELETE_EVENT;
    delete_event->record = r;
    delete_event->next = latest_node->history;
    latest_node->history = delete_event;

    return 1;
}

void delete_all_posts(struct record *r) {
    // Traverse the list of posts and free the memory
    struct list_posts *current = r->posts;
    while (current != NULL) {
        struct list_posts *next = current->next;
        free_memory(current);
        current = next;
    }

    // Set the posts list to NULL
    r->posts = NULL;
}

int read_latest_post(struct record* r, char msg[MAX_MSG_LEN]) {
    if (r->posts == NULL) {
        return 0; // No messages in the record
    }

    struct list_posts* latest_post = r->posts;
    struct trie_node* latest_node = latest_post->node;
    // Get the message associated with the latest post from the square node's value
    msg[0] = latest_node->val;

    return 1;
}

void destroy_trie_node(struct trie_node* node) {
    if (node == NULL) {
        return;
    }

    // Recursively destroy child nodes
    if (node->child != NULL) {
        destroy_trie_node(node->child);
    }

    // Recursively destroy sibling nodes
    if (node->next != NULL) {
        destroy_trie_node(node->next);
    }

    // Destroy the history list
    struct list_events* current = node->history;
    while (current != NULL) {
        struct list_events* next = current->next;
        free_memory(current);
        current = next;
    }

    // Free the memory for the node
    free_memory(node);
}

void destroy_all_trie_nodes(struct trie_node* root) {
    if (root == NULL) {
        return;
    }

    // Destroy each child node
    struct trie_node* current = root->child;
    while (current != NULL) {
        struct trie_node* next = current->next;
        destroy_trie_node(current);
        current = next;
    }
}

void destroy_trie() {
    destroy_all_trie_nodes(trie_root);
    trie_root = NULL;
    num_msgs = 0;
}
