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

#include "rule.h"

#include <libaddressinput/address_field.h>

#include <cstddef>
#include <map>
#include <string>
#include <utility>

#include "address_field_util.h"
#include "grit.h"
#include "messages.h"
#include "region_data_constants.h"
#include "util/json.h"
#include "util/re2ptr.h"
#include "util/string_split.h"

#include <re2/re2.h>

namespace i18n {
namespace addressinput {

namespace {

typedef std::map<std::string, int> NameMessageIdMap;

const char kAdminAreaNameTypeKey[] = "state_name_type";
const char kFormatKey[] = "fmt";
const char kIdKey[] = "id";
const char kLanguagesKey[] = "languages";
const char kLatinFormatKey[] = "lfmt";
const char kLatinNameKey[] = "lname";
const char kNameKey[] = "name";
const char kPostalCodeNameTypeKey[] = "zip_name_type";
const char kRequireKey[] = "require";
const char kSubKeysKey[] = "sub_keys";
const char kZipKey[] = "zip";
const char kPostalCodeExampleKey[] = "zipex";
const char kPostServiceUrlKey[] = "posturl";

// Used as a separator in a list of items. For example, the list of supported
// languages can be "de~fr~it".
const char kSeparator = '~';

NameMessageIdMap InitAdminAreaMessageIds() {
  NameMessageIdMap message_ids;
  message_ids.insert(std::make_pair(
      "area", IDS_LIBADDRESSINPUT_AREA));
  message_ids.insert(std::make_pair(
      "county", IDS_LIBADDRESSINPUT_COUNTY));
  message_ids.insert(std::make_pair(
      "department", IDS_LIBADDRESSINPUT_DEPARTMENT));
  message_ids.insert(std::make_pair(
      "district", IDS_LIBADDRESSINPUT_DISTRICT));
  message_ids.insert(std::make_pair(
      "do_si", IDS_LIBADDRESSINPUT_DO_SI));
  message_ids.insert(std::make_pair(
      "emirate", IDS_LIBADDRESSINPUT_EMIRATE));
  message_ids.insert(std::make_pair(
      "island", IDS_LIBADDRESSINPUT_ISLAND));
  message_ids.insert(std::make_pair(
      "parish", IDS_LIBADDRESSINPUT_PARISH));
  message_ids.insert(std::make_pair(
      "prefecture", IDS_LIBADDRESSINPUT_PREFECTURE));
  message_ids.insert(std::make_pair(
      "province", IDS_LIBADDRESSINPUT_PROVINCE));
  message_ids.insert(std::make_pair(
      "state", IDS_LIBADDRESSINPUT_STATE));
  return message_ids;
}

const NameMessageIdMap& GetAdminAreaMessageIds() {
  static const NameMessageIdMap kAdminAreaMessageIds(InitAdminAreaMessageIds());
  return kAdminAreaMessageIds;
}

NameMessageIdMap InitPostalCodeMessageIds() {
  NameMessageIdMap message_ids;
  message_ids.insert(std::make_pair(
      "postal", IDS_LIBADDRESSINPUT_POSTAL_CODE_LABEL));
  message_ids.insert(std::make_pair(
      "zip", IDS_LIBADDRESSINPUT_ZIP_CODE_LABEL));
  return message_ids;
}

const NameMessageIdMap& GetPostalCodeMessageIds() {
  static const NameMessageIdMap kPostalCodeMessageIds(
      InitPostalCodeMessageIds());
  return kPostalCodeMessageIds;
}

int GetMessageIdFromName(const std::string& name,
                         const NameMessageIdMap& message_ids) {
  NameMessageIdMap::const_iterator it = message_ids.find(name);
  return it != message_ids.end() ? it->second : INVALID_MESSAGE_ID;
}

// Determines whether a given string is a reg-exp or a string. We consider a
// string to be anything that doesn't contain characters with special meanings
// in regular expressions - (, [, \, {, ?. These special characters are all the
// ones that appear in the postal code regular expressions.
bool ContainsRegExSpecialCharacters(const std::string& input) {
  return input.find_first_of("([\\{?") != std::string::npos;
}

}  // namespace

Rule::Rule()
    : id_(),
      format_(),
      latin_format_(),
      required_(),
      sub_keys_(),
      languages_(),
      postal_code_matcher_(NULL),
      sole_postal_code_(),
      admin_area_name_message_id_(INVALID_MESSAGE_ID),
      postal_code_name_message_id_(INVALID_MESSAGE_ID),
      name_(),
      latin_name_(),
      postal_code_example_(),
      post_service_url_() {}

Rule::~Rule() {}

// static
const Rule& Rule::GetDefault() {
  // Allocated once and leaked on shutdown.
  static Rule* default_rule = NULL;
  if (default_rule == NULL) {
    default_rule = new Rule;
    default_rule->ParseSerializedRule(
        RegionDataConstants::GetDefaultRegionData());
  }
  return *default_rule;
}

void Rule::CopyFrom(const Rule& rule) {
  id_ = rule.id_;
  format_ = rule.format_;
  latin_format_ = rule.latin_format_;
  required_ = rule.required_;
  sub_keys_ = rule.sub_keys_;
  languages_ = rule.languages_;
  postal_code_matcher_.reset(
      rule.postal_code_matcher_ == NULL
          ? NULL
          : new RE2ptr(new RE2(rule.postal_code_matcher_->ptr->pattern(),
                               rule.postal_code_matcher_->ptr->options())));
  sole_postal_code_ = rule.sole_postal_code_;
  admin_area_name_message_id_ = rule.admin_area_name_message_id_;
  postal_code_name_message_id_ = rule.postal_code_name_message_id_;
  name_ = rule.name_;
  latin_name_ = rule.latin_name_;
  postal_code_example_ = rule.postal_code_example_;
  post_service_url_ = rule.post_service_url_;
}

bool Rule::ParseSerializedRule(const std::string& serialized_rule) {
  Json json;
  if (!json.ParseObject(serialized_rule)) {
    return false;
  }
  ParseJsonRule(json);
  return true;
}

void Rule::ParseJsonRule(const Json& json) {
  if (json.HasStringValueForKey(kIdKey)) {
    id_ = json.GetStringValueForKey(kIdKey);
  }

  if (json.HasStringValueForKey(kFormatKey)) {
    ParseFormatRule(json.GetStringValueForKey(kFormatKey), &format_);
  }

  if (json.HasStringValueForKey(kLatinFormatKey)) {
    ParseFormatRule(json.GetStringValueForKey(kLatinFormatKey), &latin_format_);
  }

  if (json.HasStringValueForKey(kRequireKey)) {
    ParseAddressFieldsRequired(
        json.GetStringValueForKey(kRequireKey), &required_);
  }

  if (json.HasStringValueForKey(kSubKeysKey)) {
    SplitString(
        json.GetStringValueForKey(kSubKeysKey), kSeparator, &sub_keys_);
  }

  if (json.HasStringValueForKey(kLanguagesKey)) {
    SplitString(
        json.GetStringValueForKey(kLanguagesKey), kSeparator, &languages_);
  }

  sole_postal_code_.clear();
  if (json.HasStringValueForKey(kZipKey)) {
    const std::string& zip = json.GetStringValueForKey(kZipKey);
    // The "zip" field in the JSON data is used in two different ways to
    // validate the postal code. At the country level, the "zip" field indicates
    // a Java compatible regular expression corresponding to all postal codes in
    // the country. At other levels, the regular expression indicates the postal
    // code prefix expected for addresses in that region.
    //
    // In order to make the RE2 object created from the "zip" field useable for
    // both these purposes, the pattern string is here prefixed with "^" to
    // anchor it at the beginning of the string so that it can be used with
    // RE2::PartialMatch() to perform prefix matching or else with
    // RE2::FullMatch() to perform matching against the entire string.
    RE2::Options options;
    options.set_never_capture(true);
    RE2* matcher = new RE2("^(" + zip + ")", options);
    if (matcher->ok()) {
      postal_code_matcher_.reset(new RE2ptr(matcher));
    } else {
      postal_code_matcher_.reset(NULL);
      delete matcher;
    }
    // If the "zip" field is not a regular expression, then it is the sole
    // postal code for this rule.
    if (!ContainsRegExSpecialCharacters(zip)) {
      sole_postal_code_ = zip;
    }
  }

  if (json.HasStringValueForKey(kAdminAreaNameTypeKey)) {
    admin_area_name_message_id_ =
        GetMessageIdFromName(json.GetStringValueForKey(kAdminAreaNameTypeKey),
                             GetAdminAreaMessageIds());
  }

  if (json.HasStringValueForKey(kPostalCodeNameTypeKey)) {
    postal_code_name_message_id_ =
        GetMessageIdFromName(json.GetStringValueForKey(kPostalCodeNameTypeKey),
                             GetPostalCodeMessageIds());
  }

  if (json.HasStringValueForKey(kNameKey)) {
    name_ = json.GetStringValueForKey(kNameKey);
  }

  if (json.HasStringValueForKey(kLatinNameKey)) {
    latin_name_ = json.GetStringValueForKey(kLatinNameKey);
  }

  if (json.HasStringValueForKey(kPostalCodeExampleKey)) {
    postal_code_example_ = json.GetStringValueForKey(kPostalCodeExampleKey);
  }

  if (json.HasStringValueForKey(kPostServiceUrlKey)) {
    post_service_url_ = json.GetStringValueForKey(kPostServiceUrlKey);
  }
}

}  // namespace addressinput
}  // namespace i18n
