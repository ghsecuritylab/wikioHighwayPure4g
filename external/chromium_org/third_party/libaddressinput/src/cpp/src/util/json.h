// Copyright (C) 2013 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef I18N_ADDRESSINPUT_UTIL_JSON_H_
#define I18N_ADDRESSINPUT_UTIL_JSON_H_

#include <libaddressinput/util/basictypes.h>
#include <libaddressinput/util/scoped_ptr.h>

#include <string>
#include <vector>

namespace i18n {
namespace addressinput {

// Parses a JSON dictionary of strings. Sample usage:
//    Json json;
//    if (json.ParseObject("{'key1':'value1', 'key2':'value2'}") &&
//        json.HasStringKey("key1")) {
//      Process(json.GetStringValueForKey("key1"));
//    }
class Json {
 public:
  Json();
  ~Json();

  // Parses the |json| string and returns true if |json| is valid and it is an
  // object.
  bool ParseObject(const std::string& json);

  // Returns the list of keys in the parsed JSON. The JSON object must be parsed
  // successfully in ParseObject() before invoking this method.
  const std::vector<std::string>& GetKeys() const;

  // Returns true if the parsed JSON contains a string value for |key|. The JSON
  // object must be parsed successfully in ParseObject() before invoking this
  // method.
  bool HasStringValueForKey(const std::string& key) const;

  // Returns the string value for the |key|. The |key| must be present and its
  // value must be of string type, i.e., HasStringValueForKey(key) must return
  // true before invoking this method.
  std::string GetStringValueForKey(const std::string& key) const;

  // Returns true if the parsed JSON contains a dictionary value for |key|. The
  // JSON object must be parsed successfully in ParseObject() before invoking
  // this method.
  bool HasDictionaryValueForKey(const std::string& key) const;

  // Returns the dictionary value for the |key|. The |key| must be present and
  // its value must be of the dictionary type, i.e., HasDictionaryValueForKey()
  // must return true before invoking this method.
  const Json& GetDictionaryValueForKey(const std::string& key) const;

 private:
  class JsonImpl;
  scoped_ptr<JsonImpl> impl_;

  DISALLOW_COPY_AND_ASSIGN(Json);
};

}  // namespace addressinput
}  // namespace i18n

#endif  // I18N_ADDRESSINPUT_UTIL_JSON_H_
