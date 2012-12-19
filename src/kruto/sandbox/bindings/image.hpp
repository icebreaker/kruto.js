#ifndef BINDINGS_IMAGE_HPP
#define BINDINGS_IMAGE_HPP

namespace kruto { namespace bindings {  namespace image {

Handle<Value> width(Local<String> pProperty, const AccessorInfo& pInfo)
{
	KR_UNUSED(pProperty);
	Image *image = static_cast<Image *>(External::Cast(*pInfo.This()->GetInternalField(0))->Value());
	return Number::New(image->width());
}
 
Handle<Value> height(Local<String> pProperty, const AccessorInfo& pInfo)
{
	KR_UNUSED(pProperty);
	Image *image = static_cast<Image *>(External::Cast(*pInfo.This()->GetInternalField(0))->Value());
	return Number::New(image->height());
}
 
Handle<Value> filenameGet(Local<String> pProperty, const AccessorInfo& pInfo)
{
	KR_UNUSED(pProperty);
	Image *image = static_cast<Image *>(External::Cast(*pInfo.This()->GetInternalField(0))->Value());
	return String::New(image->fileName());
}

void filenameSet(Local<String> pProperty, Local<Value> pValue, const AccessorInfo& pInfo)
{
	KR_UNUSED(pProperty);
	String::Utf8Value filename(pValue);
	Image *image = static_cast<Image *>(External::Cast(*pInfo.This()->GetInternalField(0))->Value());
	image->setFileName(*filename);
}

Handle<Value> create(const Arguments &pArgs)
{
	HandleScope scope;

	if (!pArgs.IsConstructCall()) 
		return ThrowException(String::New("Object constructor cannot be called as a function."));

	Script *script = static_cast<Script *>(External::Cast(*pArgs.Data())->Value());
	pArgs.This()->SetInternalField(0, External::New(script->createImage()));

	return pArgs.This();
}

Handle<Value> load(const Arguments &pArgs)
{
	HandleScope scope;
	Image *image = static_cast<Image *>(External::Cast(*pArgs.This()->GetInternalField(0))->Value());
	return Boolean::New(image->load());
}

/* image */ } /* bindings */ } /* kruto */ }

#endif
