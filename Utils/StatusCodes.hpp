/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StatusCodes.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 01:32:29 by ehakam            #+#    #+#             */
/*   Updated: 2022/08/23 00:25:17 by ehakam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __STATUS_CODES_HPP__
#define __STATUS_CODES_HPP__

#include <iostream>

#define CRLF "\r\n"
#define SERVER_VERSION "Webserv/0.0.1 (MacOS)"
#define HTTP_VERSION "HTTP/1.1"

#define H_TRANSFER_ENCODING "Transfer-Encoding"
#define H_CONTENT_LENGTH "Content-Length"
#define H_CONTENT_TYPE "Content-Type"
#define H_LOCATION "Location"
#define H_DATE "Date"
#define H_SERVER "Server"
#define H_HOST "Host"
#define H_CONNECTION "Connection"
#define H_ALLOW "Allow"
#define H_AUTHORIZATION "Authorization"
#define H_STATUS "Status"
#define H_WWW_AUTHENTICATE "WWW-Authenticate"
#define H_COOKIE "Cookie"
#define H_SET_COOKIE "Set-Cookie"
#define H_CONTENT_LANGUAGE "Content-Language"
#define H_CONTENT_LOCATION "Content-Location"
#define H_CONTENT_RANGE "Content-Range"
#define H_ETAG "ETag"
#define H_HTTP_COOKIE "Http-Cookie"
#define H_EXPIRES "Expires"
#define H_LAST_MODIFIED "Last-Modified"
#define H_ACCEPT_RANGES "Accept-Ranges"
#define H_AGE "Age"
#define H_ETAG "ETag"
#define H_PROXY_AUTHENTICATE "Proxy-Authenticate"
#define H_PROXY_AUTHORIZATION "Proxy-Authorization"
#define H_RETRY_AFTER "Retry-After"
#define H_VARY "Vary"
#define H_VIA "Via"
#define H_WARNING "Warning"
#define H_X_POWERED_BY "X-Powered-By"
#define H_ALLOW_ORIGIN "Access-Control-Allow-Origin"
#define H_ALLOW_CREDENTIALS "Access-Control-Allow-Credentials"
#define H_ALLOW_HEADERS "Access-Control-Allow-Headers"

enum StatusCodes
{
	None = 0,

	Continue = 100,
	SwitchingProtocols = 101,
	Processing = 102,
	EarlyHints = 103,

	OK = 200,
	Created = 201,
	Accepted = 202,
	NonAuthoritativeInformation = 203,
	NoContent = 204,
	ResetContent = 205,
	PartialContent = 206,
	MultiStatus = 207,
	AlreadyReported = 208,
	IMUsed = 226,

	MultipleChoices = 300,
	MovedPermanently = 301,
	Found = 302,
	SeeOther = 303,
	NotModified = 304,
	UseProxy = 305,
	TemporaryRedirect = 307,
	PermanentRedirect = 308,

	BadRequest = 400,
	Unauthorized = 401,
	PaymentRequired = 402,
	Forbidden = 403,
	NotFound = 404,
	MethodNotAllowed = 405,
	NotAcceptable = 406,
	ProxyAuthenticationRequired = 407,
	RequestTimeout = 408,
	Conflict = 409,
	Gone = 410,
	LengthRequired = 411,
	PreconditionFailed = 412,
	PayloadTooLarge = 413,
	URITooLong = 414,
	UnsupportedMediaType = 415,
	RangeNotSatisfiable = 416,
	ExpectationFailed = 417,
	ImATeapot = 418,
	UnprocessableEntity = 422,
	Locked = 423,
	FailedDependency = 424,
	UpgradeRequired = 426,
	PreconditionRequired = 428,
	TooManyRequests = 429,
	RequestHeaderFieldsTooLarge = 431,
	UnavailableForLegalReasons = 451,

	InternalServerError = 500,
	NotImplemented = 501,
	BadGateway = 502,
	ServiceUnavailable = 503,
	GatewayTimeout = 504,
	HTTPVersionNotSupported = 505,
	VariantAlsoNegotiates = 506,
	InsufficientStorage = 507,
	LoopDetected = 508,
	NotExtended = 510,
	NetworkAuthenticationRequired = 511
};

inline const char *getReason(int code)
{
	switch (code)
	{

	case 0:
		return "None";

	// 1xx - Informational
	case 100:
		return "Continue";
	case 101:
		return "Switching Protocols";
	case 102:
		return "Processing";
	case 103:
		return "Early Hints";

	// 2xx - Successful
	case 200:
		return "OK";
	case 201:
		return "Created";
	case 202:
		return "Accepted";
	case 203:
		return "Non-Authoritative Information";
	case 204:
		return "No Content";
	case 205:
		return "Reset Content";
	case 206:
		return "Partial Content";
	case 207:
		return "Multi-Status";
	case 208:
		return "Already Reported";
	case 226:
		return "IM Used";

	// 3xx - Redirections
	case 300:
		return "Multiple Choices";
	case 301:
		return "Moved Permanently";
	case 302:
		return "Found";
	case 303:
		return "See Other";
	case 304:
		return "Not Modified";
	case 305:
		return "Use Proxy";
	case 307:
		return "Temporary Redirect";
	case 308:
		return "Permanent Redirect";

	// 4xx - Client Errors
	case 400:
		return "Bad Request";
	case 401:
		return "Unauthorized";
	case 402:
		return "Payment Required";
	case 403:
		return "Forbidden";
	case 404:
		return "Not Found";
	case 405:
		return "Method Not Allowed";
	case 406:
		return "Not Acceptable";
	case 407:
		return "Proxy Authentication Required";
	case 408:
		return "Request Timeout";
	case 409:
		return "Conflict";
	case 410:
		return "Gone";
	case 411:
		return "Length Required";
	case 412:
		return "Precondition Failed";
	case 413:
		return "Payload Too Large";
	case 414:
		return "URI Too Long";
	case 415:
		return "Unsupported Media Type";
	case 416:
		return "Range Not Satisfiable";
	case 417:
		return "Expectation Failed";
	case 418:
		return "I'm a teapot";
	case 422:
		return "Unprocessable Entity";
	case 423:
		return "Locked";
	case 424:
		return "Failed Dependency";
	case 426:
		return "Upgrade Required";
	case 428:
		return "Precondition Required";
	case 429:
		return "Too Many Requests";
	case 431:
		return "Request Header Fields Too Large";
	case 451:
		return "Unavailable For Legal Reasons";

	// 5xx - Server Errors
	case 500:
		return "Internal Server Error";
	case 501:
		return "Not Implemented";
	case 502:
		return "Bad Gateway";
	case 503:
		return "Service Unavailable";
	case 504:
		return "Gateway Time-out";
	case 505:
		return "HTTP Version Not Supported";
	case 506:
		return "Variant Also Negotiates";
	case 507:
		return "Insufficient Storage";
	case 508:
		return "Loop Detected";
	case 510:
		return "Not Extended";
	case 511:
		return "Network Authentication Required";

	default:
		return NULL;
	}
}

#endif
