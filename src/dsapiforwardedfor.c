#include <string.h>

#include "global.h"
#include "addin.h"
#include "dsapi.h"

int FilterInit(FilterInitData* filterInitData) {
	AddInLogMessageText("[dsapiforwardedfor] FilterInit\n", 0);
	filterInitData->appFilterVersion = kInterfaceVersion;
	filterInitData->eventFlags = kFilterProcessRequest;
	strcpy(filterInitData->filterDesc, "X-Forwarded Support");
	return kFilterHandledEvent;
}

unsigned int TerminateFilter(unsigned int reserved) {
	AddInLogMessageText("[dsapiforwardedfor] TerminateFilter\n", 0);
	return kFilterHandledEvent;
}

int HttpFilterProc(FilterContext* context, unsigned int eventType, void* eventData) {
	switch (eventType) {
		case kFilterProcessRequest:
			AddInLogMessageText("[dsapiforwardedfor] handled kFilterProcessRequest", 0);
			return kFilterHandledEvent;
		default:
			return kFilterNotHandled;
	}
}