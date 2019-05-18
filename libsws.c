#include <curl/curl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#include <tls.h>

struct websocket
{
	CURL *curl;
	struct tls *tls;
	int is_tls;
	int sockfd;
};

//defined in RFC 6455
struct base_frame
{
	unsigned char byte_a;
	/* 
	 * 1 : fin frag
	 * 2-4: RSV
	 * 5-8: opcode
	 */
	unsigned char byte_b;

	unsigned char byte_c;
	/*
	 * uhhh can i die plz, this is hella low level
	 * i prolly need to use union for certain bytes that can be one thing or be another
	 * i still need to read into the RFC some more so this aint final
	 */
};

static curl_socket_t ws_create(void *sockfd, curlsocktype purpose, struct curl_sockaddr *addr)
{
	*sockfd = (int) socket(addr->family, addr->socktype, addr->protocol);
	return *sockfd;
}

struct websocket *ws_init(void)
{
	struct websocket *rv;

	rv = malloc(sizeof(websocket));

	rv->curl = curl_easy_init();
	rv->tls = tls_client();

	return rv;
}

void ws_cleanup(struct websocket *ws)
{
	curl_easy_cleanup(ws->curl);
	tls_free(ws->tls);
}

/*
 * use http/https for the host scheme rather than ws/wss
 * Example: https://gateway.discord.gg
 */
ws_connect(const char *host, const char key, unsigned char version)
{
	//prepares headers for websocket handshake
	curl_slist_append(NULL, "Upgrade: websocket");
	curl_slist_append(headers, "Connection: upgrade");
	curl_slist_append(headers, "Sec-Websocket-Key: ?");
	curl_slist_append(headers, "Sec-Websocket-Version: 13");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers)
}

void ws_close(struct websocket *ws)
{
	if(ws->is_ls == 1)
		tls_close(ws->tls);
	
	close(ws->sockfd);
}


ssize_t ws_read(struct websocket *ws, void *buf, size_t buflen)
{
	if(ws->is_tls == 1)
		return tls_read(ws->tls, buf, buflen);

	return read(ws->sockfd, buf, buflen);
}

ssize_t ws_write(struct websocket *ws, const void *buf, size_t buflen)
{
	if(ws->is_tls == 1)
		return tls_write(ws->tls, buf, buflen);

	return write(ws->sockfd, buf, buflen);
}

/*
 * the easy variation of ws read/write are similar to the standard ws read/write 
 * functions except that it automatically de/encapsulates data in packets following 
 * the base framing protocol defined in RFC 6455
 */

char *ws_easy_read(struct websocket *ws, void *buf, size_t buflen)
{
	//decapsulate and return a pointer to the application data
}

ssize_t ws_easy_write(struct websocket *ws, const void *buff, buflen)
{
	//encapsulates application data accoridly and sends it
}
