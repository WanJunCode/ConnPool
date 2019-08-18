#include <bson/bson.h>
#include <mongoc/mongoc.h>

int
main (int argc, char *argv[])
{
   mongoc_client_t *client;

   mongoc_init ();

   /* Create our MongoDB Client */
   client = mongoc_client_new (
      "mongodb://host01:27017,host02:27017,host03:27017/?replicaSet=myreplset");

   /* Do some work */
   /* TODO */

   /* Clean up */
   mongoc_client_destroy (client);
   mongoc_cleanup ();

   return 0;
}