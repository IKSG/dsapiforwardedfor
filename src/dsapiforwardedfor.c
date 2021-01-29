#include "dsapi.h"

int FilterInit(FilterInitData* filterInitData) {
	return kFilterHandledEvent;
}

unsigned int TerminateFilter(unsigned int reserved) {
	return kFilterHandledEvent;
}

int HttpFilterProc(FilterContext* context, unsigned int eventType, void* eventData) {
	switch (eventType) {
		default:
			return kFilterNotHandled;
	}
}