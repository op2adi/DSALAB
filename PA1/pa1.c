#include "../include/common.h"
#include "pa1.h"


// don't remove these globals
static struct record *record_arr = NULL;
static size_t max_capacity = 0;
static size_t num_records = 0;

// don't modify this function
struct record *get_record_arr()
{
  return record_arr;
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
// compare two records when uid is key
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

#define NOT_IMPLEMENTED { printf("%s still needs to be implemented!\n", __func__); abort(); }

// return the total number of records in the
// record_arr
size_t get_num_records()
{
    return (size_t) num_records;
}

// insert record r at the end of the record array
// if the record array is full, allocate
// a new buffer of double size, copy data
// from old buffer to the new buffer
// delete the old buffer
// make record_arr point to the new buffer
void insert_record(struct record r)
{
    if (num_records == max_capacity) { 
        if (num_records==0){
          max_capacity=1;
        }
        else{
            max_capacity*=2;
        }
        struct record *new_record_arr = allocate_memory(max_capacity * sizeof(struct record));
        memcpy(new_record_arr, record_arr, num_records * sizeof(struct record));
        if (max_capacity!=1){
          free_memory(record_arr);
        }
        record_arr = new_record_arr;
    }
    record_arr[num_records++] = r;
}



// return the record corresponding to the uid
// use the linear search algorithm
// you can also use cmp_uid routine defined earlier
// if none of the records contain the input uid
// return a dummy record with status == -1
struct record search_record_linear(char uid[MAX_LEN])
{
    for (size_t i = 0; i < num_records; ++i) {
        if (cmp_uid(uid, record_arr[i].uid) == 0) {
            return record_arr[i];
        }
    }
    struct record dummy = { .status = -1 };
    return dummy;
}

// return the record corresponding to the uid
// use the binary search algorithm
// you can assume that the record_arr is alaredy sorted
// if none of the records contain the input uid
// return a dummy record with status == -1
struct record search_record_binary(char uid[MAX_LEN])
{
    struct record dummy = { .status = -1 };

    int left = 0;
    int right = num_records - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp_result = cmp_uid(record_arr[mid].uid, uid);

