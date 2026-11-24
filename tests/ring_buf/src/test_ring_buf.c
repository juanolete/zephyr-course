/*
 * Ring Buffer Module - Homework Test Skeleton
 *
 * test_fresh_state is provided as a worked example. Fill in the remaining
 * 7 ZTEST bodies according to TEST_SPEC.md. Stubs call ztest_test_skip()
 * so the binary builds and runs cleanly before each test is implemented.
 *
 * Run:
 *   west twister -T tests/ring_buf -p native_sim
 */

#include <zephyr/ztest.h>
#include <errno.h>

#include "ring_buf.h"

/*
 * Shared before hook: every suite reinitialises the ring buffer with a
 * capacity of 4 so tests start from a clean, known state. Capacity 4 is
 * enough to exercise FIFO order (push 1, 2, 3) and overflow (full at 4).
 */
static void before(void *f)
{
	ARG_UNUSED(f);
	rb_init(4);
}

/*
 * ============================================================================
 * Test Suite: ring_buf_init
 *
 * Initial state and re-initialization behaviour.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_init, NULL, NULL, before, NULL, NULL);

/* PROVIDED — study this test before writing the rest. */
ZTEST(ring_buf_init, test_fresh_state)
{
	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
	zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

ZTEST(ring_buf_init, test_reinit_clears_state)
{
	zassume_ok(rb_push(99), "Can't push into buffer");
   zassert_equal(rb_count(), 1, "Fresh buffer count must be 1 after pushing");

   zassert_ok(rb_init(4), "rb_init(4) must succeed");
   zassert_true(rb_is_empty(), "Fresh buffer must be empty");
	zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

ZTEST(ring_buf_init, test_init_out_of_bounds)
{
   zassert_equal(rb_init(0), -EINVAL, "rb_init(0) must fail");
   zassert_equal(rb_init(RING_BUF_MAX_CAPACITY + 1), -EINVAL, 
                "rb_init(RING_BUF_MAX_CAPACITY + 1) must fail");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_push_pop
 *
 * Single push/pop round-trip, FIFO order, full error path.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_push_pop, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_push_pop, test_single_push_pop)
{
   zassert_ok(rb_push(42), "Can't push into buffer");

   int v = 0;
   zassert_ok(rb_pop(&v), "rb_pop() must succeed");
   zassert_equal(v, 42, "Value must be 42");

   zassert_true(rb_is_empty(), "Fresh buffer must be empty");
}

ZTEST(ring_buf_push_pop, test_fifo_order)
{
   zassert_ok(rb_push(1), "Can't push 1 into buffer");
   zassert_ok(rb_push(2), "Can't push 2 into buffer");
   zassert_ok(rb_push(3), "Can't push 3 into buffer");

   int v = 0;
   zassert_ok(rb_pop(&v), "rb_pop() must succeed");
   zassert_equal(v, 1, "Value must be 1");

   zassert_ok(rb_pop(&v), "rb_pop() must succeed");
   zassert_equal(v, 2, "Value must be 2");

   zassert_ok(rb_pop(&v), "rb_pop() must succeed");
   zassert_equal(v, 3, "Value must be 3");

   zassert_true(rb_is_empty(), "Buffer must be empty after rb_pop()");
}

ZTEST(ring_buf_push_pop, test_push_full_returns_enospc)
{
   zassume_ok(rb_push(1), "Can't push 1 into buffer");
   zassume_ok(rb_push(2), "Can't push 2 into buffer");
   zassume_ok(rb_push(3), "Can't push 3 into buffer");
   zassume_ok(rb_push(4), "Can't push 4 into buffer");

   zassert_true(rb_is_full(), "Buffer must be full");
   zassert_equal(rb_push(99), -ENOSPC);

   zassert_equal(rb_count(), 4, "Buffer size must be 4");
}

ZTEST(ring_buf_boundaries, test_pop_empty_buffer_returns_enodata)
{
   int v = 0;
   zassert_equal(rb_pop(&v), -ENODATA, "rb_pop() must fail for an empty buffer");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_boundaries
 *
 * Peek semantics and NULL-pointer boundary conditions.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_boundaries, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_boundaries, test_peek_does_not_consume)
{
   zassume_ok(rb_push(7), "rb_push(7) must succeed");

   int v = 0;
   zassert_ok(rb_peek(&v), "rb_peek() must succeed");
   zassert_equal(v, 7, "Value peek must be 7");
   zassert_ok(rb_peek(&v), "rb_peek() must succeed");
   zassert_equal(v, 7, "Value peek must be 7");
   
   zassert_equal(rb_count(), 1, "Buffer size must be 1");
}

ZTEST(ring_buf_boundaries, test_pop_null_returns_einval)
{
   zassert_equal(rb_pop(NULL), -EINVAL, "rb_pop(NULL) must fail");
}

ZTEST(ring_buf_boundaries, test_is_full_after_fill)
{
   zassume_ok(rb_push(1), "Can't push 1 into buffer");
   zassume_ok(rb_push(2), "Can't push 2 into buffer");
   zassume_ok(rb_push(3), "Can't push 3 into buffer");
   zassume_ok(rb_push(4), "Can't push 4 into buffer");

   zassert_true(rb_is_full(), "Buffer must be full");
   zassert_equal(rb_count(), 4, "Buffer size must be 4");
}

ZTEST(ring_buf_boundaries, test_peek_null_returns_einval)
{
   zassume_ok(rb_push(1), "Can't push 1 into buffer");
   zassert_equal(rb_peek(NULL), -EINVAL, "rb_peek(NULL) must fail");
}

ZTEST(ring_buf_boundaries, test_peek_empty_buffer_returns_enodata)
{
   int v = 0;
   zassert_equal(rb_peek(&v), -ENODATA, "rb_peek() must fail for an empty buffer");
}
