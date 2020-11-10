/**
 * Utility functions used across the testing procedures.
 */

#ifndef GAMELIFT_PB_TESTS_UTILITY_H_
#define GAMELIFT_PB_TESTS_UTILITY_H_

// clang-format off
#include <memory>
#include <sstream>
#include <string>

#include <napi.h>
#include <google/protobuf/message.h>
#include <sdk.pb.h>

// clang-format on

using namespace com::amazon::whitewater::auxproxy::pbuffer;

template <typename T>
class MessageFactory {
 public:
  // Assert that the template argument correctly inherits from MessageLite
  static_assert(std::is_base_of<google::protobuf::Message, T>::value);

  static std::shared_ptr<T> create() { return std::make_shared<T>(); }
};

static const std::map<
    std::string, std::function<std::shared_ptr<google::protobuf::Message>()>>
    message_map = {
        {"AcceptPlayerSession", &MessageFactory<AcceptPlayerSession>::create},
        {"ActivateGameSession", &MessageFactory<ActivateGameSession>::create},
        {"AttributeValue", &MessageFactory<AttributeValue>::create},
        {"BackfillMatchmakingRequest",
         &MessageFactory<BackfillMatchmakingRequest>::create},
        {"BackfillMatchmakingResponse",
         &MessageFactory<BackfillMatchmakingResponse>::create},
        {"DescribePlayerSessionsRequest",
         &MessageFactory<DescribePlayerSessionsRequest>::create},
        {"DescribePlayerSessionsResponse",
         &MessageFactory<DescribePlayerSessionsResponse>::create},
        {"GameProperty", &MessageFactory<GameProperty>::create},
        {"GameSession", &MessageFactory<GameSession>::create},
        {"GameSessionActivate", &MessageFactory<GameSessionActivate>::create},
        {"GameSessionTerminate", &MessageFactory<GameSessionTerminate>::create},
        {"GameLiftResponse", &MessageFactory<GameLiftResponse>::create},
        {"GetInstanceCertificate",
         &MessageFactory<GetInstanceCertificate>::create},
        {"GetInstanceCertificateResponse",
         &MessageFactory<GetInstanceCertificateResponse>::create},
        {"Player", &MessageFactory<Player>::create},
        {"PlayerSession", &MessageFactory<PlayerSession>::create},
        {"ProcessEnding", &MessageFactory<ProcessEnding>::create},
        {"ProcessReady", &MessageFactory<ProcessReady>::create},
        {"RemovePlayerSession", &MessageFactory<RemovePlayerSession>::create},
        {"ReportHealth", &MessageFactory<ReportHealth>::create},
        {"StopMatchmakingRequest",
         &MessageFactory<StopMatchmakingRequest>::create},
        {"TerminateProcess", &MessageFactory<TerminateProcess>::create},
        {"UpdateGameSession", &MessageFactory<UpdateGameSession>::create},
        {"UpdatePlayerSessionCreationPolicy",
         &MessageFactory<UpdatePlayerSessionCreationPolicy>::create}};

std::shared_ptr<google::protobuf::Message> GetMessagePointer(
    Napi::Env& env, const std::string& type_name) {
  // Search for the corresponding Protocol Buffer definition with the name of
  // the message
  if (message_map.find(type_name) == message_map.end()) {
    std::stringstream error_stream;
    error_stream << "Unable to find corresponding Protocol Buffer definition "
                    "for message type '"
                 << type_name << "'";
    Napi::Error::New(env, error_stream.str()).ThrowAsJavaScriptException();
    return std::shared_ptr<google::protobuf::Message>(nullptr);
  }

  // Construct a message from the found factory method for a message of type
  // 'name'
  std::shared_ptr<google::protobuf::Message> protobuf_message =
      std::invoke(message_map.find(type_name)->second);

  return protobuf_message;
}

std::shared_ptr<google::protobuf::Message> GetMessagePointer(
    Napi::Env& env, google::protobuf::Descriptor const* const descriptor) {
  return GetMessagePointer(env, descriptor->name());
}

