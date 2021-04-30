## Nice OpenSSL intro 
!!! Thanks to: https://www.roxlu.com/2014/042/using-openssl-with-memory-bios


## How TLS works
The Transport Layer Security protocol is used to secure your data that you e.g. want to receive and send to a client over a network. \
For example the Twitter API is only accessible over HTTPS. \
This means that the HTTP protocol is encrypted using TLS. TLS is based on asymmetric and symetric keys.

asymetric keys use a public and private key to encrypt and decrypt data. \
When data is encrypted using a private key only users with the public key can decrypt the data. When data is encrypted with the public key, only users with the private key can decrypt the data. \
Encrypting or decrypting data with asymetric keys takes more time then symetric keys. (RSA)

when using symmetric keys one can encrypt and decrypt data with the same key. \
symmetric keys are often used to encrypt large amount of data because it's easier to compute then asymetric keys. (DES, 3DES, AES, RC4, RC2)

TLS uses asymetric keys to exchange a symmetric key. \
Once the symmetric key is available between sender and receiver they are used to encrypt/decrypt the large partion of application data.

### TLS FLOW
TLS is a protocol which means that the flow of data is predefined. \
In most cases the data flows as shown in the snippet below. \
Important to know is the handshake, which kicks off the communication between a sender and receiver (e.g. browser and webserver). \
The CertificateRequest is an important aspect of TLS. \
A Certificate is used to check if the keys are created by an authority which is trusted. \
Also a certificate contains the public key. \
Also note that the handshake hops 4 times between client and server.

```
Client                                             Server
------                                             ------
 
                           (1)
ClientHello             -------->                                 |
                                              ServerHello         |
                                             Certificate*         |
                                       ServerKeyExchange*         |
                                      CertificateRequest*         |
                           (2)                                    |
                        <--------         ServerHelloDone         |
Certificate*                                                      |
ClientKeyExchange                                                 |--- HANDSHAKE
CertificateVerify*                                                |
[ChangeCipherSpec]                                                |
                           (3)                                    |
Finished                -------->                                 |
                                       [ChangeCipherSpec]         |
                           (4)                                    |
                        <--------                Finished         |
 
Application Data        <------->        Application Data

```

### BIOs
This part will describe how I've used openSSL in a experimental application to secure data. \
In the code which will follow below I'm similating a client and server in one application. \
The client starts the handshake by sending a ClientHello to the server.

OpenSSL uses the a concept of BIOs which is an input/output abstraction allowing us to e.g. use data from a file, memory or network in a similar way. \
Because I want to manage my own memory I'm using a memory bio. \
These BIOs always tend to confuse me because of the *_BIO_read()_*, *_BIO_write()_*, *_SSL_read()_*, *_SSL_write()_* functions and the BIO objects you use with them. \
To make it a bit easier and less confusing with all these read(), write() functions I name my BIOs not "read" and "write" BIO, but input bio and output bio. \
Somehow this makes more sense to me. For example, I use an input BIO to store data that comes from the network and then I use SSL_read() to retrieve the unencrypted data.

- SSL_read() read unencrypted data which is stored in the input BIO.
- SSL_write() write unencrypted data into the output BIO.
- BIO_write() write encrypted data into the input BIO.
- BIO_read() read encrypted data from the output BIO.

So when do you use what function?

- Use BIO_write() to store encrypted data you receive from e.g. a tcp/udp socket. \
Once you've written to an input BIO, you use SSL_read() to get the unencrypted data, but only after the handshake is ready.
- Use BIO_read() to check if there is any data in the output BIO. \
The output BIO will be filled by openSSL when it's handling the handshake or when you call SSL_write(). \
When there is data in your output BIO, use BIO_read to get the data and send it to e.g. a client. \
Use BIO_ctrl_pending() to check how many bytes there are stored in the output bio. \
See *_krx_ssl_handle_traffic()_* in the code listing at the bottom of this post.

### Using OpenSSL
Before you can use anything of the OpenSSL library you need to initialize it:

```cpp
SSL_library_init();
SSL_load_error_strings();
ERR_load_BIO_strings();
OpenSSL_add_all_algorithms();
```

When you're ready you can shutdown the library by using:

```cpp
ERR_remove_state(0);
ENGINE_cleanup();
CONF_modules_unload(1);
ERR_free_strings();
EVP_cleanup();
sk_SSL_COMP_free(SSL_COMP_get_compression_methods());
CRYPTO_cleanup_all_ex_data();
```

To use openSSL you create a SSL_CTX object which is a context that keeps track of shared information like the certificate to use, private key to use etc.. See the krx_ssl_ctx_init() function in the souce code at the bottom of this document.

Once you have a SSL_CTX object you can create a SSL object for a particulair connection. You can create many SSL objects for each connection you want to handle and share one SSL_CTX object.

