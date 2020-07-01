#include <node.h>
#include "k_means.cpp"

using namespace v8;
using namespace std;

vector<dvect_t> unpack_k_means(const FunctionCallbackInfo<Value>& args)
{
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();

  Local<Array> dots = Local<Array>::Cast(args[0]);
  vector<dvect_t> dots_vector;
  
  for (size_t i = 0; i < dots->Length(); i++)
  {
    Local<Array> dot = Local<Array>::Cast(dots->Get(i));
    dvect_t dot_vector;
    for (size_t j = 0; j < dot->Length(); j++)
    {
        double value = dot->Get(j)->NumberValue(context).FromJust();
        dot_vector.emplace_back(value);
    }
    dots_vector.emplace_back(dot_vector);    
  }

  return dots_vector;
}

Local<Array> pack_k_means(ivect_t source, Isolate* isolate)
{
  Local<Array> packed = Array::New(isolate, source.size());
  for (size_t i = 0; i < source.size(); i++)
  {
    packed->Set(i, Number::New(isolate, source[i]));
  }
  return packed;
}

void k_means_inner(const FunctionCallbackInfo<Value>& args, function<ivect_t(const vector<dvect_t>&, size_t, size_t)> algorithm)
{
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();

  vector<dvect_t> unpacked = unpack_k_means(args); 
  int clusters = args[1]->Int32Value(context).FromJust();
  
  ivect_t classified = algorithm(unpacked, unpacked[0].size(), clusters);
  Local<Array> packed = pack_k_means(classified, isolate);

  args.GetReturnValue().Set(packed);
}

void k_means(const FunctionCallbackInfo<Value>& args) {
  k_means_inner(args, k_means::make_classification);
}

void k_means_pp(const FunctionCallbackInfo<Value>& args) {
  k_means_inner(args, k_means::make_classification_pp);
}

void k_means_pp_manhattan(const FunctionCallbackInfo<Value>& args) {
  k_means_inner(args, k_means::make_classification_pp_manhattan);
}

void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "k_means", k_means);
    NODE_SET_METHOD(exports, "k_means_pp", k_means_pp);
    NODE_SET_METHOD(exports, "k_means_pp_manhattan", k_means_pp_manhattan);
}

NODE_MODULE(bridge, init)