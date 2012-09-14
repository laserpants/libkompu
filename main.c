#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include "comp.h"
#include "tmachine.h"
#include "lcalc.h"

static void
comp_test()
{
    {
        /*
         *  f(x, y) = x + y
         */

        struct node *f, **g;
        int y;

        g = node_array_new(2);
        g[0] = projection_node_new(0);
        g[1] = NULL;

        f = recursion_node_new(projection_node_new(0),
                               composition_node_new(successor_node_new(), g));
        //

        int x[2] = {5, 12};
        y = compute(f, x, 2);
        printf("y = %i\n", y);
        assert(y == 17);

        //

        node_destroy(f);
    }

    {
        /*
         *  f(x) = 2
         */

        struct node *f, **g, **h;
        int y;

        h = node_array_new(2);
        h[0] = zero_node_new();
        h[1] = NULL;

        g = node_array_new(2);
        g[0] = composition_node_new(successor_node_new(), h);
        g[1] = NULL;

        f = composition_node_new(successor_node_new(), g);

        int x[2] = {5, 12};
        y = compute(f, x, 2);
        printf("y = %i\n", y);
        assert(y == 2);

        node_destroy(f);
    }

    {
        /*
         *  f(x, y) = x * y
         */

        struct node *add, *mult, **g, **h;
        int y;

        g = node_array_new(2);
        g[0] = projection_node_new(0);
        g[1] = NULL;

        add = recursion_node_new(projection_node_new(0),
                                 composition_node_new(successor_node_new(), g));

        h = node_array_new(3);
        h[0] = projection_node_new(0);
        h[1] = projection_node_new(1);
        h[2] = NULL;

        mult = recursion_node_new(zero_node_new(), composition_node_new(add, h));
        int x[2] = {5, 6};
        y = compute(mult, x, 2);
        printf("y = %i\n", y);
        assert(y == 30);

        node_destroy(mult);
    }

    {
        /*
         *  f(x, y) = x ^ y
         */

        struct node *one, *add, *mult, *exp, **g, **h, **j, **k;
        int y;

        g = node_array_new(2);
        g[0] = projection_node_new(0);
        g[1] = NULL;

        add = recursion_node_new(projection_node_new(0),
                                 composition_node_new(successor_node_new(), g));

        h = node_array_new(3);
        h[0] = projection_node_new(0);
        h[1] = projection_node_new(1);
        h[2] = NULL;

        mult = recursion_node_new(zero_node_new(), composition_node_new(add, h));

        j = node_array_new(2);
        j[0] = zero_node_new();
        j[1] = NULL;

        one = composition_node_new(successor_node_new(), j);

        k = node_array_new(3);
        k[0] = projection_node_new(0);
        k[1] = projection_node_new(1);
        k[2] = NULL;

        exp = recursion_node_new(one, composition_node_new(mult, k));

        int x[2] = {4, 4};
        y = compute(exp, x, 2);
        printf("y = %i\n", y);
        assert(y == 256);

        node_destroy(exp);
    }

    {
        /*
         *  Factorial:
         *
         *  f(x) = x!
         */

        struct node *one, *add, *mult, *fact, **g, **h, **i, **j, **k;
        int y;

        g = node_array_new(2);
        g[0] = projection_node_new(0);
        g[1] = NULL;

        add = recursion_node_new(projection_node_new(0),
                                 composition_node_new(successor_node_new(), g));

        h = node_array_new(3);
        h[0] = projection_node_new(0);
        h[1] = projection_node_new(1);
        h[2] = NULL;

        mult = recursion_node_new(zero_node_new(), composition_node_new(add, h));

        k = node_array_new(2);
        k[0] = projection_node_new(1);
        k[1] = NULL;

        i = node_array_new(3);
        i[0] = projection_node_new(0);
        i[1] = composition_node_new(successor_node_new(), k);
        i[2] = NULL;

        j = node_array_new(2);
        j[0] = zero_node_new();
        j[1] = NULL;

        one = composition_node_new(successor_node_new(), j);

        fact = recursion_node_new(one, composition_node_new(mult, i));

        int x = 5;
        y = compute(fact, &x, 1);
        printf("y = %i\n", y);
        assert(y == 120);

        node_destroy(fact);
    }

    {
        /*
         *  Iszero:
         *
         *         | 1   if x = 0
         *  f(x) = | 0   if x > 0
         */

        struct node *iszero, *one, **j;
        int y;

        j = node_array_new(2);
        j[0] = zero_node_new();
        j[1] = NULL;

        one = composition_node_new(successor_node_new(), j);

        iszero = recursion_node_new(one, zero_node_new());

        int x = 0;
        y = compute(iszero, &x, 1);
        printf("y = %i\n", y);
        assert(y == 1);

        x = 5;
        y = compute(iszero, &x, 1);
        printf("y = %i\n", y);
        assert(y == 0);

        node_destroy(iszero);
    }

    {
        /*
         *  Predecessor:
         *
         *  f(x) = x - 1,      if x > 0
         *
         *  Monus:
         *
         *  f(x, y) = x - y,   if x > y
         */

        struct node **g, *monus, *pred;
        int y, x;

        pred = recursion_node_new(zero_node_new(), projection_node_new(1));

        x = 4;
        y = compute(pred, &x, 1);
        assert(3 == y);

        g = node_array_new(2);
        g[0] = projection_node_new(0);
        g[1] = NULL;

        monus = recursion_node_new(projection_node_new(0),
                                   composition_node_new(pred, g));

        int xs[2] = {17, 3};
        y = compute(monus, xs, 2);
        printf("y = %i\n", y);
        assert(14 == y);

        node_destroy(monus);
    }

    // divisibility, primes etc. !?

    printf("-------------------------\n"
           "All assertions passed ok.\n"
           "-------------------------\n");

    malloc_stats();
}

