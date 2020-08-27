
/**
 * Functions for generating random values from Javascript.
 */

#ifndef GAMELIFT_PB_TESTS_POPULATE_H_
#define GAMELIFT_PB_TESTS_POPULATE_H_

// clang-format off
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <type_traits>

#include <napi.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>

#include "utility.hh"
// clang-format on

const int kMaxRepeatedInstances = 10;

static std::function<bool(bool*)> random_boolean;
static std::function<bool(int*)> random_integer_32;
static std::function<bool(int64_t*)> random_integer_64;
static std::function<bool(double*)> random_double;
static std::function<bool(std::string*)> random_string;

template <typename T>
bool PopulateProtocolBufferArrayField(
    google::protobuf::Reflection const* const reflection,
    google::protobuf::Message* const message,
    google::protobuf::FieldDescriptor const* const field_descriptor);

bool PopulateProtocolBufferField(
    Napi::Env& env, google::protobuf::Reflection const* const reflection,
    google::protobuf::Message* const message,
    google::protobuf::FieldDescriptor const* const field_descriptor);

/**
 * Factory class with static methods for generating random data.
 *
 * @tparam T - Template parameter which denotes the type of random data to
 * generate.
 * @tparam random_type - Non-type template parameter used for selecting the
 * function from 'faker.random' to use to generate the data.
 */
template <typename T, const char* random_type>
class RandomValueFactory {
 public:
  /**
   * Generate random data from the given random module and value.
   *
   * @param env [in] NodeJS environment object
   * @param random_module [in] Previously loaded 'faker.random' object.
   * @param value [out] Pointer to the value to assign
   *
   * @return Denotes a successful operation or not. On false then an error was
   * thrown.
   */
  static bool GenerateRandomValue(Napi::Env& env,
                                  const Napi::Object& random_module, T* value) {
    if (!random_module.Has(random_type) ||
        !random_module.Get(random_type).IsFunction()) {
      std::stringstream error_stream;
      error_stream << "Expected 'faker.random' to have '"
                   << std::string(random_type) << "' member function";
      Napi::TypeError::New(env, error_stream.str())
          .ThrowAsJavaScriptException();
      return false;
    }

    Napi::Function random_value_generator =
        random_module.Get(random_type).As<Napi::Function>();
    Napi::Value random_value = random_value_generator.Call({});
    if (env.IsExceptionPending()) {
      env.GetAndClearPendingException().ThrowAsJavaScriptException();
      return false;
    }

    if (!ConvertValue<T>(env, random_value, value)) {
      // No need to do further error creation here as ConvertValue should
      // have already done that.
      return false;
    }

    return true;
  }
};

constexpr const char boolean[] = "boolean";
constexpr const char number[] = "number";
constexpr const char word[] = "word";

typedef RandomValueFactory<bool, boolean> RandomBooleanFactory;
typedef RandomValueFactory<int, number> RandomInt32Factory;
typedef RandomValueFactory<int64_t, number> RandomInt64Factory;
typedef RandomValueFactory<double, number> RandomDoubleFactory;
typedef RandomValueFactory<std::string, word> RandomStringFactory;

template <typename T>
std::vector<T> GenerateRandomVector(
    std::function<bool(T*)> random_value_function) {
  const size_t number_instances = std::rand() % kMaxRepeatedInstances + 1;

  std::vector<T> random_values(number_instances);

  for (size_t i = 0; i < number_instances; ++i) {
    T random_value;
    if (random_value_function(&random_value)) {
      random_values[i] = random_value;
    }
  }

  return random_values;
}

int Factorial(int n, int total) {
  return (n == 1 || n == 0) ? total : Factorial(n - 1, n * total);
}

int Factorial(int n) { return Factorial(n, 1); }

bool RandomMessage(Napi::Env& env,
                   google::protobuf::Reflection const* const reflection,
                   google::protobuf::Descriptor const* const descriptor,
                   google::protobuf::Message* message) {
  // Determine what fields will be set at random
  const int num_fields = descriptor->field_count();

  // Initialize the choices with the size of total number of them
  std::vector<int> possible_field_indices(num_fields);
  // Fill in the choices with 0, 1, ..., num_fields
  std::generate(possible_field_indices.begin(), possible_field_indices.end(),
                [n = 0]() mutable { return n++; });

  // Number of fields we're going to set
  const size_t k = std::rand() % num_fields + 1;

  // Size of the entire sample
  const size_t n = num_fields;

  // Size of our n choose k
  const size_t combinations = Factorial(n) / (Factorial(k) * Factorial(n - k));

  const size_t pick = std::rand() % combinations;

  // Generate combinations until our chosen one
  for (size_t i = 0; i < pick; ++i) {
    next_combination(possible_field_indices.begin(),
                     possible_field_indices.begin() + k,
                     possible_field_indices.end());
  }

  for (size_t i = 0; i < k; ++i) {
    google::protobuf::FieldDescriptor const* const field_descriptor =
        descriptor->field(possible_field_indices[i]);

    if (!PopulateProtocolBufferField(env, reflection, message,
                                     field_descriptor)) {
      return false;
    }
  }

  return true;
}

