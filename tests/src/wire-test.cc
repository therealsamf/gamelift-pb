
// clang-format off
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <google/protobuf/message.h>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/util/message_differencer.h>
#include <napi.h>
#include <sdk.pb.h>

#include "populate.hh"
#include "utility.hh"
// clang-format on

using namespace com::amazon::whitewater::auxproxy::pbuffer;

static Napi::Object faker_module;

/**
 * Check the name of the message against the corresponding protobuf message
 *
 * @param env NodeJS environment used for throwing exceptions.
 * @param descriptor Descriptor retrieved from the Protocol Buffer message.
 * @param message_type '$type' variable retrieved from the  Javascript class.
 *
 * @return Boolean for the assertion.
 */
bool CheckFullMessageName(Napi::Env& env,
                          google::protobuf::Descriptor const* const descriptor,
                          Napi::Object message_type);

/**
 * Check all combinations of message fields and that setting each one yields the
 * same result.
 *
 * @param env NodeJS environment used for throwing exceptions.
 * @param message
 * @param message_class
 *
 * @return Boolean for the assertion.
 */
bool CheckMessageFields(Napi::Env& env,
                        std::shared_ptr<google::protobuf::Message> message,
                        Napi::Object message_class);

std::string ToJson(Napi::Env& env, Napi::Object object);

Napi::Value FromJson(Napi::Env& env, const std::string& json_string);

/**
 * Compute the result after calling 'encode()'
 *
 * @param env NodeJS environment variable used for retrieving the global
 * require function.
 */
std::string GetResult(Napi::Env& env, const std::string& type_name,
                      const Napi::Object& message_type,
                      const Napi::Object& message_object);

/**
 * Main entrypoint into the test.
 *
 * Test that the given Javascript message class correctly emulates the
 * corresponding C++ protobuf class.
 *
 * @param info NodeJS callback information.
 */