static void
tmachine_test()
{
    struct tm_machine *machine;
    struct tm_tape *tape;

    machine = t_machine_new();

    t_machine_insert_states(machine, 3);

    t_machine_add_instruction(machine, 0 /* q1 */, 0 /* q1 */, 1 /* a */, 1 /* a */, TM_RIGHT);
    t_machine_add_instruction(machine, 0 /* q1 */, 0 /* q1 */, 2 /* b */, 2 /* b */, TM_RIGHT);
    t_machine_add_instruction(machine, 0 /* q1 */, 1 /* q2 */, 0 /* - */, 1 /* a */, TM_LEFT);
    t_machine_add_instruction(machine, 1 /* q2 */, 1 /* q2 */, 1 /* a */, 1 /* a */, TM_LEFT);
    t_machine_add_instruction(machine, 1 /* q2 */, 1 /* q2 */, 2 /* b */, 2 /* b */, TM_LEFT);
    t_machine_add_instruction(machine, 1 /* q2 */, 2 /* q3 */, 0 /* - */, 0 /* - */, TM_RIGHT);


    tape = t_machine_tape_new();
    t_machine_tape_append_symbol(tape, 1);
    t_machine_tape_append_symbol(tape, 1);
    t_machine_tape_append_symbol(tape, 2);
    t_machine_tape_append_symbol(tape, 2);

    if (t_machine_run(machine, tape) < 0) {
        printf("Error!\n");
    }

    t_machine_tape_destroy(tape);
    t_machine_destroy(machine);

    malloc_stats();
}

static void
lcalc_test()
{
    /*
     *  λ0.λ1.1     (Church numeral zero)
     */
    struct lambda_term *zero = lambda_abstraction_new(0,
                                                      lambda_abstraction_new(1,
                                                                             lambda_variable_new(1)));

    /*
     *  λ2.λ3.λ4.3(234)     (Successor)
     */

    struct lambda_term *succ = lambda_abstraction_new(2,
                                                      lambda_abstraction_new(3,
                                                                             lambda_abstraction_new(4, lambda_application_new(lambda_variable_new(3),
                                                                                                                              lambda_application_new(lambda_application_new(lambda_variable_new(2),
                                                                                                                                                                            lambda_variable_new(3)),
                                                                                                                                                     lambda_variable_new(4))))));
    // ...

    struct lambda_term *a = lambda_application_new(succ, zero);

    lambda_term_dump(a);

    lamba_term_normal_order_reduce_step(&a);
    lambda_term_dump(a);

    lamba_term_normal_order_reduce_step(&a);
    lambda_term_dump(a);

    lamba_term_normal_order_reduce_step(&a);
    lambda_term_dump(a);

    lambda_term_destroy(a);

    malloc_stats();
}

int
main(void)
{
    if (0 == 1)
        comp_test();    // tmp

    if (0 == 1)
        tmachine_test();    // tmp

    lcalc_test();

    return 0;
}