        if (cmp_result == 0) {
            return record_arr[mid];
        }
        else if (cmp_result < 0) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return dummy;
}
// delete record correspondig to the uid
// if the corresponding record doesn't exist
// return a dummy record with status == -1
// Otherwise, return a copy of the record to the caller

// resize the record array in the following manner
// let's say, x is the size of the array
// and after deletion only x/4 
// space in the array is occupied, in this
// case, allocate an array of size x/2,
// copy records from old array to the new
// array, and delete the old array,
// make record_arr point to the new array
struct record delete_record(char uid[MAX_LEN]) {
    struct record dummy = { .status = -1 };
    int i;
    for (i = 0; i < num_records; i++) {
        if (cmp_uid(uid, record_arr[i].uid) == 0) {
            struct record deleted_record = record_arr[i];
            for (int j = i; j < num_records - 1; j++) {
                record_arr[j] = record_arr[j+1];
            }
            num_records--;
            if ((num_records <= max_capacity/4)) {
                max_capacity /= 4;
                struct record *new_record_arr = allocate_memory(max_capacity * sizeof(struct record));
                memcpy(new_record_arr, record_arr, num_records * sizeof(struct record));
                free_memory(record_arr);
                record_arr = new_record_arr;
            }
            return deleted_record;
        }
    }
    return dummy;
}



// delete all records in the record_arr
// free record_arr
void delete_all_records()
{
    free_memory(record_arr);
    record_arr = NULL;
    num_records = 0;
    max_capacity = 0;
}

// sort the record array using quick sort
// use cmp_record implementaion to compare two records
// making quicksort function
void quick_sort(struct record *arr, int start, int end)
{
    if (start >= end) {
        return;
    }

    // choose the pivot as the last element in the array
    char pivot[20];
    memcpy(pivot, arr[end].uid,20);

    // partition the array into two halves based on the pivot element
    int i = start - 1;
    for (int j = start; j < end; j++) {
        if (cmp_uid(arr[j].uid, pivot) < 0) {
            i++;
            struct record temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    i++;
    struct record temp = arr[i];
    arr[i] = arr[end];
    arr[end] = temp;

    // recursively sort the two halves
    quick_sort(arr, start, i - 1);
    quick_sort(arr, i + 1, end);
}

void sort_records_quick() {
    quick_sort(record_arr, 0, num_records-1);
    
}



// sort the record_arr using merge sort
// use cmp_record implementaion to compare two records
// merge sort algorithm
static void merge(struct record *arr, int left, int mid, int right)
{
    struct record *temp = allocate_memory((right - left + 1) * sizeof(struct record));

    int i = left;
    int j = mid + 1;
    int k = 0;

    while (i <= mid && j <= right) {
        if (cmp_record(&arr[i], &arr[j]) < 0) {
            temp[k++] = arr[i++];
        }
        else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    while (j <= right) {
        temp[k++] = arr[j++];
    }

    for (i = left, k = 0; i <= right; ++i, ++k) {
        arr[i] = temp[k];
    }
    free_memory(temp);
}

// recursive function to merge sort an array
static void merge_sort(struct record *arr, int left, int right)
{
    if (left >= right) {
        return;
    }

    int mid = (left + right) / 2;

    merge_sort(arr, left, mid);
    merge_sort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}
void sort_records_merge()
{
  merge_sort(record_arr, 0, num_records - 1);
}

// sort the record_arr using selection sort
// use cmp_record implementaion to compare two records
void sort_records_selection() {
    for (size_t i = 0; i < num_records - 1; i++) {
        size_t min_index = i;
        for (size_t j = i + 1; j < num_records; j++) {
            if (cmp_uid(record_arr[j].uid, record_arr[min_index].uid) < 0) {
                min_index = j;
            }
        }
        if (min_index != i) {
            struct record temp = record_arr[i];
            record_arr[i] = record_arr[min_index];
            record_arr[min_index] = temp;
        }
    }
}


// return the number of records corresponding to the input name
// use linear search to obtain the results
size_t get_num_records_with_name_linear(char name[MAX_LEN])
{
    size_t count = 0;
    for (int i = 0; i < num_records; i++) {
        if (strcmp(record_arr[i].name, name) == 0) {
            count++;
        }
    }
    return count;
}


// implement quick sort algorithm to sort the 
// record_arr using name as key

void swap2(struct record *a, struct record *b) {
    struct record tmp = *a;
    *a = *b;
    *b = tmp;
}

int partition2(struct record arr[], int low, int high) {
    char pivot[50];
    strcpy(pivot, arr[high].name);
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        if (strcmp(arr[j].name, pivot) < 0) {
            i++;
            swap2(&arr[i], &arr[j]);
        }
    }
    swap2(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quicksort2(struct record arr[], int low, int high) {
    if (low < high) {
        int pi = partition2(arr, low, high);
        quicksort2(arr, low, pi - 1);
        quicksort2(arr, pi + 1, high);
    }
}
void rearrange_data(){
    quicksort2(record_arr, 0, num_records - 1);

}


// return the number of records corresponding to the input name
// use binary search to obtain the results
// you can assume that the caller has sorted
// data using name as key before making these queries
size_t get_num_records_with_name_binary(char name[MAX_LEN])
{
    int left = 0, right = num_records - 1;
    int first_idx = -1, last_idx = -1;

    // find first occurrence of name using binary search
    while (left <= right) {
        int mid = (left + right) / 2;
        int cmp = strcmp(record_arr[mid].name, name);

        if (cmp == 0) {
            // found a match, but there might be earlier matches
            first_idx = mid;
            right = mid - 1;
        } else if (cmp < 0) {
            // the name is after mid, search right half
            left = mid + 1;
        } else {
            // the name is before mid, search left half
            right = mid - 1;
        }
    }

    if (first_idx == -1) {
        // no match found
        return (size_t)(0);
    }

    // find last occurrence of name using binary search
    left = 0, right = num_records - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        int cmp = strcmp(record_arr[mid].name, name);

        if (cmp == 0) {
            // found a match, but there might be later matches
            last_idx = mid;
            left = mid + 1;
        } else if (cmp < 0) {
            // the name is after mid, search right half
            left = mid + 1;
        } else {
            // the name is before mid, search left half
            right = mid - 1;
        }
    }

    // return number of records with the given name
    return (size_t)(last_idx - first_idx + 1);;
}