When you're using openSSL as a server you need to make sure that the SSL object knows that it should act as a server which can be done by calling SSL_set_accept_state(ssl). When SSL is used as a server it will wait for a ClientHello message and makes sure that a ServerHello, CertificateRequest etc.. is send back to the client. When you want your SSL object to act like a client you need to call SSL_set_connect_state(ssl) and call start filling your output BIO by calling SSL_do_handshake().

As long as the handshake is not yet ready, you have to make sure that the function SSL_do_handshake() is called everytime the input or output BIOs change. For example, when the server starts, and a clients connects, you call BIO_write() and fill the input BIO with data you got on your socket, then call SSL_do_handshake(ssl) as long as the handshake isn't ready yet. You can check this by using SSL_is_init_finished(ssl). After the handshake has been completed you can start filling the BIOs by calling SSL_read and SSL_write with your application data that you want to secure.

#### Update: tracking state
Some people have told me you don't need to track state of SSL. Though in some situations it's good to know when the handshake (initialisation) has finished. OpenSSL provides a function SSL_is_init_finished() to check this. As long as SSL_is_init_finished() returns false you should handle the handshake, which is a 4 step procedure as described above (see the flow example). When SSL_is_init_finished() returns false you need to call SSL_do_handshake(). OpenSSL keeps state internally and calling SSL_do_handshake() should be enough to allow openSSL to perform the handshake. *But*, it's important that you check the return value of the call to SSL_do_handshake() because it tells you want functions you need to call next, especially when using memory bios. As described in the manual: _"if the underlying BIO is non-blocking, SSL_do_handshake() will also return when the underlying BIO could not satisfy the needs of SSL_do_handshake() to continue the handshake. In this case a call to SSL_get_error() with the return value of SSL_do_handshake() will yield SSL_ERROR_WANT_READ or SSL_ERROR_WANT_WRITE."_ So A very verbose way of handling this could be something like:

```cpp

if (!SSL_is_init_finished(ssl)) {
 
   /* NOT INITIALISED */
 
   r = SSL_do_handshake(ssl);
   if (r < 0) {
 
     r = SSL_get_error(ssl, r);
     if (SSL_ERROR_WANT_READ == r) {
        pending = BIO_ctrl_pending(out_bio);
        if (pending > 0) {
          read = BIO_read(out_bio, buffer, POLY_SSL_BUFFER_SIZE);
          if (read > 0) {
            listener->onSslBufferEncryptedData(buffer, read);
          }
        }
      }
    }
 }
else {
 
    /* SSL IS INITIALISED */
 
    pending = BIO_ctrl_pending(out_bio);
    if (pending > 0) {
      read = BIO_read(out_bio, buffer, POLY_SSL_BUFFER_SIZE);
      if (read > 0) {
        listener->onSslBufferEncryptedData(buffer, read);
      }
    }
 
    pending = BIO_ctrl_pending(in_bio);
    if (pending > 0) {
      while((read = SSL_read(ssl, buffer, POLY_SSL_BUFFER_SIZE)) > 0) {
        listener->onSslBufferDecryptedData(buffer, read);
      }    
    }
  }
}
```
If you implement your own socket functions you need to make sure that the SSL handshake has been finished before your start enrypting your application data. A function to check if you need to read more data from your socket is:

```cpp
int SslBuffer::wantsMoreData() {
 
  int r = 0;
 
  if (SSL_is_init_finished(ssl)) {
    return -1;
  }
 
  r = SSL_do_handshake(ssl);
  if (r < 0) {
    r = SSL_get_error(ssl, r);
    if (SSL_ERROR_WANT_READ == r) {
      return 0;
    }
  }
 
  return -1;
}
```

