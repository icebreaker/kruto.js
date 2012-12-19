#ifndef BINDINGS_TIMER_HPP
#define BINDINGS_TIMER_HPP

namespace kruto { namespace bindings {  namespace timer {

Handle<Value> add(const Arguments &pArgs)
{
	HandleScope scope;

	if(pArgs.Length() < 3)
		return Undefined();

	Handle<Value> func = pArgs[0];
	if(!func->IsFunction())
		return Undefined();

	Script *script = static_cast<Script *>(External::Cast(*pArgs.Data())->Value());
	return Integer::New(script->addTimer(Handle<Function>::Cast(func), pArgs[1]->Uint32Value(), pArgs[2]->BooleanValue()));
}

Handle<Value> remove(const Arguments &pArgs)
{
	HandleScope scope;

	if(pArgs.Length() < 1)
		return Undefined();

	Script *script = static_cast<Script *>(External::Cast(*pArgs.Data())->Value());
	script->removeTimer(pArgs[0]->Int32Value());
	return Undefined();
}

Handle<Value> delay(const Arguments &pArgs)
{
	HandleScope scope;

	if(pArgs.Length() < 1)
		return Undefined();

	Timer::delay(pArgs[0]->Uint32Value());
	return Undefined();
}

/* timer */ } /* bindings */ } /* kruto */ }

#endif
