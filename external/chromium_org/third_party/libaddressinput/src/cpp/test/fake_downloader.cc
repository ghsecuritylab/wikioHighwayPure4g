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

#include "fake_downloader.h"

#include <cassert>
#include <fstream>
#include <map>
#include <string>
#include <utility>

#include "lookup_key_util.h"

namespace i18n {
namespace addressinput {

// static
const char FakeDownloader::kFakeDataUrl[] = "test:///plain/";

// static
const char FakeDownloader::kFakeAggregateDataUrl[] = "test:///aggregate/";

namespace {

// The name of the test data file.
const char kDataFileName[] = TEST_DATA_DIR "/countryinfo.txt";

// The number of characters in the fake data URL prefix.
const size_t kFakeDataUrlLength = sizeof FakeDownloader::kFakeDataUrl - 1;

// The number of characters in the fake aggregate data URL prefix.
const size_t kFakeAggregateDataUrlLength =
    sizeof FakeDownloader::kFakeAggregateDataUrl - 1;

// Each data key begins with this string. Example of a data key:
//     data/CH/AG
const char kDataKeyPrefix[] = "data/";

// The number of characters in the data key prefix.
const size_t kDataKeyPrefixLength = sizeof kDataKeyPrefix - 1;

// The number of characters in a CLDR region code, e.g. 'CH'.
const size_t kCldrRegionCodeLength = 2;

// The number of characters in an aggregate data key, e.g. 'data/CH'.
const size_t kAggregateDataKeyLength =
    kDataKeyPrefixLength + kCldrRegionCodeLength;

const LookupKeyUtil& GetLookupKeyUtil() {
  static const LookupKeyUtil kLookupKeyUtil(FakeDownloader::kFakeDataUrl);
  return kLookupKeyUtil;
}

const LookupKeyUtil& GetAggregateLookupKeyUtil() {
  static const LookupKeyUtil kLookupKeyUtil(
      FakeDownloader::kFakeAggregateDataUrl);
  return kLookupKeyUtil;
}

std::map<std::string, std::string> InitData() {
  std::map<std::string, std::string> data;
  std::ifstream file(kDataFileName);
  assert(file.is_open());

  std::string line;
  while (file.good()) {
    // Example line from countryinfo.txt:
    //     data/CH/AG={"name": "Aargau"}
    std::getline(file, line);

    std::string::size_type divider = line.find('=');
    if (divider != std::string::npos) {
      // Example key:
      //     data/CH/AG
      const std::string& key = line.substr(0, divider);

      // Example value:
      //     {"name": "Aargau"}
      const std::string& value = line.substr(divider + 1);

      // For example, map 'test:///plain/data/CH/AG' to '{"name": "Aargau"}'.
      data.insert(std::make_pair(GetLookupKeyUtil().GetUrlForKey(key), value));

      // Aggregate keys that begin with 'data/'. We don't aggregate keys that
      // begin with 'example/' because example data is not used anywhere.
      if (key.compare(0,
                      kDataKeyPrefixLength,
                      kDataKeyPrefix,
                      kDataKeyPrefixLength) == 0) {
        // Example aggregate URL:
        //     test:///aggregate/data/CH
        const std::string& aggregate_url = GetAggregateLookupKeyUtil()
            .GetUrlForKey(key.substr(0, kAggregateDataKeyLength));

        std::map<std::string, std::string>::iterator aggregate_data_it =
            data.find(aggregate_url);
        if (aggregate_data_it != data.end()) {
          // Append more data to the aggregate string, for example:
          //     , "data/CH/AG": {"name": "Aargau"}
          aggregate_data_it->second.append(", \"" + key + "\": " + value);
        } else {
          // Begin a new aggregate string, for example:
          //     {"data/CH/AG": {"name": "Aargau"}
          data.insert(
              std::make_pair(aggregate_url, "{\"" + key + "\": " + value));
        }
      }
    }
  }
  file.close();

  // Make the aggregate data strings valid. For example, this incomplete JSON
  // data:
  //     {"data/CH/AG": {"name": "Aargau"},
  //      "data/CH": {"name": "SWITZERLAND"}
  //
  // becomes valid JSON data like so:
  //
  //     {"data/CH/AG": {"name": "Aargau"},
  //      "data/CH": {"name": "SWITZERLAND"}}
  for (std::map<std::string, std::string>::iterator data_it = data.begin();
       data_it != data.end(); ++data_it) {
    if (data_it->first.compare(0,
                               kFakeAggregateDataUrlLength,
                               FakeDownloader::kFakeAggregateDataUrl,
                               kFakeAggregateDataUrlLength) == 0) {
      data_it->second.append("}");
    }
  }

  return data;
}

const std::map<std::string, std::string>& GetData() {
  static const std::map<std::string, std::string> kData(InitData());
  return kData;
}

}  // namespace

FakeDownloader::FakeDownloader() {}

FakeDownloader::~FakeDownloader() {}

void FakeDownloader::Download(const std::string& url,
                              const Callback& downloaded) const {
  std::map<std::string, std::string>::const_iterator data_it =
      GetData().find(url);
  bool success = data_it != GetData().end();
  std::string* data = NULL;
  if (success) {
    data = new std::string(data_it->second);
  } else if (GetLookupKeyUtil().IsValidationDataUrl(url) ||
             GetAggregateLookupKeyUtil().IsValidationDataUrl(url)) {
    // URLs that start with "https://i18napis.appspot.com/ssl-address/" or
    // "https://i18napis.appspot.com/ssl-aggregate-address/" prefix, but do not
    // have associated data will always return "{}" with status code 200.
    // FakeDownloader imitates this behavior for URLs that start with
    // "test://address/" and "test://aggregate-address/" prefixes.
    success = true;
    data = new std::string("{}");
  }
  downloaded(success, url, data);
}

}  // namespace addressinput
}  // namespace i18n
