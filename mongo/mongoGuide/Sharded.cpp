#include <bson/bson.h>
#include <mongoc/mongoc.h>

int
main (int argc, char *argv[])
{
   mongoc_client_t *client;

   mongoc_init ();

   /* Create our MongoDB Client */
   client = mongoc_client_new ("mongodb://myshard01:27017/");

    // 创建支持压缩的连接
    // mongoc_client_t *client = NULL;
    // client = mongoc_client_new ("mongodb://localhost:27017/?compressors=snappy,zlib,zstd");

   /* Do something with client ... */

   /* Free the client */
   mongoc_client_destroy (client);

   mongoc_cleanup ();

   return 0;
}