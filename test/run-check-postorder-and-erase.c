#include <linux/rbtree.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

struct test_node {
	unsigned key;
	struct rb_node rb;
	unsigned mark;
};

static void insert(struct test_node *node, struct rb_root *root)
{
	struct rb_node **new = &root->rb_node, *parent = NULL;
	unsigned key = node->key;

	while (*new) {
		parent = *new;
		if (key < rb_entry(parent, struct test_node, rb)->key)
			new = &parent->rb_left;
		else
			new = &parent->rb_right;
	}

	rb_link_node(&node->rb, parent, new);
	rb_insert_color(&node->rb, root);
}

static inline void erase(struct test_node *node, struct rb_root *root)
{
	rb_erase(&node->rb, root);
}

int main(void)
{
	srand(time(NULL));

	struct rb_root root = RB_ROOT;
	struct test_node nodes[100];

	size_t i;
	for (i = 0; i < ARRAY_SIZE(nodes); i++) {
		nodes[i] = (struct test_node) { .key = rand(), };

		insert(nodes + i, &root);
	}

	struct test_node *p, *n;
	rbtree_postorder_for_each_entry_safe(p, n, &root, rb) {
		rb_erase(&p->rb, &root);
		p->mark = 1;
	}

	for (i = 0; i < ARRAY_SIZE(nodes); i++) {
		if (!nodes[i].mark) {
			printf("node %zu unmarked!!!\n", i);
		}
	}

	return 0;
}
