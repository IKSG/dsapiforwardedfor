#include <string.h>
#include <stdlib.h>

#include "global.h"
#include "osenv.h"
#include "addin.h"
#include "dsapi.h"

struct AllowedIP {
	char* val;
	struct AllowedIP* next;
};

static char* ENV_PROXYIPS = "X_Forwarded_ProxyIPs";
static struct AllowedIP* allowedIPs;

#define BUFFER_SIZE 4096

int processRequest(FilterContext* context, FilterRawRequest* eventData);

int FilterInit(FilterInitData* filterInitData) {
	AddInLogMessageText("[X-Forwarded] FilterInit", 0);
	filterInitData->appFilterVersion = kInterfaceVersion;
	filterInitData->eventFlags = kFilterRawRequest;
	strcpy(filterInitData->filterDesc, "X-Forwarded Support");

	// Find our configured allowed proxies
	char ipbuf[MAXENVVALUE];
	if(!OSGetEnvironmentString(ENV_PROXYIPS, ipbuf, MAXENVVALUE)) {
		AddInLogMessageText("[X-Forwarded] Unable to load IPs from %s environment variable", 0, ENV_PROXYIPS);
		return kFilterNotHandled;
	}
	AddInLogMessageText("[X-Forwarded] Initializing with allowed IPs %s", 0, ipbuf);

	char* p = strtok(ipbuf, ",");
	struct AllowedIP* prev = NULL;
	while(p) {
		if(!prev) {
			// Then start our new list
			allowedIPs = (struct AllowedIP*)calloc(1, sizeof(struct AllowedIP));
			allowedIPs->val = (char*)malloc(sizeof(char)*strlen(p));
			strcpy(allowedIPs->val, p);
			prev = allowedIPs;
		} else {
			// Then prev is something - work from there
			struct AllowedIP* next = (struct AllowedIP*)calloc(1, sizeof(struct AllowedIP));
			next->val = (char*)malloc(sizeof(char)*(strlen(p)+1));
			strcpy(next->val, p);
			prev->next = next;
			prev = next;
		}
		p = strtok(NULL, ",");
	}

	return kFilterHandledEvent;
}

unsigned int TerminateFilter(unsigned int reserved) {
	AddInLogMessageText("[X-Forwarded] TerminateFilter", 0);

	struct AllowedIP* ip = allowedIPs;
	while(ip) {
		struct AllowedIP* target = ip;
		ip = ip->next;
		free(target);
	}

	return kFilterHandledEvent;
}

int HttpFilterProc(FilterContext* context, unsigned int eventType, void* eventData) {
	if(!allowedIPs) {
		// Exit early
		return kFilterNotHandled;
	}

	switch (eventType) {
		case kFilterRawRequest:
			return processRequest(context, (FilterRawRequest*)eventData);
		default:
			return kFilterNotHandled;
	}
}

int processRequest(FilterContext* context, FilterRawRequest* rawRequest) {
	AddInLogMessageText("[X-Forwarded] Handling kFilterRawRequest", 0);

	char buf[256];
	int errId;
	context->GetServerVariable(context, "Remote_Addr", buf, 256, &errId);
	AddInLogMessageText("Got host %s", 0, buf);
	rawRequest->GetHeader(context, "X-Forwarded-For", buf, 256, &errId);
	AddInLogMessageText("Got xff %s", 0, buf);

	return kFilterHandledEvent;
}