Example code
The following code listing shows how you can use OpenSSL with Memory BIOs.
```cpp
/* 
 
    Create server/client self-signed certificate/key (self signed, DONT ADD PASSWORD) 
 
    openssl req -x509 -newkey rsa:2048 -days 3650 -nodes -keyout client-key.pem -out client-cert.pem
    openssl req -x509 -newkey rsa:2048 -days 3650 -nodes -keyout server-key.pem -out server-cert.pem
 
*/
 
#include <stdio.h>
#include <stdlib.h>
 
#include <openssl/err.h>
#include <openssl/dh.h>
#include <openssl/ssl.h>
#include <openssl/conf.h>
#include <openssl/engine.h>
 
/* SSL debug */
#define SSL_WHERE_INFO(ssl, w, flag, msg) {                \
    if(w & flag) {                                         \
      printf("+ %s: ", name);                              \
      printf("%20.20s", msg);                              \
      printf(" - %30.30s ", SSL_state_string_long(ssl));   \
      printf(" - %5.10s ", SSL_state_string(ssl));         \
      printf("\n");                                        \
    }                                                      \
  } 
 
typedef void(*info_callback)();
 
typedef struct {
  SSL_CTX* ctx;                                                                       /* main ssl context */
  SSL* ssl;                                                                           /* the SSL* which represents a "connection" */
  BIO* in_bio;                                                                        /* we use memory read bios */
  BIO* out_bio;                                                                       /* we use memory write bios */
  char name[512];
} krx;
 
void krx_begin();                                                                     /* initialize SSL */
void krx_end();                                                                       /* shutdown SSL */
int krx_ssl_ctx_init(krx* k, const char* keyname);                                    /* initialize the SSL_CTX */
int krx_ssl_init(krx* k, int isserver, info_callback cb);                             /* init the SSL* (the "connection"). we use the `isserver` to tell SSL that it should either use the server or client protocol */
int krx_ssl_shutdown(krx* k);                                                         /* cleanup SSL allocated mem */
int krx_ssl_verify_peer(int ok, X509_STORE_CTX* ctx);                                 /* we set the SSL_VERIFY_PEER option on the SSL_CTX, so that the server will request the client certificate. We can use the certificate to get/verify the fingerprint */
int krx_ssl_handle_traffic(krx* from, krx* to);
 
/* some debug info */
void krx_ssl_server_info_callback(const SSL* ssl, int where, int ret);                /* purely for debug purposes; logs server info. */
void krx_ssl_client_info_callback(const SSL* ssl, int where, int ret);                /* client info callback */
void krx_ssl_info_callback(const SSL* ssl, int where, int ret, const char* name);     /* generic info callback */
 
int main() {
 
  /* startup SSL */
  krx_begin();
 
  /* create client/server objects */
  krx server;
  krx client;
 
  /* init server. */
  if(krx_ssl_ctx_init(&server, "server") < 0) {
    exit(EXIT_FAILURE);
  }
  if(krx_ssl_init(&server, 1, krx_ssl_server_info_callback) < 0) {
    exit(EXIT_FAILURE);
  }
 
  printf("+ Initialized server.\n");
 
  /* init client. */
  if(krx_ssl_ctx_init(&client, "client") < 0) {
    exit(EXIT_FAILURE);
  }
  if(krx_ssl_init(&client, 0, krx_ssl_client_info_callback) < 0) {
    exit(EXIT_FAILURE);
  }
 
  printf("+ Initialized client.\n");
 
  /* kickoff handshake; initiated by client (e.g. browser) */
  SSL_do_handshake(client.ssl);
  krx_ssl_handle_traffic(&client, &server); 
  krx_ssl_handle_traffic(&server, &client); 
  krx_ssl_handle_traffic(&client, &server); 
  krx_ssl_handle_traffic(&server, &client); 
 
  /* encrypt some data and send it to the client */
  char buf[521] = { 0 } ;
  sprintf(buf, "%s", "Hello world");
  SSL_write(server.ssl, buf, sizeof(buf));
  krx_ssl_handle_traffic(&server, &client);
 
  krx_ssl_shutdown(&server);
  krx_ssl_shutdown(&client);
 
  krx_end();
  return EXIT_SUCCESS;
}
 
void krx_begin() {
  SSL_library_init();
  SSL_load_error_strings();
  ERR_load_BIO_strings();
  OpenSSL_add_all_algorithms();
}
 
void krx_end() {
  ERR_remove_state(0);
  ENGINE_cleanup();
  CONF_modules_unload(1);
  ERR_free_strings();
  EVP_cleanup();
  sk_SSL_COMP_free(SSL_COMP_get_compression_methods());
  CRYPTO_cleanup_all_ex_data();
}
 
int krx_ssl_ctx_init(krx* k, const char* keyname) {
 
  int r = 0;
 
  /* create a new context using DTLS */
  k->ctx = SSL_CTX_new(DTLSv1_method());
  if(!k->ctx) {
    printf("Error: cannot create SSL_CTX.\n");
    ERR_print_errors_fp(stderr);
    return -1;
  }
 
  /* set our supported ciphers */
  r = SSL_CTX_set_cipher_list(k->ctx, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
  if(r != 1) {
    printf("Error: cannot set the cipher list.\n");
    ERR_print_errors_fp(stderr);
    return -2;
  }
 
  /* the client doesn't have to send it's certificate */
  SSL_CTX_set_verify(k->ctx, SSL_VERIFY_PEER, krx_ssl_verify_peer);
 
  /* enable srtp */
  r = SSL_CTX_set_tlsext_use_srtp(k->ctx, "SRTP_AES128_CM_SHA1_80");
  if(r != 0) {
    printf("Error: cannot setup srtp.\n");
    ERR_print_errors_fp(stderr);
    return -3;
  }
 
  /* load key and certificate */
  char certfile[1024];
  char keyfile[1024];
  sprintf(certfile, "./%s-cert.pem", keyname);
  sprintf(keyfile, "./%s-key.pem", keyname);
 
  /* certificate file; contains also the public key */
  r = SSL_CTX_use_certificate_file(k->ctx, certfile, SSL_FILETYPE_PEM);
  if(r != 1) {
    printf("Error: cannot load certificate file.\n");
    ERR_print_errors_fp(stderr);
    return -4;
  }
 
  /* load private key */
  r = SSL_CTX_use_PrivateKey_file(k->ctx, keyfile, SSL_FILETYPE_PEM);
  if(r != 1) {
    printf("Error: cannot load private key file.\n");
    ERR_print_errors_fp(stderr);
    return -5;
  }
 
  /* check if the private key is valid */
  r = SSL_CTX_check_private_key(k->ctx);
  if(r != 1) {
    printf("Error: checking the private key failed. \n");
    ERR_print_errors_fp(stderr);
    return -6;
  }
 
  sprintf(k->name, "+ %s", keyname);
 
  return 0;
}
 
int krx_ssl_verify_peer(int ok, X509_STORE_CTX* ctx) {
  return 1;
}
 
/* this sets up the SSL* */
int krx_ssl_init(krx* k, int isserver, info_callback cb) {
 
  /* create SSL* */
  k->ssl = SSL_new(k->ctx);
  if(!k->ssl) {
    printf("Error: cannot create new SSL*.\n");
    return -1;
  }
 
  /* info callback */
  SSL_set_info_callback(k->ssl, cb);
 
  /* bios */
  k->in_bio = BIO_new(BIO_s_mem());
  if(k->in_bio == NULL) {
    printf("Error: cannot allocate read bio.\n");
    return -2;
  }
 
  BIO_set_mem_eof_return(k->in_bio, -1); /* see: https://www.openssl.org/docs/crypto/BIO_s_mem.html */
 
  k->out_bio = BIO_new(BIO_s_mem());
  if(k->out_bio == NULL) {
    printf("Error: cannot allocate write bio.\n");
    return -3;
  }
 
  BIO_set_mem_eof_return(k->out_bio, -1); /* see: https://www.openssl.org/docs/crypto/BIO_s_mem.html */
 
  SSL_set_bio(k->ssl, k->in_bio, k->out_bio);
 
  /* either use the server or client part of the protocol */
  if(isserver == 1) {
    SSL_set_accept_state(k->ssl);
  }
  else {
    SSL_set_connect_state(k->ssl);
  }
 
  return 0;
}
 
void krx_ssl_server_info_callback(const SSL* ssl, int where, int ret) {
  krx_ssl_info_callback(ssl, where, ret, "server");
}
void krx_ssl_client_info_callback(const SSL* ssl, int where, int ret) {
  krx_ssl_info_callback(ssl, where, ret, "client");
}
 
void krx_ssl_info_callback(const SSL* ssl, int where, int ret, const char* name) {
 
  if(ret == 0) {
    printf("-- krx_ssl_info_callback: error occured.\n");
    return;
  }
 
  SSL_WHERE_INFO(ssl, where, SSL_CB_LOOP, "LOOP");
  SSL_WHERE_INFO(ssl, where, SSL_CB_HANDSHAKE_START, "HANDSHAKE START");
  SSL_WHERE_INFO(ssl, where, SSL_CB_HANDSHAKE_DONE, "HANDSHAKE DONE");
}
 
int krx_ssl_handle_traffic(krx* from, krx* to) {
 
  // Did SSL write something into the out buffer
  char outbuf[4096]; 
  int written = 0;
  int read = 0;
  int pending = BIO_ctrl_pending(from->out_bio);
 
  if(pending > 0) {
    read = BIO_read(from->out_bio, outbuf, sizeof(outbuf));
  }
  printf("%s Pending %d, and read: %d\n", from->name, pending, read);
 
  if(read > 0) {
    written = BIO_write(to->in_bio, outbuf, read);
  }
 
  if(written > 0) {
    if(!SSL_is_init_finished(to->ssl)) {
      SSL_do_handshake(to->ssl);
    }
    else {
      read = SSL_read(to->ssl, outbuf, sizeof(outbuf));
      printf("%s read: %s\n", to->name, outbuf);
    }
  }
 
  return 0;
}
 
int krx_ssl_shutdown(krx* k) {
  if(!k) {
    return -1;
  }
 
  if(k->ctx) { 
    SSL_CTX_free(k->ctx);
    k->ctx = NULL;
  }
 
  if(k->ssl) {
    SSL_free(k->ssl);
    k->ssl = NULL;
  }
 
  return 0;
}
```
