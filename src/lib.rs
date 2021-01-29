use core::ffi::c_void;
use std::ffi::CString;
use std::ffi::CStr;
use std::ptr::null;

mod notesapi;
use crate::notesapi::FilterReturnCode_kFilterHandledEvent;
use crate::notesapi::FilterReturnCode_kFilterNotHandled;
use crate::notesapi::EventFlags_kFilterParsedRequest;
use crate::notesapi::kInterfaceVersion;
use crate::notesapi::FilterInitData;
use crate::notesapi::AddInLogMessageText;
use crate::notesapi::FilterContext;
use crate::notesapi::FilterRequest;

// **********************************************************************
// * Main Filter
// **********************************************************************

#[no_mangle]
pub extern "C" fn FilterInit(filter_init_data:*mut FilterInitData)->u32 {
	let msg = CString::new("[dsapiforwardedfor] FilterInit\n").expect("");
	unsafe { AddInLogMessageText(msg.as_ptr(), 0); }

	unsafe {
		(*filter_init_data).appFilterVersion = kInterfaceVersion;
		(*filter_init_data).eventFlags = EventFlags_kFilterParsedRequest;

		let name = CString::new("X-Forwarded-* Support").expect("");
		let bytes = name.as_bytes();
		(*filter_init_data).filterDesc[0..bytes.len()].copy_from_slice(bytes);
	}
	
	return FilterReturnCode_kFilterHandledEvent;
}

#[no_mangle]
pub extern "C" fn TerminateFilter(_reserved:usize)->u32 {
	let msg = CString::new("[dsapiforwardedfor] TerminateFilter\n").expect("");
	unsafe { AddInLogMessageText(msg.as_ptr(), 0); }
	return FilterReturnCode_kFilterHandledEvent;
}

#[no_mangle]
pub extern "C" fn HttpFilterProc(context:*const FilterContext, event_type:u32, _event_data:*const c_void)->u32 {
	if event_type == EventFlags_kFilterParsedRequest {
		let msg = CString::new("[dsapiforwardedfor] Handling Request\n").expect("");
		unsafe { AddInLogMessageText(msg.as_ptr(), 0); }

		let req = FilterRequest {
			method: 0,
			URL: &mut 0,
			version: &mut 0,
			userName: &mut 0,
			password: &mut 0,
			clientCert: &mut 0,
			clientCertLen: 0,
			contentRead: &mut 0,
			contentReadLen: 0
		};
		let mut errid:u32 = 0;
		let ctx = &*context;
		ctx.GetRequest(context, &req, &errid);

		return FilterReturnCode_kFilterHandledEvent;
	}
	return FilterReturnCode_kFilterNotHandled;
}