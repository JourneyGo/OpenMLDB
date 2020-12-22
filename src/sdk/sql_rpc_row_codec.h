/*
 * sql_rpc_row_codec.h
 * Copyright (C) 4paradigm.com 2020 wangtaize <wangtaize@4paradigm.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SRC_SDK_SQL_RPC_ROW_CODEC_H_
#define SRC_SDK_SQL_RPC_ROW_CODEC_H_

#include <memory>
#include <set>
#include <string>
#include <vector>

#include "base/fe_slice.h"
#include "butil/iobuf.h"
#include "codec/fe_row_codec.h"
#include "codec/row.h"
#include "sdk/base.h"

namespace rtidb {
namespace sdk {

bool DecodeRpcRow(const butil::IOBuf& buf, size_t offset, size_t size, size_t slice_num, fesql::codec::Row* row);

bool EncodeRpcRow(const fesql::codec::Row& row, butil::IOBuf* buf, size_t* total_size);

bool EncodeRpcRow(const int8_t* buf, size_t size, butil::IOBuf* io_buf);

}  // namespace sdk
}  // namespace rtidb
#endif  // SRC_SDK_SQL_RPC_ROW_CODEC_H_
