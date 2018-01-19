#ifndef TABLE_H_INCLUDED
#define TABLE_H_INCLUDED

#include <errno.h>
#include <signal.h>
#include <stdio.h>

#include "tests.h"

static struct {
#define RES_OK      0
#define RES_CRASH   1
#define RES_TIMEOUT 2
#define RES_STOP    3
	int result;
	int data[2];
} test_results[testCases_FILE_count][testCases_CSTR_count];

static const char *errnoname(int errnum)
{
	switch(errnum)
	{
	case 0: return "Success";
	case EACCES: return "EACCES";
	case EADDRINUSE: return "EADDRINUSE";
	case EADDRNOTAVAIL: return "EADDRNOTAVAIL";
	case EAFNOSUPPORT: return "EAFNOSUPPORT";
	case EAGAIN: return "EAGAIN";
	case EALREADY: return "EALREADY";
	case EBADE: return "EBADE";
	case EBADF: return "EBADF";
	case EBADFD: return "EBADFD";
	case EBADMSG: return "EBADMSG";
	case EBADR: return "EBADR";
	case EBADRQC: return "EBADRQC";
	case EBADSLT: return "EBADSLT";
	case EBUSY: return "EBUSY";
	case ECANCELED: return "ECANCELED";
	case ECHILD: return "ECHILD";
	case ECHRNG: return "ECHRNG";
	case ECOMM: return "ECOMM";
	case ECONNABORTED: return "ECONNABORTED";
	case ECONNREFUSED: return "ECONNREFUSED";
	case ECONNRESET: return "ECONNRESET";
	case EDEADLK: return "EDEADLK";
	case EDESTADDRREQ: return "EDESTADDRREQ";
	case EDOM: return "EDOM";
	case EDQUOT: return "EDQUOT";
	case EEXIST: return "EEXIST";
	case EFAULT: return "EFAULT";
	case EFBIG: return "EFBIG";
	case EHOSTDOWN: return "EHOSTDOWN";
	case EHOSTUNREACH: return "EHOSTUNREACH";
	case EIDRM: return "EIDRM";
	case EILSEQ: return "EILSEQ";
	case EINPROGRESS: return "EINPROGRESS";
	case EINTR: return "EINTR";
	case EINVAL: return "EINVAL";
	case EIO: return "EIO";
	case EISCONN: return "EISCONN";
	case EISDIR: return "EISDIR";
	case EISNAM: return "EISNAM";
	case EKEYEXPIRED: return "EKEYEXPIRED";
	case EKEYREJECTED: return "EKEYREJECTED";
	case EKEYREVOKED: return "EKEYREVOKED";
	case ELIBACC: return "ELIBACC";
	case ELIBBAD: return "ELIBBAD";
	case ELIBMAX: return "ELIBMAX";
	case ELIBSCN: return "ELIBSCN";
	case ELIBEXEC: return "ELIBEXEC";
	case ELOOP: return "ELOOP";
	case EMEDIUMTYPE: return "EMEDIUMTYPE";
	case EMFILE: return "EMFILE";
	case EMLINK: return "EMLINK";
	case EMSGSIZE: return "EMSGSIZE";
	case EMULTIHOP: return "EMULTIHOP";
	case ENAMETOOLONG: return "ENAMETOOLONG";
	case ENETDOWN: return "ENETDOWN";
	case ENETRESET: return "ENETRESET";
	case ENETUNREACH: return "ENETUNREACH";
	case ENFILE: return "ENFILE";
	case ENOBUFS: return "ENOBUFS";
	case ENODATA: return "ENODATA";
	case ENODEV: return "ENODEV";
	case ENOENT: return "ENOENT";
	case ENOEXEC: return "ENOEXEC";
	case ENOKEY: return "ENOKEY";
	case ENOLCK: return "ENOLCK";
	case ENOLINK: return "ENOLINK";
	case ENOMEDIUM: return "ENOMEDIUM";
	case ENOMEM: return "ENOMEM";
	case ENOMSG: return "ENOMSG";
	case ENONET: return "ENONET";
	case ENOPKG: return "ENOPKG";
	case ENOPROTOOPT: return "ENOPROTOOPT";
	case ENOSPC: return "ENOSPC";
	case ENOSR: return "ENOSR";
	case ENOSTR: return "ENOSTR";
	case ENOSYS: return "ENOSYS";
	case ENOTBLK: return "ENOTBLK";
	case ENOTCONN: return "ENOTCONN";
	case ENOTDIR: return "ENOTDIR";
	case ENOTEMPTY: return "ENOTEMPTY";
	case ENOTSOCK: return "ENOTSOCK";
	case ENOTSUP: return "ENOTSUP";
	case ENOTTY: return "ENOTTY";
	case ENOTUNIQ: return "ENOTUNIQ";
	case ENXIO: return "ENXIO";
	case EOVERFLOW: return "EOVERFLOW";
	case EPERM: return "EPERM";
	case EPFNOSUPPORT: return "EPFNOSUPPORT";
	case EPIPE: return "EPIPE";
	case EPROTO: return "EPROTO";
	case EPROTONOSUPPORT: return "EPROTONOSUPPORT";
	case EPROTOTYPE: return "EPROTOTYPE";
	case ERANGE: return "ERANGE";
	case EREMCHG: return "EREMCHG";
	case EREMOTE: return "EREMOTE";
	case EREMOTEIO: return "EREMOTEIO";
	case ERESTART: return "ERESTART";
	case EROFS: return "EROFS";
	case ESHUTDOWN: return "ESHUTDOWN";
	case ESPIPE: return "ESPIPE";
	case ESOCKTNOSUPPORT: return "ESOCKTNOSUPPORT";
	case ESRCH: return "ESRCH";
	case ESTALE: return "ESTALE";
	case ESTRPIPE: return "ESTRPIPE";
	case ETIME: return "ETIME";
	case ETIMEDOUT: return "ETIMEDOUT";
	case ETXTBSY: return "ETXTBSY";
	case EUCLEAN: return "EUCLEAN";
	case EUNATCH: return "EUNATCH";
	case EUSERS: return "EUSERS";
	case EXDEV: return "EXDEV";
	case EXFULL: return "EXFULL";
	default: return "&lt;unknown>";
	}
}
static const char *signame(int sig)
{
	switch(sig)
	{
	case SIGHUP: return "SIGHUP";
	case SIGINT: return "SIGINT";
	case SIGQUIT: return "SIGQUIT";
	case SIGILL: return "SIGILL";
	case SIGABRT: return "SIGABRT";
	case SIGFPE: return "SIGFPE";
	case SIGKILL: return "SIGKILL";
	case SIGSEGV: return "SIGSEGV";
	case SIGPIPE: return "SIGPIPE";
	case SIGALRM: return "SIGALRM";
	case SIGTERM: return "SIGTERM";
	case SIGCHLD: return "SIGCHLD";
	case SIGCONT: return "SIGCONT";
	case SIGSTOP: return "SIGSTOP";
	case SIGTSTP: return "SIGTSTP";
	case SIGTTIN: return "SIGTTIN";
	case SIGTTOU: return "SIGTTOU";
	case SIGBUS: return "SIGBUS";
	case SIGPROF: return "SIGPROF";
	case SIGSYS: return "SIGSYS";
	case SIGTRAP: return "SIGTRAP";
	case SIGURG: return "SIGURG";
	case SIGVTALRM: return "SIGVTALRM";
	case SIGXCPU: return "SIGXCPU";
	case SIGXFSZ: return "SIGXFSZ";
	case SIGIO: return "SIGIO";
	case SIGPWR: return "SIGPWR";
	case SIGWINCH: return "SIGWINCH";
	default: return "&lt;unknown>";
	}
}

