// Copyright (C) 2018-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include <node_context.hpp>

#include "default_opset.hpp"

namespace ngraph {
namespace frontend {
namespace pdpd {
namespace op {
NamedOutputs layer_norm(const NodeContext& node) {
    using namespace default_opset;
    const auto data = node.get_ng_input("X");
    const auto epsilon = node.get_attribute<float>("epsilon", 1e-05);
    const auto begin_norm_axis = node.get_attribute<int32_t>("begin_norm_axis", 1);
    // The limitation from:
    // https://github.com/PaddlePaddle/Paddle/blob/cec36ea6ff16fda90c1a004c6e043cd9b2096a2a/paddle/fluid/operators/layer_norm_op.cc#L176
    PDPD_ASSERT(begin_norm_axis > 0, "begin_norm_axis should be greater than 0");

    // shape of input
    const auto shape_of_node = std::make_shared<ShapeOf>(data);
    // dims of input, reduce to scalar
    const auto dims_node = std::make_shared<ReduceMin>(std::make_shared<ShapeOf>(shape_of_node),
                                                       Constant::create(element::i64, {1}, {0}),
                                                       false);
    // get axis list to do the computation: [begin_norm_axis: dims)
    const auto axis = std::make_shared<Range>(Constant::create(element::i64, {}, {begin_norm_axis}),
                                              dims_node,
                                              Constant::create(element::i64, {}, {1}),
                                              element::i64);
    // 'Scale' and 'Bias' are in plain, shoule get the real shape. The shape: shape_of_node[begin_norm_axis:-1]
    const auto scale_bias_shape = std::make_shared<StridedSlice>(shape_of_node,
                                                                 Constant::create(element::i64, {1}, {begin_norm_axis}),
                                                                 Constant::create(element::i64, {1}, {0}),
                                                                 std::vector<int64_t>{0},
                                                                 std::vector<int64_t>{1});

    const auto mvn = std::make_shared<MVN>(data, axis, true, epsilon, ngraph::op::MVNEpsMode::INSIDE_SQRT);
    std::shared_ptr<ngraph::Node> result = mvn;
    if (node.has_ng_input("Scale")) {
        const auto s = node.get_ng_input("Scale");
        const auto reshaped_s = std::make_shared<Reshape>(s, scale_bias_shape, false);
        result = std::make_shared<Multiply>(mvn, reshaped_s);
    }

    if (node.has_ng_input("Bias")) {
        const auto b = node.get_ng_input("Bias");
        const auto reshaped_b = std::make_shared<Reshape>(b, scale_bias_shape, false);
        result = std::make_shared<Add>(result, reshaped_b);
    }

    return node.default_single_output_mapping({result}, {"Y"});
}
}  // namespace op
}  // namespace pdpd
}  // namespace frontend
}  // namespace ngraph