#include <bson/bson.h>
#include <mongoc/mongoc.h>
#include <stdio.h>

int
main (int argc, char *argv[])
{
   mongoc_client_t *client;
   mongoc_collection_t *collection;
   bson_error_t error;
   bson_t *doc;
   int64_t count;

   mongoc_init ();

   client =
      mongoc_client_new ("mongodb://localhost:27017/?appname=count-example");
   collection = mongoc_client_get_collection (client, "mydb", "mycoll");
   doc = bson_new_from_json (
      (const uint8_t *) "{\"hello\" : \"world\"}", -1, &error);

    // 查询 hello:world 的文档
   count = mongoc_collection_count (
      collection, MONGOC_QUERY_NONE, doc, 0, 0, NULL, &error);

   if (count < 0) {
      fprintf (stderr, "%s\n", error.message);
   } else {
      printf ("%" PRId64 "\n", count);
   }

   bson_destroy (doc);
   mongoc_collection_destroy (collection);
   mongoc_client_destroy (client);
   mongoc_cleanup ();

   return 0;
}