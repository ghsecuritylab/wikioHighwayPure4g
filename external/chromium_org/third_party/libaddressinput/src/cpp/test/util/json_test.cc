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

#include "util/json.h"

#include <string>
#include <vector>

#include <gtest/gtest.h>

namespace {

using i18n::addressinput::Json;
using i18n::addressinput::scoped_ptr;

TEST(JsonTest, EmptyStringIsNotValid) {
  Json json;
  EXPECT_FALSE(json.ParseObject(std::string()));
}

TEST(JsonTest, EmptyDictionaryContainsNoKeys) {
  Json json;
  ASSERT_TRUE(json.ParseObject("{}"));
  EXPECT_FALSE(json.HasStringValueForKey("key"));
  EXPECT_FALSE(json.HasStringValueForKey(std::string()));
}

TEST(JsonTest, InvalidJsonIsNotValid) {
  Json json;
  EXPECT_FALSE(json.ParseObject("{"));
}

TEST(JsonTest, OneKeyIsValid) {
  Json json;
  ASSERT_TRUE(json.ParseObject("{\"key\": \"value\"}"));
  ASSERT_TRUE(json.HasStringValueForKey("key"));
  EXPECT_EQ("value", json.GetStringValueForKey("key"));
}

TEST(JsonTest, EmptyStringKeyIsNotInObject) {
  Json json;
  ASSERT_TRUE(json.ParseObject("{\"key\": \"value\"}"));
  EXPECT_FALSE(json.HasStringValueForKey(std::string()));
}

TEST(JsonTest, EmptyKeyIsValid) {
  Json json;
  ASSERT_TRUE(json.ParseObject("{\"\": \"value\"}"));
  ASSERT_TRUE(json.HasStringValueForKey(std::string()));
  EXPECT_EQ("value", json.GetStringValueForKey(std::string()));
}

TEST(JsonTest, EmptyValueIsValid) {
  Json json;
  ASSERT_TRUE(json.ParseObject("{\"key\": \"\"}"));
  ASSERT_TRUE(json.HasStringValueForKey("key"));
  EXPECT_TRUE(json.GetStringValueForKey("key").empty());
}

TEST(JsonTest, Utf8EncodingIsValid) {
  Json json;
  ASSERT_TRUE(json.ParseObject("{\"key\": \"Ü\"}"));
  ASSERT_TRUE(json.HasStringValueForKey("key"));
  EXPECT_EQ("Ü", json.GetStringValueForKey("key"));
}

TEST(JsonTest, InvalidUtf8IsNotValid) {
  Json json;
  EXPECT_FALSE(json.ParseObject("{\"key\": \"\xC3\x28\"}"));
}

TEST(JsonTest, NullInMiddleIsNotValid) {
  Json json;
  static const char kJson[] = "{\"key\": \"val\0ue\"}";
  EXPECT_FALSE(json.ParseObject(std::string(kJson, sizeof kJson - 1)));
}

TEST(JsonTest, TwoKeysAreValid) {
  Json json;
  ASSERT_TRUE(json.ParseObject("{\"key1\": \"value1\", \"key2\": \"value2\"}"));
  ASSERT_TRUE(json.HasStringValueForKey("key1"));
  EXPECT_EQ("value1", json.GetStringValueForKey("key1"));

  ASSERT_TRUE(json.HasStringValueForKey("key2"));
  EXPECT_EQ("value2", json.GetStringValueForKey("key2"));
}

TEST(JsonTest, ListIsNotValid) {
  Json json;
  EXPECT_FALSE(json.ParseObject("[]"));
}

TEST(JsonTest, StringIsNotValid) {
  Json json;
  EXPECT_FALSE(json.ParseObject("\"value\""));
}

TEST(JsonTest, NumberIsNotValid) {
  Json json;
  EXPECT_FALSE(json.ParseObject("3"));
}

TEST(JsonTest, NoDictionaryFound) {
  Json json;
  ASSERT_TRUE(json.ParseObject("{\"key\":\"value\"}"));
  EXPECT_FALSE(json.HasDictionaryValueForKey("key"));
}

TEST(JsonTest, DictionaryFound) {
  Json json;
  ASSERT_TRUE(json.ParseObject("{\"key\":{\"inner_key\":\"value\"}}"));
  ASSERT_TRUE(json.HasDictionaryValueForKey("key"));
  const Json& sub_json = json.GetDictionaryValueForKey("key");
  ASSERT_TRUE(sub_json.HasStringValueForKey("inner_key"));
  EXPECT_EQ("value", sub_json.GetStringValueForKey("inner_key"));
}

TEST(JsonTest, DictionariesHaveKeys) {
  Json json;
  ASSERT_TRUE(json.ParseObject("{\"key\":{\"inner_key\":\"value\"}}"));
  std::vector<std::string> expected_keys(1, "key");
  EXPECT_EQ(expected_keys, json.GetKeys());

  ASSERT_TRUE(json.HasDictionaryValueForKey("key"));
  const Json& sub_json = json.GetDictionaryValueForKey("key");
  std::vector<std::string> expected_sub_keys(1, "inner_key");
  EXPECT_EQ(expected_sub_keys, sub_json.GetKeys());
}

}  // namespace