/**
 * Convert the given NodeJS value to the appropriate native type.
 *
 * @param env [in] NodeJS environment object
 * @param node_value [in] NodeJS compatible variable to convert
 * @param value [out] Pointer to native variable to store result of conversion.
 *
 * @return Denotes a successful operation or not. On false then an error was
 * thrown.
 */
template <typename T>
bool ConvertValue(Napi::Env& env, const Napi::Value& node_value, T* value) {
  std::function<bool()> check_function = []() { return false; };

  if constexpr (std::is_same<bool, T>::value) {
    check_function = std::bind(&Napi::Value::IsBoolean, node_value);
  } else if constexpr (std::is_same<int, T>::value ||
                       std::is_same<int64_t, T>::value ||
                       std::is_same<double, T>::value) {
    check_function = std::bind(&Napi::Value::IsNumber, node_value);
  } else if constexpr (std::is_same<std::string, T>::value) {
    check_function = std::bind(&Napi::Value::IsString, node_value);
  }

  if (!check_function()) {
    Napi::TypeError::New(env, "Invalid type returned during conversion")
        .ThrowAsJavaScriptException();
    return false;
  }

  if constexpr (std::is_same<bool, T>::value) {
    *value = node_value.As<Napi::Boolean>().Value();
  } else if constexpr (std::is_same<int, T>::value) {
    *value = node_value.As<Napi::Number>().Int32Value();
  } else if constexpr (std::is_same<int64_t, T>::value) {
    *value = node_value.As<Napi::Number>().Int64Value();
  } else if constexpr (std::is_same<double, T>::value) {
    *value = node_value.As<Napi::Number>().DoubleValue();
  } else if constexpr (std::is_same<std::string, T>::value) {
    *value = node_value.As<Napi::String>().Utf8Value();
  }

  return true;
}

/**
 * Convert the given native value to the equivalent NodeJS type.
 *
 * @tparam T Native type
 * @param env [in] NodeJS environment object
 * @param value [in] Native value to convert
 * @param nodejs_value [out] Pointer to NodeJS value for assignment
 *
 * @return Denotes a successful operation or not. On false then an erro was
 * thrown.
 */
template <typename T>
bool ConvertNative(Napi::Env& env, const T& value, Napi::Value* nodejs_value) {
  if constexpr (std::is_same<bool, T>::value) {
    *nodejs_value = Napi::Boolean::New(env, value);
  } else if constexpr (std::is_same<int, T>::value ||
                       std::is_same<int64_t, T>::value) {
    *nodejs_value = Napi::Number::New(env, value);
  } else if constexpr (std::is_same<std::string, T>::value) {
    *nodejs_value = Napi::String::New(env, value);
  }

  if (env.IsExceptionPending()) {
    Napi::Error error = env.GetAndClearPendingException();
    std::stringstream error_stream;
    error_stream << "Error occurred while attempting to convert native value: "
                 << error.Message();
    Napi::Error::New(env, error_stream.str()).ThrowAsJavaScriptException();
    return false;
  }

  return true;
}

/**
 * This code was retrieved from https://stackoverflow.com/a/5097100/13690190
 *
 * Computes all combinations of a sequence using size(k - first) number of
 * elements in each combination.
 *
 * @tparam Iterator
 * @param first
 * @param k
 * @param last
 */
template <typename Iterator>
inline bool next_combination(const Iterator first, Iterator k,
                             const Iterator last) {
  /* Credits: Thomas Draper */
  if ((first == last) || (first == k) || (last == k)) return false;
  Iterator itr1 = first;
  Iterator itr2 = last;
  ++itr1;
  if (last == itr1) return false;
  itr1 = last;
  --itr1;
  itr1 = k;
  --itr2;
  while (first != itr1) {
    if (*--itr1 < *itr2) {
      Iterator j = k;
      while (!(*itr1 < *j)) ++j;
      std::iter_swap(itr1, j);
      ++itr1;
      ++j;
      itr2 = k;
      std::rotate(itr1, j, last);
      while (last != j) {
        ++j;
        ++itr2;
      }
      std::rotate(k, itr2, last);
      return true;
    }
  }
  std::rotate(first, k, last);
  return false;
}

#endif  // GAMELIFT_PB_TESTS_UTILITY_H_