/**
 *
 */
template <typename T>
bool PopulateProtocolBufferArrayField(
    google::protobuf::Reflection const* const reflection,
    google::protobuf::Message* const message,
    google::protobuf::FieldDescriptor const* const field_descriptor) {
  std::function<bool(T*)> random_value_generator;
  if constexpr (std::is_same<T, bool>::value) {
    random_value_generator = random_boolean;
  } else if constexpr (std::is_same<T, int>::value) {
    random_value_generator = random_integer_32;
  } else if constexpr (std::is_same<T, int64_t>::value) {
    random_value_generator = random_integer_64;
  } else if constexpr (std::is_same<T, std::string>::value) {
    random_value_generator = random_string;
  }

  const std::vector<T> random_values =
      GenerateRandomVector<T>(random_value_generator);

  for (size_t i = 0; i < random_values.size(); ++i) {
    if constexpr (std::is_same<T, bool>::value) {
      reflection->AddBool(message, field_descriptor, random_values[i]);
    } else if constexpr (std::is_same<T, int>::value) {
      reflection->AddInt32(message, field_descriptor, random_values[i]);
    } else if constexpr (std::is_same<T, int64_t>::value) {
      reflection->AddInt64(message, field_descriptor, random_values[i]);
    } else if constexpr (std::is_same<T, double>::value) {
      reflection->AddDouble(message, field_descriptor, random_values[i]);
    } else if constexpr (std::is_same<T, std::string>::value) {
      reflection->AddString(message, field_descriptor, random_values[i]);
    }
  }

  return true;
}

template <typename T>
bool PopulateProtocolBufferSingularField(
    google::protobuf::Reflection const* const reflection,
    google::protobuf::Message* const message,
    google::protobuf::FieldDescriptor const* const field_descriptor) {
  std::function<bool(T*)> random_value_function;
  if constexpr (std::is_same<T, bool>::value) {
    random_value_function = random_boolean;
  } else if constexpr (std::is_same<T, int>::value) {
    random_value_function = random_integer_32;
  } else if constexpr (std::is_same<T, int64_t>::value) {
    random_value_function = random_integer_64;
  } else if constexpr (std::is_same<T, double>::value) {
    random_value_function = random_double;
  } else if constexpr (std::is_same<T, std::string>::value) {
    random_value_function = random_string;
  }

  T value;
  if (!random_value_function(&value)) {
    return false;
  }

  if constexpr (std::is_same<T, bool>::value) {
    reflection->SetBool(message, field_descriptor, value);
  } else if constexpr (std::is_same<T, int>::value) {
    reflection->SetInt32(message, field_descriptor, value);
  } else if constexpr (std::is_same<T, int64_t>::value) {
    reflection->SetInt64(message, field_descriptor, value);
  } else if constexpr (std::is_same<T, std::string>::value) {
    reflection->SetString(message, field_descriptor, value);
  }

  return true;
}

