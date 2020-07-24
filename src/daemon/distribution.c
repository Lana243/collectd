//
// Created by Svetlana Shmidt on 24.07.2020.
//

#include "distribution.h"

typedef struct bucket_s {
  uint64_t bucket_counter;
  double minimum, maximum;
} bucket_t;

struct distribution_s {
  bucket_t *tree;
  size_t num_buckets;
};

size_t left_child_index(size_t node_index, size_t left, size_t right) {
  return node_index + 1;
}

size_t right_child_index(size_t node_index, size_t left, size_t right) {
  size_t mid = (left + right) / 2;
  return node_index + 2 * (mid - left + 1);
}

bucket_t merge_buckets(bucket_t left_child, bucket_t right_child) {
  return (bucket_t) {
      .bucket_counter = left_child.bucket_counter + right_child.bucket_counter,
      .minimum = left_child.minimum,
      .maximum = right_child.maximum,
  };
}

void build_tree(distribution_t *d, bucket_t *buckets, size_t node_index, size_t left, size_t right) {
  if (left > right)
    return;
  if (left == right) {
    d->tree[node_index] = buckets[left];
    return;
  }
  size_t mid = (left + right) / 2;
  size_t left_child = left_child_index(node_index, left, right);
  size_t right_child = right_child_index(node_index, left, right);
  build_tree(d, buckets, left_child, left, mid);
  build_tree(d, buckets, right_child, mid + 1, right);
  d->tree[node_index] = merge_buckets(d->tree[left_child], d->tree[right_child]);
}

distribution_t* distribution_new_linear(size_t num_buckets, double size) {
  bucket_t *bucket_array = calloc(num_buckets, sizeof(bucket_t));
  for (size_t i = 0; i < num_buckets; i++) {
    bucket_array[i].minimum = i * size;
    bucket_array[i].maximum = (i + 1) * size;
  }

  distribution_t *new_distribution = calloc(1, sizeof(distribution_t));
  new_distribution->tree = calloc(2 * num_buckets - 1, sizeof(bucket_t));
  new_distribution->num_buckets = num_buckets;
  build_tree(new_distribution, bucket_array, 0, 0, num_buckets - 1);
  free(bucket_array);
  return new_distribution;
}

int main() {
  distribution_t *p = distribution_new_linear(5, 2);
}