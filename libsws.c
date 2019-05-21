#include <curl/curl.h>
#include <poll.h>
#include <unistd.h>

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

/*
 * use http/https for the host scheme rather than ws/wss
 * Example: https://gateway.discord.gg
 */
static int ws_handshake(CURL *curl, const char *host, const char *key, unsigned char version)
{
	//prepares headers for websocket handshake
	long resp;
	int rv;

	rv = 0;

	curl_slist_append(NULL, "Upgrade: websocket");
	curl_slist_append(headers, "Connection: upgrade");
	curl_slist_append(headers, "Sec-Websocket-Key: ?");
	curl_slist_append(headers, "Sec-Websocket-Version: 13");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_preform(curl);
	
	curl_easy_getinfo(curl, CURLINFO_HTTP_RESPONSECODE, &resp);
	
	if(resp != 101)
		goto err;

	rv = 1;

err:
	return rv;
}

int ws_connect(CURL *curl, const char *host, const char *key, unsigned char version)
{
	int rv;

	rv = 0;

	if(ws_handshake(curl, host, key, version) != 1)
		goto err;

	curl_easy_pause(curl, CURLPAUSE_ALL);
	curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET &rv);

err:
	return rv;
}

CURL *ws_init(void)
{
	return curl_easy_init();
}

void ws_cleanup(CURL *curl)
{
	curl_easy_cleanup(curl);
}



void ws_close(CURL *curl)
{
}


ssize_t ws_read(struct websocket *ws, void *buf, size_t buflen)
{
}

ssize_t ws_write(struct websocket *ws, const void *buf, size_t buflen)
{
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
