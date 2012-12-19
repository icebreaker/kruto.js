#ifndef BINDINGS_CORE_HPP
#define BINDINGS_CORE_HPP

namespace kruto { namespace bindings {  namespace core {

Handle<Value> log(const Arguments &pArgs)
{
	HandleScope scope;

	if(pArgs.Length() < 1) 
		return Undefined();

	printf("[script]");
	for(int i=0; i<pArgs.Length(); i++)
	{
		Handle<Value> arg = pArgs[i];
		String::Utf8Value value(arg);
		printf(" %s", *value);
	}
	printf("\n");

	return Undefined();
}

Handle<Value> require(const Arguments &pArgs)
{
	HandleScope scope;

	if(pArgs.Length() < 1) 
		return Undefined();

	Handle<Value> arg = pArgs[0];
	String::Utf8Value value(arg);

	Script *script = static_cast<Script *>(External::Cast(*pArgs.Data())->Value());
	return Boolean::New(script->require(*value));
}

Handle<Value> read(const Arguments &pArgs)
{
	HandleScope scope;

	if(pArgs.Length() < 1) 
		return Undefined();

	Handle<Value> arg = pArgs[0];
	String::Utf8Value value(arg);

	return String::New(File::read(*value));
}

Handle<Value> versionGet(Local<String> pProperty, const AccessorInfo& pInfo)
{
	KR_UNUSED(pProperty);
	KR_UNUSED(pInfo);
	return String::New(KR_VERSION_STRING);
}

Handle<Value> nameGet(Local<String> pProperty, const AccessorInfo& pInfo)
{
	KR_UNUSED(pProperty);
	KR_UNUSED(pInfo);
	return String::New(KR_VERSION_NAME);
}

Handle<Value> argumentsGet(Local<String> pProperty, const AccessorInfo& pInfo)
{
	KR_UNUSED(pProperty);
	HandleScope scope;

	Application *app = static_cast<Application *>(External::Cast(*pInfo.Data())->Value());

	Handle<Array> value = Array::New(app->argc());
	
	for(int i=0; i<app->argc(); i++)
		value->Set(Integer::New(i), String::New(app->argv(i)));
	
	return value;
}

Handle<Value> widthGet(Local<String> pProperty, const AccessorInfo& pInfo)
{
	KR_UNUSED(pProperty);
	HandleScope scope;

	Application *app = static_cast<Application *>(External::Cast(*pInfo.Data())->Value());
	return Integer::New(app->width());
}

Handle<Value> heightGet(Local<String> pProperty, const AccessorInfo& pInfo)
{
	KR_UNUSED(pProperty);
	HandleScope scope;

	Application *app = static_cast<Application *>(External::Cast(*pInfo.Data())->Value());
	return Integer::New(app->height());
}

Handle<Value> titleGet(Local<String> pProperty, const AccessorInfo& pInfo)
{
	KR_UNUSED(pProperty);
	Application *app = static_cast<Application *>(External::Cast(*pInfo.Data())->Value());
	return String::New(app->title());
}
 
void titleSet(Local<String> pProperty, Local<Value> pValue, const AccessorInfo& pInfo)
{
	KR_UNUSED(pProperty);
	String::Utf8Value str(pValue);
	Application *app = static_cast<Application *>(External::Cast(*pInfo.Data())->Value());
	app->setTitle(*str);
}

Handle<Value> quit(const Arguments &pArgs)
{
	HandleScope scope;
	Script *script = static_cast<Script *>(External::Cast(*pArgs.Data())->Value());
	script->application()->quit();
	Console::log("[script] quit");
	return Undefined();
}

Handle<Value> emptyStub(const Arguments &pArgs)
{
	KR_UNUSED(pArgs);
	return Undefined();
}

/* core */ } /* bindings */ } /* kruto */ }

#endif
