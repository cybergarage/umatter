/******************************************************************
 *
 * Copyright (C) 2025 The uMatter for C Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include <boost/test/unit_test.hpp>

#include <stdlib.h>

#include <umatter/util/bytes.h>

BOOST_AUTO_TEST_CASE(Bytes)
{
  byte* ibytes;
  int n;

  for (n = 0; n <= 0xFF; n++) {
    ibytes = umatter_bytes_fromint(n, 1);
    BOOST_CHECK_EQUAL(umatter_bytes_toint(ibytes, 1), n);
    free(ibytes);
  }

  for (n = 0; n <= 0xFFFF; n += (0xFFFF / 0xFF)) {
    ibytes = umatter_bytes_fromint(n, 2);
    BOOST_CHECK_EQUAL(umatter_bytes_toint(ibytes, 2), n);
    free(ibytes);
  }

  for (n = 0; n <= 0xFFFFFF; n += (0xFFFFFF / 0xFFFF)) {
    ibytes = umatter_bytes_fromint(n, 3);
    BOOST_CHECK_EQUAL(umatter_bytes_toint(ibytes, 3), n);
    free(ibytes);
  }
}
