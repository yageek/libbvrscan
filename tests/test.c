#include "munit.h"
#include <io.h>
#include <bvrcore.h>
#include <bvrcore_private.h>

/* A test runs various assertions, then calls PASS(), FAIL(), or SKIP(). */
MunitResult matrix_init_free(const MunitParameter params[], void *user_data_or_fixture)
{
    bvr_mat8_t *mat = bvr_mat8_new(1, 10);

    // Should initialize with 0
    munit_assert_uint8(bvr_mat_get(mat, 0, 0), ==, 0);
    munit_assert_uint8(bvr_mat_get(mat, 0, 1), ==, 0);

    bvr_mat_set(mat, 0, 0, 255);
    munit_assert_uint8(bvr_mat_get(mat, 0, 0), ==, 255);

    bvr_mat_free(mat);
    return MUNIT_OK;
}

MunitResult filter_convert_gray(const MunitParameter params[], void *user_data_or_fixture)
{
    const char *input_image_name = "samples/small_bvr.png";

    bvr_io_image_source_t *src = NULL;
    MunitResult t_res = MUNIT_FAIL;
    src = bvr_io_load_png(input_image_name);
    if (src == NULL)
        goto out;

    bvr_mat8_t *real = bvr_io_rgb_src(src);
    if (real == NULL)
        goto free_source;

    bvr_mat8_t *gray = bvr_filter_create_grayscale(real, 3);
    if (gray == NULL)
        goto free_source;

    int res = bvr_io_image_grayscale_write(gray, "outputs/small_bvr_gray.png");
    if (res < 0)
        goto free_image;

    t_res = MUNIT_OK;

    goto out;
free_image:
    bvr_mat_free(gray);
    bvr_mat_free(real);
free_source:
    bvr_io_image_source_free(src);
out:
    return t_res;
}

MunitResult filter_image_integral(const MunitParameter params[], void *user_data_or_fixture)
{
    uint8_t src[] = {5, 2, 3, 6};
    bvr_mat8_t img = (bvr_mat8_t){2, 2, &src[0]};

    double res[] = {5, 7, 8, 16};

    bvr_mat_real_t *computed = bvr_filter_create_image_integral(&img);
    MunitResult t_res = MUNIT_FAIL;
    if (computed == NULL)
        goto out;

    munit_assert_memory_equal(4, computed->content, &res[0]);
    t_res = MUNIT_OK;
    bvr_mat_free(computed);
out:
    return t_res;
}

MunitResult filter_image_integral_2(const MunitParameter params[], void *user_data_or_fixture)
{

    uint8_t src[] = {5, 2, 5, 2, 3, 6, 3, 6, 5, 2, 5, 2, 3, 6, 3, 6};
    bvr_mat8_t img = (bvr_mat8_t){4, 4, &src[0]};

    double res[] = {5, 7, 12, 14, 8, 16, 24, 32, 13, 23, 36, 46, 16, 32, 48, 64};

    bvr_mat_real_t *computed = bvr_filter_create_image_integral(&img);
    MunitResult t_res = MUNIT_FAIL;
    if (computed == NULL)
        goto out;

    munit_assert_memory_equal(16, computed->content, &res[0]);
    t_res = MUNIT_OK;
    bvr_mat_free(computed);
out:
    return t_res;
}

MunitResult filter_image_integral_3(const MunitParameter params[], void *user_data_or_fixture)
{

    uint8_t src[] = {31, 2, 4, 33, 5, 36, 12, 26, 9, 10, 29, 25, 13, 17, 21, 22, 20, 18, 24, 23, 15, 16, 14, 19, 30, 8, 28, 27, 11, 7, 1, 35, 34, 3, 32, 6};
    bvr_mat8_t img = (bvr_mat8_t){6, 6, &src[0]};

    double res[] = {31, 33, 37, 70, 75, 111, 43, 71, 84, 127, 161, 222, 56, 101, 135, 200, 254, 333, 80, 148, 197, 278, 346, 444, 110, 186, 263, 371, 450, 555, 111, 222, 333, 444, 555, 666};

    bvr_mat_real_t *computed = bvr_filter_create_image_integral(&img);
    MunitResult t_res = MUNIT_FAIL;
    if (computed == NULL)
        goto out;

    munit_assert_memory_equal(36, computed->content, &res[0]);
    t_res = MUNIT_OK;
    bvr_mat_free(computed);
out:
    return t_res;
}

