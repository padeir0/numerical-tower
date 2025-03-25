# Numerical Tower

I need some numerical tower shenanigans for
my lisp interpreter. So here's where it all begins.

## Constraints

The lisp is pure, so all operations must keep operands untouched,
with an explicit scratch space.
Example:

```c
number add(context* ctx, const number* a, const number* b, number* out);
```

Operations on decimal numbers need explicit precision.
In this case precision is stored inside the `Context` object.

Operations must minimize allocation, increasing the size of a number
is fine, but allocating other numbers as scratch space is not. For this,
we have scratch number registers inside the `Context` object.

All calls to `malloc` must occur inside `alloc_*` procedures,
so that we can swap the allocator later.
