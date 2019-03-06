#include "munit.h"

#include <bvrcore.h>
#include <bvrcore_private.h>
/* A test runs various assertions, then calls PASS(), FAIL(), or SKIP(). */
MunitResult matrix_init_free(const MunitParameter params[], void *user_data_or_fixture)
{
    bvr_mat8_t *mat = bvr_mat8_new(10, 1);

    // Should initialize with 0
    munit_assert_uint8(bvr_mat_get(mat, 0, 0), ==, 0);
    munit_assert_uint8(bvr_mat_get(mat, 1, 0), ==, 0);

    bvr_mat_set(mat, 0, 0, 255);
    munit_assert_uint8(bvr_mat_get(mat, 0, 0), ==, 255);

    bvr_mat_free(mat);
    return MUNIT_OK;
}

MunitResult filter_convert_gray(const MunitParameter params[], void *user_data_or_fixture)
{
    const char *input_image_name = "samples/small_bvr.jpg";

    bvr_io_image_source_t src;
    int res;
    MunitResult t_res = MUNIT_FAIL;
    res = bvr_io_load_image(input_image_name, &src);
    if (res < 0)
        goto out;

    bvr_mat8_t *gray = bvr_filter_create_grayscale(&src);
    if (gray == NULL)
        goto free_source;

    res = bvr_io_image_grayscale_write(gray, "outputs/small_bvr_gray.jpg", BVRWritingTypeJPG);
    if (res < 0)
        goto free_image;

    t_res = MUNIT_OK;

    goto out;
free_image:
    bvr_mat_free(gray);
free_source:
    bvr_io_image_source_free(src);
out:
    return res;
}

MunitResult filter_image_integral(const MunitParameter params[], void *user_data_or_fixture)
{
    uint8_t src[] = {5, 2, 3, 6};
    bvr_mat8_t img = (bvr_mat8_t){2, 2, &src[0]};

    double res[] = {5, 7, 8, 16};

    bvr_matf32_t *computed = bvr_filter_create_image_integral(&img);
    MunitResult t_res = MUNIT_FAIL;
    if (computed == NULL)
        goto out;

    munit_assert_memory_equal(4, computed->content, &res[0]);
    t_res = MUNIT_OK;
out:
    bvr_mat_free(computed);
    return t_res;
}

MunitResult filter_image_integral_2(const MunitParameter params[], void *user_data_or_fixture)
{

    uint8_t src[] = {5, 2, 5, 2, 3, 6, 3, 6, 5, 2, 5, 2, 3, 6, 3, 6};
    bvr_mat8_t img = (bvr_mat8_t){4, 4, &src[0]};

    double res[] = {5, 7, 12, 14, 8, 16, 24, 32, 13, 23, 36, 46, 16, 32, 48, 64};

    bvr_matf32_t *computed = bvr_filter_create_image_integral(&img);
    MunitResult t_res = MUNIT_FAIL;
    if (computed == NULL)
        goto out;

    munit_assert_memory_equal(16, computed->content, &res[0]);
    t_res = MUNIT_OK;
out:
    bvr_mat_free(computed);
    return t_res;
}

MunitResult filter_image_integral_3(const MunitParameter params[], void *user_data_or_fixture)
{

    uint8_t src[] = {31, 2, 4, 33, 5, 36, 12, 26, 9, 10, 29, 25, 13, 17, 21, 22, 20, 18, 24, 23, 15, 16, 14, 19, 30, 8, 28, 27, 11, 7, 1, 35, 34, 3, 32, 6};
    bvr_mat8_t img = (bvr_mat8_t){6, 6, &src[0]};

    double res[] = {31, 33, 37, 70, 75, 111, 43, 71, 84, 127, 161, 222, 56, 101, 135, 200, 254, 333, 80, 148, 197, 278, 346, 444, 110, 186, 263, 371, 450, 555, 111, 222, 333, 444, 555, 666};

    bvr_matf32_t *computed = bvr_filter_create_image_integral(&img);
    MunitResult t_res = MUNIT_FAIL;
    if (computed == NULL)
        goto out;

    munit_assert_memory_equal(36, computed->content, &res[0]);
    t_res = MUNIT_OK;
out:
    bvr_mat_free(computed);
    return t_res;
}

MunitResult io_load_jpg(const MunitParameter params[], void *user_data_or_fixture)
{
    const char *input_image_name = "samples/white.jpg";

    bvr_io_image_source_t src;
    int result = bvr_io_load_image(input_image_name, &src);
    if (result < 0)
        return MUNIT_FAIL;

    munit_assert_size(40, ==, src.pixel_width);
    munit_assert_size(40, ==, src.pixel_height);
    size_t bytesPerRow = 40UL * 4UL;
    munit_assert_size(bytesPerRow * src.pixel_height, ==, src.data_len);

    size_t i;
    for (i = 0; i < src.data_len; i++)
    {
        // All the pixels of the test image are white
        munit_assert_uint8(src.data[i], ==, 0xFF);
    }

    bvr_io_image_source_free(src);
    return MUNIT_OK;
}

MunitResult blob_test_flood_fill(const MunitParameter params[], void *user_data_or_fixture)
{
    uint8_t src[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    bvr_mat8_t *img = bvr_mat8_new(7, 5);
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

    bvr_mat8_t *img = bvr_mat8_new(7, 5);
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
    const char *input_image_name = "samples/sauvola.jpg";

    bvr_io_image_source_t src;
    int res;
    MunitResult t_res = MUNIT_FAIL;
    res = bvr_io_load_image(input_image_name, &src);
    if (res < 0)
        goto out;

    bvr_mat8_t *gray = bvr_filter_create_grayscale(&src);
    if (gray == NULL)
        goto free_source;

    bvr_mat8_t *filtered = bvr_filter_sauvola(gray, 0.5, 70, 0, 255);
    res = bvr_io_image_grayscale_write(filtered, "outputs/sauvola_applied.jpg", BVRWritingTypeJPG);
    if (res < 0)
        goto free_image;

    t_res = MUNIT_OK;

free_image:
    bvr_mat_free(gray);
    bvr_mat_free(filtered);
free_source:
    bvr_io_image_source_free(src);
out:
    return res;
}

MunitResult bvr_test_simple1(const MunitParameter params[], void *user_data_or_fixture)
{
    const char *input_image_name = "samples/small_bvr.jpg";

    bvr_io_image_source_t src;
    int res;
    MunitResult t_res = MUNIT_FAIL;
    res = bvr_io_load_image(input_image_name, &src);
    if (res < 0)
        goto out;

    bvr_mat8_t *gray = bvr_filter_create_grayscale(&src);
    if (gray == NULL)
        goto free_source;

    bvr_mat8_t *filtered = bvr_filter_sauvola(gray, 0.5, 70, 1, 0);
    bvr_blob_t *blobs;
    size_t len;
    bvr_blobs_projections(filtered, &blobs, &len);
    munit_assert_size(len, ==, 42);
    t_res = MUNIT_OK;
free_image:
    bvr_mat_free(gray);
    bvr_mat_free(filtered);
    free(blobs);
free_source:
    bvr_io_image_source_free(src);
out:
    return res;
}
MunitTest tests[] = {
    {
        "/io_load_jpg",         /* name */
        io_load_jpg,            /* test */
        NULL,                   /* setup */
        NULL,                   /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL                    /* parameters */
    },
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
