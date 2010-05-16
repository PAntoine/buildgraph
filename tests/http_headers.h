/*--------------------------------------------------------------------------------*
 * Name: Parser Look-up tree
 * Desc: function and data structures to speed up a specific string search
 *
 *         **** DO NOT AMEND THIS CODE - IT IS AUTO_GENERATED ****
 *
 * Code and table produced by:
 *            build_graph 
 *            version 0.3
 *
 *  written by Peter Antoine. 
 *
 *   **** This code can be used however you like, no claims whatsoever are ****
 *   **** placed on the auto-generated code or data, or on the code that   ****
 *   **** uses the that code or data.                                      ****
 *   **** Or, to paraphrase "Fill you're boots".                           ****
 *--------------------------------------------------------------------------------*/

#ifndef	__HTTP_HEADERS_H__
#define __HTTP_HEADERS_H__

typedef	struct
{
	char*			name;
	unsigned int	length;
} HTTP_HEADERS_STRING_TABLE;

typedef enum
{
	HTTP_HST_CACHE_CONTROL,
	HTTP_HST_CONNECTION,
	HTTP_HST_DATE,
	HTTP_HST_PRAGMA,
	HTTP_HST_TRAILER,
	HTTP_HST_TRANSFER_ENCODING,
	HTTP_HST_UPGRADE,
	HTTP_HST_VIA,
	HTTP_HST_WARNING,
	HTTP_HST_ACCEPT,
	HTTP_HST_ACCEPT_CHARSET,
	HTTP_HST_ACCEPT_ENCODING,
	HTTP_HST_ACCEPT_LANGUAGE,
	HTTP_HST_AUTHORIZATION,
	HTTP_HST_EXPECT,
	HTTP_HST_FROM,
	HTTP_HST_HOST,
	HTTP_HST_IF_MATCH,
	HTTP_HST_IF_MODIFIED_SINCE,
	HTTP_HST_IF_NONE_MATCH,
	HTTP_HST_IF_RANGE,
	HTTP_HST_IF_UNMODIFIED_SINCE,
	HTTP_HST_MAX_FORWARDS,
	HTTP_HST_PROXY_AUTHORIZATION,
	HTTP_HST_RANGE,
	HTTP_HST_REFERER,
	HTTP_HST_TE,
	HTTP_HST_USER_AGENT,
	HTTP_HST_ACCEPT_RANGES,
	HTTP_HST_AGE,
	HTTP_HST_ETAG,
	HTTP_HST_LOCATION,
	HTTP_HST_PROXY_AUTHENTICATE,
	HTTP_HST_RETRY_AFTER,
	HTTP_HST_SERVER,
	HTTP_HST_VARY,
	HTTP_HST_WWW_AUTHENTICATE,
	HTTP_HST_ALLOW,
	HTTP_HST_CONTENT_ENCODING,
	HTTP_HST_CONTENT_LANGUAGE,
	HTTP_HST_CONTENT_LENGTH,
	HTTP_HST_CONTENT_LOCATION,
	HTTP_HST_CONTENT_MD5,
	HTTP_HST_CONTENT_RANGE,
	HTTP_HST_CONTENT_TYPE,
	HTTP_HST_EXPIRES,
	HTTP_HST_LAST_MODIFIED,
	HTTP_HST_EXTENSION_HEADER,
	HTTP_HST_KEEP_ALIVE,
	HTTP_HST_COOKIE,
	HTTP_HST_SET_COOKIE,
	HTTP_HST_SET_COOKIE2
} HTTP_HST_HTTP_HEADERS;

int	http_headers_check_word(unsigned char* word);

#endif