MunitResult blob_test_flood_fill(const MunitParameter params[], void *user_data_or_fixture)
{
    uint8_t src[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    bvr_mat8_t *img = bvr_mat8_new(5, 7);
    memcpy(img->content, &src[0], 7 * 5);

    bvr_blob_t *array = NULL;
    size_t len = 0;

    bvr_blobs_flood_fill(img, 1, 0, &array, &len);
    munit_assert_size(len, ==, 2);

    bvr_blob_t one = array[0];
    munit_assert_uint8(one.x_min, ==, 1);
    munit_assert_uint8(one.x_max, ==, 2);
    munit_assert_uint8(one.y_min, ==, 1);
    munit_assert_uint8(one.y_max, ==, 3);

    bvr_blob_t two = array[1];
    munit_assert_uint8(two.x_min, ==, 3);
    munit_assert_uint8(two.x_max, ==, 5);
    munit_assert_uint8(two.y_min, ==, 2);
    munit_assert_uint8(two.y_max, ==, 3);

    bvr_mat_free(img);

    return MUNIT_OK;
}

MunitResult blob_test_flood_projections(const MunitParameter params[], void *user_data_or_fixture)
{
    uint8_t src[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    bvr_mat8_t *img = bvr_mat8_new(5, 7);
    memcpy(img->content, &src[0], 7 * 5);

    bvr_blob_t *array = NULL;
    size_t len = 0;

    bvr_blobs_projections(img, &array, &len);
    munit_assert_size(len, ==, 1);

    bvr_mat_free(img);

    return MUNIT_OK;
}
MunitResult blob_test_queue(const MunitParameter params[], void *user_data_or_fixture)
{
    /// Simple push and length
    // Creation
    __bvr_blob_node_queue_t queue_store = _bvr_blob_new_queue();
    __bvr_blob_node_queue_t *queue = &queue_store;

    munit_assert_true(_bvr_blob_queue_is_empty(queue));
    // Push
    __bvr_blob_node_t node = (__bvr_blob_node_t){1, 2};
    _bvr_blob_queue_push(queue, node);
    munit_assert_false(_bvr_blob_queue_is_empty(queue));

    // Pop
    _bvr_blob_queue_pop(queue);
    munit_assert_true(_bvr_blob_queue_is_empty(queue));

    node = (__bvr_blob_node_t){1, 2};
    _bvr_blob_queue_push(queue, node);

    munit_assert_uint8(queue->head->x, ==, 1);
    munit_assert_uint8(queue->head->y, ==, 2);

    node = (__bvr_blob_node_t){10, 12};
    _bvr_blob_queue_push(queue, node);

    munit_assert_uint8(queue->head->x, ==, 10);
    munit_assert_uint8(queue->head->y, ==, 12);
    munit_assert_uint8(queue->tail->x, ==, 1);
    munit_assert_uint8(queue->tail->y, ==, 2);

    node = (__bvr_blob_node_t){25, 25};
    _bvr_blob_queue_push(queue, node);
    munit_assert_uint8(queue->head->x, ==, 25);
    munit_assert_uint8(queue->head->y, ==, 25);

    _bvr_blob_queue_pop(queue);
    munit_assert_uint8(queue->head->x, ==, 25);
    munit_assert_uint8(queue->head->y, ==, 25);
    munit_assert_uint8(queue->tail->x, ==, 10);
    munit_assert_uint8(queue->tail->y, ==, 12);

    _bvr_blob_queue_pop(queue);
    munit_assert_uint8(queue->head->x, ==, 25);
    munit_assert_uint8(queue->head->y, ==, 25);
    munit_assert_uint8(queue->tail->x, ==, 25);
    munit_assert_uint8(queue->tail->y, ==, 25);

    _bvr_blob_queue_pop(queue);
    munit_assert_true(_bvr_blob_queue_is_empty(queue));
    return MUNIT_OK;
}

MunitResult bvr_test_sauvola(const MunitParameter params[], void *user_data_or_fixture)
{
    const char *input_image_name = "samples/sauvola.png";

    bvr_io_image_source_t *src;
    int res;
    MunitResult t_res = MUNIT_FAIL;
    src = bvr_io_load_png(input_image_name);
    if (src == NULL)
        goto out;

    bvr_mat8_t *real = bvr_io_rgb_src(src);
    if (real == NULL)
        goto free_source;

    bvr_mat8_t *gray = bvr_filter_create_grayscale(real, 3);
    if (gray == NULL)
        goto free_source;

    bvr_mat8_t *filtered = bvr_filter_sauvola(gray, 0.5, 70, 0, 255);
    res = bvr_io_image_grayscale_write(filtered, "outputs/sauvola_applied.png");
    if (res < 0)
        goto free_image;

    t_res = MUNIT_OK;

free_image:
    bvr_mat_free(gray);
    bvr_mat_free(real);
    bvr_mat_free(filtered);
free_source:
    bvr_io_image_source_free(src);
out:
    return t_res;
}

MunitResult bvr_mat_real_mul_test(const MunitParameter params[], void *user_data_or_fixture)
{
    bvr_mat_real_t *lhs = bvr_mat_real_new(3, 3);
    double lhs_content[] = {0.9, 0.3, 0.4, 0.2, 0.8, 0.2, 0.1, 0.5, 0.6};
    memcpy(lhs->content, &lhs_content, 9 * 8);

    bvr_mat_real_t *rhs = bvr_mat_real_new(3, 1);
    double rhs_content[] = {0.9, 0.1, 0.8};
    memcpy(rhs->content, &rhs_content, 3 * 8);

    bvr_mat_real_t *result = bvr_mat_real_new(3, 1);
    bvr_mat_real_mul(lhs, rhs, result);

    double expected[] = {1.16, 0.42, 0.62};
    unsigned int i;
    for (i = 0; i < 3; i++)
    {
        munit_assert_double_equal(expected[i], bvr_mat_get(result, i, 0), 10);
    }
    bvr_mat_free(result);
    bvr_mat_free(lhs);
    bvr_mat_free(rhs);

    return MUNIT_OK;
}

double ten_times(double val) { return val * 10; };
MunitResult bvr_test_matrix_scalar_mul(const MunitParameter params[], void *user_data_or_fixture)
{
    bvr_mat_real_t *mat = bvr_mat_real_new(3, 1);
    double mat_content[] = {0.9, 0.1, 0.8};
    memcpy(mat->content, &mat_content, 3 * 8);

    bvr_mat_real_t *result = bvr_mat_real_new(3, 1);
    bvr_mat_real_t *result_func = bvr_mat_real_new(3, 1);
    double expected[] = {9.0, 1.0, 8.0};
    bvr_mat_scalar_mul(mat, 10.0, result);
    bvr_mat_apply_scalar_func(mat, ten_times, result_func);

    unsigned int i;
    for (i = 0; i < 3; i++)
    {
        munit_assert_double_equal(expected[i], bvr_mat_get(result, i, 0), 10);
        munit_assert_double_equal(expected[i], bvr_mat_get(result_func, i, 0), 10);
    }

    bvr_mat_free(result);
    bvr_mat_free(mat);

    return MUNIT_OK;
}
MunitResult bvr_test_simple1(const MunitParameter params[], void *user_data_or_fixture)
{
    // Load the inage
    const char *input_image_name = "samples/small_bvr.jpg";
    bvr_io_image_source_t *src;
    MunitResult t_res = MUNIT_FAIL;
    src = bvr_io_load_png(input_image_name);
    if (src == NULL)
    {
        goto out;
    }
    bvr_mat8_t *real = bvr_io_rgb_src(src);
    if (real == NULL)
        goto free_source;

    // Filter in gray
    bvr_mat8_t *gray = bvr_filter_create_grayscale(real, 3);
    if (gray == NULL)
        goto free_source;

    // Filter Sauvola
    bvr_mat8_t *filtered = bvr_filter_sauvola(gray, 0.5, 70, 255, 0);

    // Detect Blobs
    bvr_blob_t *blobs;
    size_t len;
    bvr_blobs_projections(filtered, &blobs, &len);
    munit_assert_size(len, ==, 42);

    // // Initialize network
    bvr_neural_net_t *net = bvr_neural_net_load("samples/neural1.genann");

    int i;
    char pathname[30];
    for (i = 0; i < len; i++)
    {
        // Extract blob
        bvr_mat8_t *blob_mat = bvr_extract_blob(filtered, &blobs[i]);
        sprintf(pathname, "outputs/%i.png", i);
        bvr_io_image_grayscale_write(blob_mat, pathname);

        // Resize to correct size
        bvr_mat8_t *blob_resize = bvr_resize(blob_mat, 32, 32);
        sprintf(pathname, "outputs/%i_sized.png", i);
        bvr_io_image_grayscale_write(blob_resize, pathname);

        bvr_mat_real_t *in = bvr_mat_real_from_mat8(blob_resize);
        bvr_mat_real_t reshaped;
        reshaped.columns = 1;
        reshaped.rows = in->columns * in->rows;
        reshaped.content = in->content;

        const char ch = bvr_neural_net_detect_blob(net, &reshaped);
        printf("Char %i: %c\n", i, ch);
    }

    t_res = MUNIT_OK;
    bvr_mat_free(gray);
    bvr_mat_free(filtered);
    bvr_mat_free(real);
    free(blobs);
    bvr_neural_net_free(net);
free_source:
    bvr_io_image_source_free(src);
out:
    return MUNIT_OK;
}

MunitTest tests[] = {
    {
        "/matrix",              /* name */
        matrix_init_free,       /* test */
        NULL,                   /* setup */
        NULL,                   /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL                    /* parameters */
    },
    {
        "/filter_convert_gray", /* name */
        filter_convert_gray,    /* test */
        NULL,                   /* setup */
        NULL,                   /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL                    /* parameters */
    },
    {
        "/filter_integral_image", /* name */
        filter_image_integral,    /* test */
        NULL,                     /* setup */
        NULL,                     /* tear_down */
        MUNIT_TEST_OPTION_NONE,   /* options */
        NULL                      /* parameters */
    },
    {
        "/filter_integral_image_2", /* name */
        filter_image_integral_2,    /* test */
        NULL,                       /* setup */
        NULL,                       /* tear_down */
        MUNIT_TEST_OPTION_NONE,     /* options */
        NULL                        /* parameters */
    },
    {
        "/filter_integral_image_3", /* name */
        filter_image_integral_3,    /* test */
        NULL,                       /* setup */
        NULL,                       /* tear_down */
        MUNIT_TEST_OPTION_NONE,     /* options */
        NULL                        /* parameters */
    },
    {
        "/blob_test_queue",     /* name */
        blob_test_queue,        /* test */
        NULL,                   /* setup */
        NULL,                   /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL                    /* parameters */
    },
    {
        "/blob_test_flood_fill", /* name */
        blob_test_flood_fill,    /* test */
        NULL,                    /* setup */
        NULL,                    /* tear_down */
        MUNIT_TEST_OPTION_NONE,  /* options */
        NULL                     /* parameters */
    },
    {
        "/blob_test_flood_projections", /* name */
        blob_test_flood_projections,    /* test */
        NULL,                           /* setup */
        NULL,                           /* tear_down */
        MUNIT_TEST_OPTION_NONE,         /* options */
        NULL                            /* parameters */
    },
    {
        "/bvr_test_sauvola",    /* name */
        bvr_test_sauvola,       /* test */
        NULL,                   /* setup */
        NULL,                   /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL                    /* parameters */
    },
    {
        "/bvr_test_simple1",    /* name */
        bvr_test_simple1,       /* test */
        NULL,                   /* setup */
        NULL,                   /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL                    /* parameters */
    },
    {
        "/bvr_mat_real_mul_test", /* name */
        bvr_mat_real_mul_test,    /* test */
        NULL,                     /* setup */
        NULL,                     /* tear_down */
        MUNIT_TEST_OPTION_NONE,   /* options */
        NULL                      /* parameters */
    },
    {
        "/bvr_test_matrix_scalar_mul", /* name */
        bvr_test_matrix_scalar_mul,    /* test */
        NULL,                          /* setup */
        NULL,                          /* tear_down */
        MUNIT_TEST_OPTION_NONE,        /* options */
        NULL                           /* parameters */
    },

    /* Mark the end of the array with an entry where the test
   * function is NULL */
    {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

static const MunitSuite suite = {
    "/core",                /* name */
    tests,                  /* tests */
    NULL,                   /* suites */
    1,                      /* iterations */
    MUNIT_SUITE_OPTION_NONE /* options */
};

int main(int argc, char **argv)
{
    return munit_suite_main(&suite, NULL, argc, argv);
}
