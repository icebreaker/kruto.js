#ifndef BINDINGS_CANVAS_HPP
#define BINDINGS_CANVAS_HPP

namespace kruto { namespace bindings {  namespace canvas {

Handle<Value> setVideoMode(const Arguments &pArgs)
{
	HandleScope scope;

	if(pArgs.Length() < 2)
		return Undefined();

	return Boolean::New(Canvas::setVideoMode(
						pArgs[0]->Uint32Value(), 
						pArgs[1]->Uint32Value(), 
						pArgs.Length() >= 3 ? pArgs[2]->BooleanValue() : false,
						pArgs.Length() >= 4 ? *String::Utf8Value(pArgs[3]) : NULL));
}

Handle<Value> setClearColor(const Arguments &pArgs)
{
	HandleScope scope;

	if(pArgs.Length() < 1)
		return Undefined();

	String::Utf8Value color(pArgs[0]);
	Canvas::setClearColor(*color);
	return Undefined();
}

Handle<Value> save(const Arguments &pArgs)
{
	KR_UNUSED(pArgs);
	Canvas::save();
	return Undefined();
}

Handle<Value> restore(const Arguments &pArgs)
{
	KR_UNUSED(pArgs);
	Canvas::restore();
	return Undefined();
}

Handle<Value> scale(const Arguments &pArgs)
{
	HandleScope scope;

	if(pArgs.Length() < 2)
		return Undefined();

	Canvas::scale(pArgs[0]->NumberValue(),
				  pArgs[1]->NumberValue());

	return Undefined();
}

Handle<Value> rotate(const Arguments &pArgs)
{
	HandleScope scope;

	if(pArgs.Length() < 1)
		return Undefined();

	Canvas::rotate(pArgs[0]->NumberValue());
	return Undefined();
}

Handle<Value> translate(const Arguments &pArgs)
{
	HandleScope scope;

	if(pArgs.Length() < 2)
		return Undefined();

	Canvas::translate(pArgs[0]->NumberValue(),
					  pArgs[1]->NumberValue());

	return Undefined();
}

Handle<Value> transform(const Arguments &pArgs)
{
	HandleScope scope;

	if(pArgs.Length() < 6)
		return Undefined();

	Canvas::transform(pArgs[0]->NumberValue(),
					  pArgs[1]->NumberValue(), 
					  pArgs[2]->NumberValue(), 
					  pArgs[3]->NumberValue(),
					  pArgs[4]->NumberValue(),
					  pArgs[5]->NumberValue());

	return Undefined();
}

Handle<Value> setTransform(const Arguments &pArgs)
{
	HandleScope scope;

	if(pArgs.Length() < 6)
		return Undefined();

	Canvas::setTransform(pArgs[0]->NumberValue(),
						 pArgs[1]->NumberValue(), 
						 pArgs[2]->NumberValue(), 
						 pArgs[3]->NumberValue(),
						 pArgs[4]->NumberValue(),
						 pArgs[5]->NumberValue());

	return Undefined();
}

void setStrokeStyle(Local<String> pProperty, Local<Value> pValue, const AccessorInfo& pInfo)
{
	KR_UNUSED(pProperty);
	KR_UNUSED(pInfo);
	String::Utf8Value color(pValue);
	Canvas::setStrokeStyle(*color);
}

void setFillStyle(Local<String> pProperty, Local<Value> pValue, const AccessorInfo& pInfo)
{
	KR_UNUSED(pProperty);
	KR_UNUSED(pInfo);
	String::Utf8Value color(pValue);
	Canvas::setFillStyle(*color);
}

void setLineWidth(Local<String> pProperty, Local<Value> pValue, const AccessorInfo& pInfo)
{
	KR_UNUSED(pProperty);
	KR_UNUSED(pInfo);
	Canvas::setLineWidth(pValue->NumberValue());
}

Handle<Value> fillRect(const Arguments &pArgs)
{
	if(pArgs.Length() < 4)
		return Undefined();

	HandleScope scope;

	Canvas::fillRect(pArgs[0]->NumberValue(),
					 pArgs[1]->NumberValue(), 
					 pArgs[2]->NumberValue(), 
					 pArgs[3]->NumberValue());

	return Undefined();
}

Handle<Value> strokeRect(const Arguments &pArgs)
{
	if(pArgs.Length() < 4)
		return Undefined();

	HandleScope scope;

	Canvas::strokeRect(pArgs[0]->NumberValue(),
					   pArgs[1]->NumberValue(), 
					   pArgs[2]->NumberValue(), 
					   pArgs[3]->NumberValue());

	return Undefined();
}

Handle<Value> clearRect(const Arguments &pArgs)
{
	if(pArgs.Length() < 4)
		return Undefined();

	HandleScope scope;

	Canvas::clearRect(pArgs[0]->NumberValue(),
					  pArgs[1]->NumberValue(), 
					  pArgs[2]->NumberValue(), 
					  pArgs[3]->NumberValue());

	return Undefined();
}

Handle<Value> beginPolygon(const Arguments &pArgs)
{
	if(pArgs.Length() < 1)
		return Undefined();

	HandleScope scope;

	Canvas::beginPolygon(pArgs[0]->BooleanValue());

	return Undefined();
}

Handle<Value> drawPoint(const Arguments &pArgs)
{
	if(pArgs.Length() < 2)
		return Undefined();

	HandleScope scope;

	Canvas::drawPoint(pArgs[0]->NumberValue(),
					  pArgs[1]->NumberValue());

	return Undefined();
}

Handle<Value> endPolygon(const Arguments &pArgs)
{
	KR_UNUSED(pArgs);
	Canvas::endPolygon();
	return Undefined();
}

Handle<Value> drawImage(const Arguments &pArgs)
{
	if(pArgs.Length() < 3)
		return Undefined();

	HandleScope scope;

	Image *image = static_cast<Image *>(External::Cast(*pArgs[0]->ToObject()->GetInternalField(0))->Value());
	switch(pArgs.Length())
	{
		case 3:
			Canvas::drawImage(image,
					pArgs[1]->NumberValue(), 
					pArgs[2]->NumberValue());
			break;
		case 5:
			Canvas::drawImage(image,
					pArgs[1]->NumberValue(), 
					pArgs[2]->NumberValue(), 
					pArgs[3]->NumberValue(), 
					pArgs[4]->NumberValue());
			break;
		case 9:
			Canvas::drawImage(image,
					pArgs[1]->NumberValue(), 
					pArgs[2]->NumberValue(), 
					pArgs[3]->NumberValue(), 
					pArgs[4]->NumberValue(),
					pArgs[5]->NumberValue(),
					pArgs[6]->NumberValue(),
					pArgs[7]->NumberValue(),
					pArgs[8]->NumberValue());
			break;
	}

	return Undefined();
}

/* canvas */ } /* bindings */ } /* kruto */ }

#endif
