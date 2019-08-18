#include <bson/bson.h>
#include <mongoc/mongoc.h>

static void
print_all_documents (mongoc_collection_t *collection)
{
    mongoc_cursor_t *cursor;
    const bson_t *doc;
    bson_error_t error;
    bson_t query = BSON_INITIALIZER;
    char *str;

    cursor = mongoc_collection_find_with_opts (collection, query, NULL, NULL);

    // 获得 cursor 指向的 document
    while (mongoc_cursor_next (cursor, &doc)) {
        str = bson_as_canonical_extended_json (doc, NULL);
        printf ("%s\n", str);
        bson_free (str);
    }

    // 检查 cursor 是否有错误发生
    if (mongoc_cursor_error (cursor, &error)) {
        fprintf (stderr, "Failed to iterate all documents: %s\n", error.message);
    }

    mongoc_cursor_destroy (cursor);
}