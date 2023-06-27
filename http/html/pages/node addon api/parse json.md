# Parsing / Stringifying JSON in node addon api

```cpp
Napi::String JSONStringify(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Object json_object = info[0].As<Napi::Object>();
  Napi::Object json = env.Global().Get("JSON").As<Napi::Object>();
  Napi::Function stringify = json.Get("stringify").As<Napi::Function>();
  return stringify.Call(json, { json_object }).As<Napi::String>();
}

Napi::Object JSONParse(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::String json_string = info[0].As<Napi::String>();
  Napi::Object json = env.Global().Get("JSON").As<Napi::Object>();
  Napi::Function parse = json.Get("parse").As<Napi::Function>();
  return parse.Call(json, { json_string }).As<Napi::Object>();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports["jsonStringify"] =  Napi::Function::New(env, JSONStringify);
  exports["jsonParse"] = Napi::Function::New(env, JSONParse);          
  return exports;
}

NODE_API_MODULE(json, Init)
```