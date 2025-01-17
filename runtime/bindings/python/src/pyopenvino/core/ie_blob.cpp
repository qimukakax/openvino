// Copyright (C) 2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "ie_blob.h"

#include <ie_blob.h>
#include <ie_common.h>
#include <ie_layouts.h>

#include <ie_precision.hpp>

#include "pyopenvino/core/ie_blob.hpp"
#include "pyopenvino/core/tensor_description.hpp"

namespace py = pybind11;

void regclass_Blob(py::module m) {
    py::class_<InferenceEngine::Blob, std::shared_ptr<InferenceEngine::Blob>> cls(m, "Blob");
}
