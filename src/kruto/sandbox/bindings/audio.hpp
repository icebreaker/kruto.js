#ifndef BINDINGS_AUDIO_HPP
#define BINDINGS_AUDIO_HPP

namespace kruto { namespace bindings {  namespace audio {

Handle<Value> loopGet(Local<String> pProperty, const AccessorInfo& pInfo)
{
	KR_UNUSED(pProperty);
	Audio *audio = static_cast<Audio *>(External::Cast(*pInfo.This()->GetInternalField(0))->Value());
	return Boolean::New(audio->isLooping());
}

void loopSet(Local<String> pProperty, Local<Value> pValue, const AccessorInfo& pInfo)
{
	KR_UNUSED(pProperty);
	Audio *audio = static_cast<Audio *>(External::Cast(*pInfo.This()->GetInternalField(0))->Value());
	audio->setLoop(pValue->BooleanValue());
}

Handle<Value> volumeGet(Local<String> pProperty, const AccessorInfo& pInfo)
{
	KR_UNUSED(pProperty);
	Audio *audio = static_cast<Audio *>(External::Cast(*pInfo.This()->GetInternalField(0))->Value());
	return Number::New(audio->volume());
}

void volumeSet(Local<String> pProperty, Local<Value> pValue, const AccessorInfo& pInfo)
{
	KR_UNUSED(pProperty);
	Audio *audio = static_cast<Audio *>(External::Cast(*pInfo.This()->GetInternalField(0))->Value());
	audio->setVolume(pValue->NumberValue());
}

Handle<Value> filenameGet(Local<String> pProperty, const AccessorInfo& pInfo)
{
	KR_UNUSED(pProperty);
	Audio *audio = static_cast<Audio *>(External::Cast(*pInfo.This()->GetInternalField(0))->Value());
	return String::New(audio->fileName());
}

void filenameSet(Local<String> pProperty, Local<Value> pValue, const AccessorInfo& pInfo)
{
	KR_UNUSED(pProperty);
	String::Utf8Value filename(pValue);
	Audio *audio = static_cast<Audio *>(External::Cast(*pInfo.This()->GetInternalField(0))->Value());
	audio->setFileName(*filename);
}

Handle<Value> canplaytype(const Arguments &pArgs)
{
	HandleScope scope;

	if(pArgs.Length() < 1)
		return Undefined();

	String::Utf8Value type(pArgs[0]);
	return Boolean::New(!strcmp(*type, "audio/ogg") || !strcmp(*type, "audio/wave"));
}

Handle<Value> create(const Arguments &pArgs)
{
	HandleScope scope;

	if (!pArgs.IsConstructCall()) 
		return ThrowException(String::New("Object constructor cannot be called as a function."));

	Script *script = static_cast<Script *>(External::Cast(*pArgs.Data())->Value());
	pArgs.This()->SetInternalField(0, External::New(script->createAudio()));

	return pArgs.This();
}

Handle<Value> load(const Arguments &pArgs)
{
	HandleScope scope;
	Audio *audio = static_cast<Audio *>(External::Cast(*pArgs.This()->GetInternalField(0))->Value());
	return Boolean::New(audio->load());
}

Handle<Value> play(const Arguments &pArgs)
{
	HandleScope scope;
	Audio *audio = static_cast<Audio *>(External::Cast(*pArgs.This()->GetInternalField(0))->Value());
	audio->play();
	return Undefined();
}

Handle<Value> pause(const Arguments &pArgs)
{
	HandleScope scope;
	Audio *audio = static_cast<Audio *>(External::Cast(*pArgs.This()->GetInternalField(0))->Value());
	audio->pause();
	return Undefined();
}

/* audio */ } /* bindings */ } /* kruto */ }

#endif