Napi::Value CheckMessageWireFormat(const Napi::CallbackInfo& info) {

  std::srand(time(nullptr));

  Napi::Env env = info.Env();

  // Check that the minimum number of arguments were passed
  if (info.Length() < 2) {
    Napi::TypeError::New(env, "wrong number of arguments")
        .ThrowAsJavaScriptException();
    return env.Undefined();
  }

  // Check that the first argument is an object
  if (!info[0].IsObject() || !info[1].IsObject()) {
    Napi::TypeError::New(env, "expected object").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  faker_module = info[0].As<Napi::Object>();

  // Retrieve a reference to the Javascript class defintiion
  const Napi::Object& message_class = info[1].As<Napi::Object>();

  // Check that the class has a '$type' static member
  if (!message_class.Has("$type")) {
    Napi::Error::New(env, "class definition has no '$type' static member")
        .ThrowAsJavaScriptException();
    return env.Undefined();
  }

  // Retrieve a reference to the '$type' static member
  const Napi::Value& message_type_value = message_class.Get("$type");

  // Verify that the '$type' static member is an object
  if (!message_type_value.IsObject()) {
    Napi::TypeError::New(env, "expected '$type' static member to be object")
        .ThrowAsJavaScriptException();
    return env.Undefined();
  }

  // Retrieve a reference to the '$type' static member as an object
  const Napi::Object& message_type = message_type_value.As<Napi::Object>();

  // Verify that the '$type' static member has  'name' field
  if (!message_type.Has("name")) {
    Napi::TypeError::New(env,
                         "expected '$type' static member to have 'name' field")
        .ThrowAsJavaScriptException();
    return env.Undefined();
  }

  // Retrieve a reference to the 'name' field
  const Napi::Value& type_name_value = message_type.Get("name");

  // Verify that the 'name' field value is a string
  if (!type_name_value.IsString()) {
    Napi::TypeError::New(env, "expected '$type.name' to be string")
        .ThrowAsJavaScriptException();
    return env.Undefined();
  }

  // Retrieve a reference to the 'name' field as a string
  const std::string& type_name = type_name_value.As<Napi::String>().Utf8Value();

  std::shared_ptr<google::protobuf::Message> protobuf_message =
      GetMessagePointer(env, type_name);
  // If nullptr was returned then error occurred
  if (!protobuf_message) {
    return env.Undefined();
  }

  // Check all the fields for the message class.
  if (!CheckMessageFields(env, protobuf_message, message_class)) {
    return env.Undefined();
  }

  return env.Undefined();
}

bool CheckFullMessageName(Napi::Env& env,
                          google::protobuf::Descriptor const* const descriptor,
                          Napi::Object message_type) {
  const std::string& protobuf_name = descriptor->full_name();

  const Napi::Value& type_name_value = message_type.Get("fullName");
  if (!type_name_value.IsString()) {
    Napi::Error::New(env, "invalid result from '$type.fullName'")
        .ThrowAsJavaScriptException();
    return false;
  }

  const std::string& type_name = type_name_value.As<Napi::String>().Utf8Value();

  if (protobuf_name != type_name) {
    std::stringstream error_stream;
    error_stream << "invalid message name: '" << protobuf_name << "' != '"
                 << type_name << "'";
    Napi::Error::New(env, error_stream.str()).ThrowAsJavaScriptException();
    return false;
  }

  return true;
}

bool CheckMessageFields(Napi::Env& env,
                        std::shared_ptr<google::protobuf::Message> message,
                        Napi::Object message_class) {
  google::protobuf::Descriptor const* const descriptor =
      message->GetDescriptor();

  if (!message_class.Has("$type")) {
    Napi::Error::New(env, "class definition has no '$type' static member")
        .ThrowAsJavaScriptException();
    return false;
  }

  const Napi::Value& message_type_value = message_class.Get("$type");
  if (!message_type_value.IsObject()) {
    Napi::TypeError::New(env, "expected '$type' static member to be object")
        .ThrowAsJavaScriptException();
    return false;
  }

  const Napi::Object& message_type = message_type_value.As<Napi::Object>();

  if (!CheckFullMessageName(env, descriptor, message_type)) {
    return false;
  }
  const std::string& type_name =
      message_type.Get("name").As<Napi::String>().Utf8Value();

  if (!faker_module.Has("random") || !faker_module.Get("random").IsObject()) {
    Napi::TypeError::New(env, "Invalid 'random' member on faker module")
        .ThrowAsJavaScriptException();
    return false;
  }
  Napi::Object random_object = faker_module.Get("random").As<Napi::Object>();

  // For convenience bind this to a local variable
  random_boolean = std::bind(RandomBooleanFactory::GenerateRandomValue, env,
                             random_object, std::placeholders::_1);
  random_integer_32 = std::bind(RandomInt32Factory::GenerateRandomValue, env,
                                random_object, std::placeholders::_1);
  random_integer_64 = std::bind(RandomInt64Factory::GenerateRandomValue, env,
                                random_object, std::placeholders::_1);
  random_double = std::bind(RandomDoubleFactory::GenerateRandomValue, env,
                                random_object, std::placeholders::_1);
  random_string = std::bind(RandomStringFactory::GenerateRandomValue, env,
                            random_object, std::placeholders::_1);

  const int num_fields = descriptor->field_count();

  // Iterate through how many fields we're going to test at a time, starting
  // with 1 field, and then going all the way up to all the fields at once.
  //
  // For each iteration we're going to select every combination of that size
  // of fields to test with.
  for (int current_total_num_fields = 1; current_total_num_fields <= num_fields;
       current_total_num_fields++) {
    // Initialize the choices with the size of total number of them
    std::vector<int> possible_field_indices(num_fields);
    // Fill in the choices with 0, 1, ..., current_total_num_fields
    std::generate(possible_field_indices.begin(), possible_field_indices.end(),
                  [n = 0]() mutable { return n++; });

    // Iterate through every possible combination of size
    // current_total_num_fields within possible_field_indices
    do {
      // Grab a reference to the reflection in order to properly assign fields
      // to the underlying generic message
      google::protobuf::Reflection const* const reflection =
          message->GetReflection();

      // Iterate through all the fields chosen for this iteration and assign
      // them to both our message & the Javascript object.
      for (int i = 0; i < current_total_num_fields; ++i) {
        // This is the index of the field we're assigning
        int field_index = possible_field_indices[i];
        const google::protobuf::FieldDescriptor* field_descriptor =
            descriptor->field(field_index);
        const std::string& field_name = field_descriptor->camelcase_name();

        if (!PopulateProtocolBufferField(env, reflection, message.get(),
                                         field_descriptor)) {
          return false;
        }
      }

      std::string json_string;
      google::protobuf::util::JsonPrintOptions print_options;
      print_options.always_print_enums_as_ints = true;
      if (!google::protobuf::util::MessageToJsonString(*message, &json_string, print_options)
               .ok()) {
        Napi::Error::New(env,
                         "Failed to serialize Protocol Buffer to JSON string")
            .ThrowAsJavaScriptException();
        return false;
      }

      // Create an object we're eventually going to pass to 'encode()'
      Napi::Object message_object =
          FromJson(env, json_string).As<Napi::Object>();

      // Error has already been thrown
      if (message_object.IsUndefined()) {
        return false;
      }

      // Check for additional error
      if (env.IsExceptionPending()) {
        Napi::Error error = env.GetAndClearPendingException();
        std::stringstream error_stream;
        error_stream << "error occurred while attempting to create new object: "
                     << error.Message();
        Napi::Error::New(env, error_stream.str()).ThrowAsJavaScriptException();
        return false;
      }

      std::string protobuf_result = message->SerializePartialAsString();
      std::string js_result =
          GetResult(env, type_name, message_type, message_object);

      // Error thrown while extracting the JS result
      if (protobuf_result != js_result && !js_result.size()) {
        return false;
      }

      // Compare the two serialized results!
      if (protobuf_result != js_result) {
        // Tracks if the failure was for real
        bool for_real = false;

        std::shared_ptr<google::protobuf::Message> message_copy =
            GetMessagePointer(env, type_name);

        std::string nodejs_message;

        if (!message_copy->ParseFromString(js_result) || !google::protobuf::util::MessageToJsonString(*message_copy, &nodejs_message).ok()) {
          std::cerr << "Failed to parse JavaScript result" << std::endl;
          for_real = true;
        } else {

          google::protobuf::util::MessageDifferencer differencer;
          std::string difference_string;
          differencer.ReportDifferencesToString(&difference_string);
          differencer.set_message_field_comparison(google::protobuf::util::MessageDifferencer::EQUIVALENT);

          if (!differencer.Compare(
                  *message, *message_copy)) {
            std::cerr << "Differences: " << difference_string << std::endl;
            for_real = true;
          }
        }

        // TODO: Verify that every field in the JS result that isn't in the
        // protobuf_result is a field set to the default value.
        //
        // I'm holding off on that because I'm pretty sure it would involve a
        // JSON parser.

        if (for_real) {
          std::stringstream error_stream;
          error_stream << "Different results received for message '"
                       << type_name << "'\n";

          error_stream << "\tProtocol Buffer JSON message: '" << json_string
                       << "'\n";

          if (nodejs_message.size()) {
            error_stream << "\tNodeJS encoded message:       '"
                         << nodejs_message << "'\n";
          } else {
            error_stream << "\tNote: Error occurred while attempting to "
                            "stringify NodeJS encoded message.\n";
          }

          error_stream << "\tResult from 'sdk.pb.h': '" << protobuf_result
                       << "'\n";
          error_stream << "\tResult from '" << type_name << ".ts': '"
                       << js_result << "'\n";
          error_stream << "\tLength of 'sdk.pb.h' result: '"
                       << protobuf_result.size() << "'\n";
          error_stream << "\tLength of '" << type_name << ".ts' result: '"
                       << js_result.size() << "'\n";

          Napi::Error::New(env, error_stream.str())
              .ThrowAsJavaScriptException();
          return false;
        }
      }

      message->Clear();

    } while (next_combination(
        possible_field_indices.begin(),
        possible_field_indices.begin() + current_total_num_fields,
        possible_field_indices.end()));
  }

  return true;
}

std::string ToJson(Napi::Env& env, Napi::Object object) {
  std::string return_value;

  // Retrieve a reference to the global object in the passed NodeJS environment
  Napi::Object global_object = env.Global();
  if (env.IsExceptionPending()) {
    env.GetAndClearPendingException().ThrowAsJavaScriptException();
    return return_value;
  }

  if (!global_object.Has("JSON") || !global_object.Get("JSON").IsObject()) {
    Napi::TypeError::New(env, "Invalid result for 'JSON' global object")
        .ThrowAsJavaScriptException();
    return return_value;
  }

  const Napi::Object& json_object =
      global_object.Get("JSON").As<Napi::Object>();

  if (!json_object.Has("stringify") ||
      !json_object.Get("stringify").IsFunction()) {
    Napi::TypeError::New(env,
                         "Invalid type for 'JSON.stringify': function expected")
        .ThrowAsJavaScriptException();
    return return_value;
  }

  Napi::Function json_function =
      json_object.Get("stringify").As<Napi::Function>();
  return_value = json_function.Call({object, Napi::Number::New(env, 2)})
                     .As<Napi::String>()
                     .Utf8Value();

  if (env.IsExceptionPending()) {
    env.GetAndClearPendingException().ThrowAsJavaScriptException();
    return "";
  }
  return return_value;
}

Napi::Value FromJson(Napi::Env& env, const std::string& json_string) {
  // Retrieve a reference to the global object in the passed NodeJS environment
  Napi::Object global_object = env.Global();
  if (env.IsExceptionPending()) {
    env.GetAndClearPendingException().ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!global_object.Has("JSON") || !global_object.Get("JSON").IsObject()) {
    Napi::TypeError::New(env, "Invalid result for 'JSON' global object")
        .ThrowAsJavaScriptException();
    return env.Undefined();
  }

  const Napi::Object& json_object =
      global_object.Get("JSON").As<Napi::Object>();

  if (!json_object.Has("parse") || !json_object.Get("parse").IsFunction()) {
    Napi::TypeError::New(env,
                         "Invalid type for 'JSON.parse': function expected")
        .ThrowAsJavaScriptException();
    return env.Undefined();
  }

  Napi::Function json_function = json_object.Get("parse").As<Napi::Function>();
  Napi::Object return_value =
      json_function.Call({Napi::String::New(env, json_string)})
          .As<Napi::Object>();

  if (env.IsExceptionPending()) {
    env.GetAndClearPendingException().ThrowAsJavaScriptException();
    return env.Undefined();
  }
  return return_value;
}

std::string GetResult(Napi::Env& env, const std::string& type_name,
                      const Napi::Object& message_type,
                      const Napi::Object& message_object) {
  std::string return_value;

  // Get the 'encode()' function
  Napi::Value encode_function_value = message_type.Get("encode");
  if (encode_function_value.IsUndefined() ||
      !encode_function_value.IsFunction()) {
    Napi::TypeError::New(env,
                         "'encode()' is not a valid static member function")
        .ThrowAsJavaScriptException();
    return return_value;
  }
  Napi::Function encode_function = encode_function_value.As<Napi::Function>();

  // Call 'encode()' and get the resulting 'Writer'
  Napi::Value writer_value =
      encode_function.Call(message_type, {message_object});
  if (env.IsExceptionPending()) {
    Napi::Error error = env.GetAndClearPendingException();
    std::stringstream error_stream;

    error_stream << "error occurred while attempting to serialize '"
                 << type_name << "': " << error.Message();
    Napi::Error::New(env, error_stream.str()).ThrowAsJavaScriptException();
    return return_value;
  }

  // Verify the writer is an object
  if (!writer_value.IsObject()) {
    Napi::TypeError::New(env, "expected object for result from 'encode()'")
        .ThrowAsJavaScriptException();
    return return_value;
  }
  Napi::Object writer = writer_value.As<Napi::Object>();

  // Verify 'Writer' has a 'finish()' method that's callable
  if (!writer.Has("finish") || !writer.Get("finish").IsFunction()) {
    Napi::TypeError::New(
        env, "expected member 'finish' on type 'Writer' to be function")
        .ThrowAsJavaScriptException();
    return return_value;
  }

  // Call the 'finish()' method and check for errors
  Napi::Value js_result_value =
      writer.Get("finish").As<Napi::Function>().Call(writer, {});
  if (env.IsExceptionPending()) {
    Napi::Error error = env.GetAndClearPendingException();
    std::stringstream error_stream;

    error_stream << "error occurred while attempting to write '" << type_name
                 << "': " << error.Message();
    Napi::Error::New(env, error_stream.str()).ThrowAsJavaScriptException();
    return return_value;
  }

  // Verify the result is a typed array
  if (!js_result_value.IsTypedArray()) {
    Napi::TypeError::New(env, "expected TypedArray from 'finish()'")
        .ThrowAsJavaScriptException();
    return return_value;
  }

  Napi::TypedArray js_array = js_result_value.As<Napi::TypedArray>();
  if (js_array.TypedArrayType() != napi_uint8_array) {
    Napi::TypeError::New(env,
                         "TypedArray of invalid type received from 'finish()'");
    return return_value;
  }

  uint8_t const* data_buffer_ptr = js_array.As<Napi::Uint8Array>().Data();
  std::string js_result(reinterpret_cast<const char*>(data_buffer_ptr),
                        js_array.ElementLength());

  return js_result;
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  return Napi::Function::New(env, CheckMessageWireFormat,
                             "checkMessageWireFormat");
}

NODE_API_MODULE(wiretest, Init)
