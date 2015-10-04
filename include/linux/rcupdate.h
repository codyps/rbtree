#ifndef RBTREE_LINUX_RCUPDATE_H_
#define RBTREE_LINUX_RCUPDATE_H_

#include <linux/compiler.h>
#define rcu_assign_pointer(p, v) WRITE_ONCE(p, v)

#endif