static void print_results(const char *name, FILE *fp)
{
	static const char *classes[] = {"ok", "crash", "timeout", "stop"};

	if(name)
	{
		fp = fopen(name, "wb");
		if(!fp)
			return;
	}

	fputs("<!DOCTYPE html ["
			"<!ENTITY a \"{border-\">"
			"<!ENTITY b \"{background:\">"
			"<!ENTITY s \"<td class='crash'>SIGSEGV</td>\">"
			"<!ENTITY t \"<td class='ok'>1 (Success)</td>\">"
			"<!ENTITY u \"<td class='ok'>-1 (EBADF)</td>\">"
			"]>"
			"<html xmlns=\"http://www.w3.org/1999/xhtml\">"
				"<head>"
					"<style type=\"text/css\">"
						"table&a;collapse:collapse;}"
						"td,th{text-align:center;border:0.1em solid black;}"
						"tr:first-child>*""&a;top:0;}"
						"tr>:first-child" "&a;left:0;}"
						"tr:last-child>*" "&a;bottom:0;}"
						"tr>:last-child"  "&a;right:0;}"
						"td.ok"     "&b;lightgreen;}"
						"td.crash"  "&b;salmon;}"
						"td.timeout""&b;yellow;}"
						"td.stop"   "&b;lightblue;}"
					"</style>"
				"</head>"
				"<body>"
					"<table>"
						"<tr>"
							"<th/>", fp);
	for(int j = 0; j < testCases_CSTR_count; j++)
		fprintf(fp, "<th>%s</th>", testCases_CSTR[j].desc);

	for(int i = 0; i < testCases_FILE_count; i++)
	{
		fprintf(fp, "</tr><tr><th>%s</th>", testCases_FILE[i].desc);
		for(int j = 0; j < testCases_CSTR_count; j++)
		{
			if(test_results[i][j].result == RES_CRASH && test_results[i][j].data[0] == SIGSEGV)
				fputs("&s;", fp);
			else if(test_results[i][j].result == RES_OK && test_results[i][j].data[0] == 1)
				fputs("&t;", fp);
			else if(test_results[i][j].result == RES_OK && test_results[i][j].data[0] == EOF && test_results[i][j].data[1] == EBADF)
				fputs("&u;", fp);
			else
			{
				fprintf(fp, "<td class=\"%s\">", classes[test_results[i][j].result]);
				if(test_results[i][j].result == RES_OK)
					fprintf(fp, "%d (%s)", test_results[i][j].data[0], errnoname(test_results[i][j].data[1]));
				else if(test_results[i][j].result == RES_CRASH || test_results[i][j].result == RES_STOP)
					fputs(signame(test_results[i][j].data[0]), fp);
				fputs("</td>", fp);
			}
		}
	}
	fputs("</tr></table></body></html>\n", fp);

	if(name)
		fclose(fp);
}

#endif
