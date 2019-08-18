#include <mongoc/mongoc.h>
#include <pthread.h>
#include <stdio.h>

const char *COLLECTION_NAME = "things";

/* Don't try to compile this file on its own. It's meant to be #included
   by example code */

/* Insert some sample data */
bool
insert_data (mongoc_collection_t *collection)
{
   mongoc_bulk_operation_t *bulk;
   enum N { ndocs = 4 };
   bson_t *docs[ndocs];
   bson_error_t error;
   int i = 0;
   bool ret;

   // 大量的操作
   bulk = mongoc_collection_create_bulk_operation_with_opts (collection, NULL);

   docs[0] = BCON_NEW ("x", BCON_DOUBLE (1.0), "tags", "[", "dog", "cat", "]");
   docs[1] = BCON_NEW ("x", BCON_DOUBLE (2.0), "tags", "[", "cat", "]");
   docs[2] = BCON_NEW (
      "x", BCON_DOUBLE (2.0), "tags", "[", "mouse", "cat", "dog", "]");
   docs[3] = BCON_NEW ("x", BCON_DOUBLE (3.0), "tags", "[", "]");

   // 添加到 bulk 中
   for (i = 0; i < ndocs; i++) {
      mongoc_bulk_operation_insert (bulk, docs[i]);
      bson_destroy (docs[i]);
      docs[i] = NULL;
   }

   ret = mongoc_bulk_operation_execute (bulk, NULL, &error);

   if (!ret) {
      fprintf (stderr, "Error inserting data: %s\n", error.message);
   }

   mongoc_bulk_operation_destroy (bulk);
   return ret;
}

/* A helper which we'll use a lot later on */
void
print_res (const bson_t *reply)
{
   char *str;
   BSON_ASSERT (reply);
   str = bson_as_canonical_extended_json (reply, NULL);
   printf ("%s\n", str);
   bson_free (str);
}

bool
explain (mongoc_collection_t *collection)
{
   bson_t *command;
   bson_t reply;
   bson_error_t error;
   bool res;

   command = BCON_NEW ("explain",
                       "{",
                       "find",
                       BCON_UTF8 (COLLECTION_NAME),
                       "filter",
                       "{",
                       "x",
                       BCON_INT32 (1),
                       "}",
                       "}");
   res = mongoc_collection_command_simple (collection, command, NULL, &reply, &error);
   if (!res) {
      fprintf (stderr, "Error with explain: %s\n", error.message);
      goto cleanup;
   }

   /* Do something with the reply */
   print_res (&reply);

cleanup:
   bson_destroy (&reply);
   bson_destroy (command);
   return res;
}


int
main (int argc, char *argv[])
{
   mongoc_database_t *database = NULL;
   mongoc_client_t *client = NULL;
   mongoc_collection_t *collection = NULL;
   mongoc_uri_t *uri = NULL;
   bson_error_t error;
   char *host_and_port;
   int res = 0;

   if (argc < 2 || argc > 3) {
      fprintf (stderr,
               "usage: %s MONGOD-1-CONNECTION-STRING "
               "[MONGOD-2-HOST-NAME:MONGOD-2-PORT]\n",
               argv[0]);
      fprintf (stderr,
               "MONGOD-1-CONNECTION-STRING can be "
               "of the following forms:\n");
      fprintf (stderr, "localhost\t\t\t\tlocal machine\n");
      fprintf (stderr, "localhost:27018\t\t\t\tlocal machine on port 27018\n");
      fprintf (stderr,
               "mongodb://user:pass@localhost:27017\t"
               "local machine on port 27017, and authenticate with username "
               "user and password pass\n");
      return EXIT_FAILURE;
   }

   mongoc_init ();

   if (strncmp (argv[1], "mongodb://", 10) == 0) {
      host_and_port = bson_strdup (argv[1]);
   } else {
      host_and_port = bson_strdup_printf ("mongodb://%s", argv[1]);
   }

   uri = mongoc_uri_new_with_error (host_and_port, &error);
   if (!uri) {
      fprintf (stderr,
               "failed to parse URI: %s\n"
               "error message:       %s\n",
               host_and_port,
               error.message);
      res = EXIT_FAILURE;
      goto cleanup;
   }

   client = mongoc_client_new_from_uri (uri);
   if (!client) {
      res = EXIT_FAILURE;
      goto cleanup;
   }

   mongoc_client_set_error_api (client, 2);
   database = mongoc_client_get_database (client, "test");
   collection = mongoc_database_get_collection (database, COLLECTION_NAME);

   printf ("Inserting data\n");
   if (!insert_data (collection)) {
      res = EXIT_FAILURE;
      goto cleanup;
   }

   printf ("explain\n");
   if (!explain (collection)) {
      res = EXIT_FAILURE;
      goto cleanup;
   }

cleanup:
   if (collection) {
      mongoc_collection_destroy (collection);
   }

   if (database) {
      mongoc_database_destroy (database);
   }

   if (client) {
      mongoc_client_destroy (client);
   }

   if (uri) {
      mongoc_uri_destroy (uri);
   }

   bson_free (host_and_port);
   mongoc_cleanup ();
   return res;
}