bool PopulateProtocolBufferField(
    Napi::Env& env, google::protobuf::Reflection const* const reflection,
    google::protobuf::Message* const message,
    google::protobuf::FieldDescriptor const* const field_descriptor) {
  if (field_descriptor->type() ==
      google::protobuf::FieldDescriptor::TYPE_MESSAGE) {
    // Treat maps differently, because for each map entry we **always** want to
    // set the key & value
    if (field_descriptor->is_map()) {
      google::protobuf::Descriptor const* const descriptor =
          field_descriptor->message_type();

      // Randomize how many entries in the map
      const size_t num_entries = std::rand() % kMaxRepeatedInstances + 1;

      for (size_t i = 0; i < num_entries; i++) {
        google::protobuf::Message* map_message =
            reflection->AddMessage(message, field_descriptor);
        google::protobuf::Reflection const* const map_reflection =
            map_message->GetReflection();

        // Populate the key field (index 0)
        if (!PopulateProtocolBufferField(env, map_reflection, map_message,
                                         descriptor->field(0))) {
          return false;
        }

        // Populate the value field (index 1)
        if (!PopulateProtocolBufferField(env, map_reflection, map_message,
                                         descriptor->field(1))) {
          return false;
        }
      }

      return true;
    }

    const int number_instances = field_descriptor->is_repeated()
                                     ? std::rand() % kMaxRepeatedInstances + 1
                                     : 1;
    google::protobuf::Descriptor const* const sub_descriptor =
        field_descriptor->message_type();

    for (size_t i = 0; i < number_instances; ++i) {
      std::shared_ptr<google::protobuf::Message> sub_message =
          GetMessagePointer(env, sub_descriptor);
      google::protobuf::Reflection const* const sub_reflection =
          sub_message->GetReflection();

      if (!RandomMessage(env, sub_reflection, sub_descriptor,
                         sub_message.get())) {
        return false;
      }

      google::protobuf::Message* sub_mutable_message;
      if (field_descriptor->is_repeated()) {
        sub_mutable_message = reflection->AddMessage(message, field_descriptor);
      } else {
        sub_mutable_message =
            reflection->MutableMessage(message, field_descriptor);
      }
      sub_mutable_message->CopyFrom(*sub_message);
    }

    return true;
  }

  std::function<bool(google::protobuf::Reflection const* const,
                     google::protobuf::Message* const,
                     google::protobuf::FieldDescriptor const* const)>
      population_function;

  if (field_descriptor->is_repeated()) {
    switch (field_descriptor->type()) {
      case google::protobuf::FieldDescriptor::Type::TYPE_BOOL: {
        population_function = &PopulateProtocolBufferArrayField<bool>;
        break;
      }
      case google::protobuf::FieldDescriptor::Type::TYPE_INT32: {
        population_function = &PopulateProtocolBufferArrayField<int>;
        break;
      }
      case google::protobuf::FieldDescriptor::Type::TYPE_INT64: {
        population_function = &PopulateProtocolBufferArrayField<int64_t>;
        break;
      }
      case google::protobuf::FieldDescriptor::Type::TYPE_DOUBLE: {
        population_function = &PopulateProtocolBufferSingularField<double>;
        break;
      }
      case google::protobuf::FieldDescriptor::Type::TYPE_STRING: {
        population_function = &PopulateProtocolBufferArrayField<std::string>;
        break;
      }
      // Handle enums differently since they could have any range of values
      case google::protobuf::FieldDescriptor::TYPE_ENUM: {
        google::protobuf::EnumDescriptor const* const enum_descriptor =
            field_descriptor->enum_type();

        const size_t num_entries = std::rand() % kMaxRepeatedInstances + 1;
        for (size_t i = 0; i < num_entries; ++i) {
          int value = std::rand() % enum_descriptor->value_count();
          reflection->AddEnumValue(message, field_descriptor, value);
        }
        return true;
      }
      default: {
        std::stringstream error_stream;
        error_stream << "Unsupported type found on field '"
                     << message->GetDescriptor()->name() << "."
                     << field_descriptor->name() << "'";
        Napi::Error::New(env, error_stream.str()).ThrowAsJavaScriptException();
        return false;
      }
    }
  } else {
    switch (field_descriptor->type()) {
      case google::protobuf::FieldDescriptor::Type::TYPE_BOOL: {
        population_function = &PopulateProtocolBufferSingularField<bool>;
        break;
      }
      case google::protobuf::FieldDescriptor::Type::TYPE_INT32: {
        population_function = &PopulateProtocolBufferSingularField<int>;
        break;
      }
      case google::protobuf::FieldDescriptor::Type::TYPE_INT64: {
        population_function = &PopulateProtocolBufferSingularField<int64_t>;
        break;
      }
      case google::protobuf::FieldDescriptor::Type::TYPE_DOUBLE: {
        population_function = &PopulateProtocolBufferSingularField<double>;
        break;
      }
      case google::protobuf::FieldDescriptor::Type::TYPE_STRING: {
        population_function = &PopulateProtocolBufferSingularField<std::string>;
        break;
      }
      // Handle enums differently since they could have any range of values
      case google::protobuf::FieldDescriptor::TYPE_ENUM: {
        google::protobuf::EnumDescriptor const* const enum_descriptor =
            field_descriptor->enum_type();

        int value = std::rand() % enum_descriptor->value_count();

        reflection->SetEnumValue(message, field_descriptor, value);
        return true;
      }
      default: {
        std::stringstream error_stream;
        error_stream << "Unsupported type found on field '"
                     << message->GetDescriptor()->name() << "."
                     << field_descriptor->name() << "'";
        Napi::Error::New(env, error_stream.str()).ThrowAsJavaScriptException();
        return false;
      }
    }
  }

  return population_function(reflection, message, field_descriptor);
}

#endif  // GAMELIFT_PB_TESTS_POPULATE_H_
