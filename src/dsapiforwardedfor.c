#include <string.h>
#include <stdlib.h>

#include "global.h"
#include "osenv.h"
#include "addin.h"
#include "dsapi.h"

#define ENV_SECRET "HTTPConnectorHeadersSecret"
#define HEADER_SECRET "X-ConnectorHeaders-Secret"

char* WS_HEADERS[] = {
	"$WSSP",
	"$WSSN",
	"$WSRU",
	"$WSRH",
	"$WSRA",
	"$WSLA",
	"$WSPR",
	"$WSIS",
	"$WSCC",
	"$WSAT",
	"$WSKS"
};
int WS_HEADERS_LEN = 11;

static char secret[MAXENVVALUE];
static int enabled;

#define BUFFER_SIZE 4096

int processRequest(FilterContext* context, FilterRawRequest* eventData);
int startRequest(FilterContext* context);


// *******************************************************************************
// * DSAPI filter API
// *******************************************************************************

int FilterInit(FilterInitData* filterInitData) {
	filterInitData->appFilterVersion = kInterfaceVersion;
	filterInitData->eventFlags = kFilterRawRequest;
	strcpy(filterInitData->filterDesc, "HTTPConnectorHeaders Secret Filter");

	// Find our configured allowed proxies
	if(!OSGetEnvironmentString(ENV_SECRET, secret, MAXENVVALUE)) {
		AddInLogMessageText("[HTTPConnectorHeadersSecret] Unable to headers secret from %s environment variable", 0, ENV_SECRET);
		return kFilterNotHandled;
	}
	enabled = strlen(secret) > 0;

	return kFilterHandledEvent;
}

unsigned int TerminateFilter(unsigned int reserved) {
	return kFilterHandledEvent;
}

int HttpFilterProc(FilterContext* context, unsigned int eventType, void* eventData) {
	if(!enabled) {
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

// *******************************************************************************
// * Internal processing functions
// *******************************************************************************

int processRequest(FilterContext* context, FilterRawRequest* rawRequest) {

	char buf[256];
	int errId;
	for(int i = 0; i < WS_HEADERS_LEN; i++) {
		rawRequest->GetHeader(context, WS_HEADERS[i], buf, 256, &errId);
		if(strlen(buf) > 0) {
			// Check if the secret is present
			rawRequest->GetHeader(context, HEADER_SECRET, buf, 256, &errId);
			if(strcmp(secret, buf) == 0) {
				// Then all is well - allow the authorized request
			} else {
				// Then this is unauthorized - declare that we'll handle the request and let it fall
				//   off the edge of the world
				context->ServerSupport(context, kOwnsRequest, NULL, NULL, 0, &errId);
			}

			// We're done here one way or another
			return kFilterHandledEvent;
		}
	}

	return kFilterHandledEvent;
}